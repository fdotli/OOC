# include "Ic.h"
# include "Ic.r"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "List.h"

static int Ic_puto (const void * _self, FILE * fp) {
	int result;
	const struct Ic * self = cast(Ic(), _self);

	result = super_puto(Ic(), self, fp);
	result += fputs("wired to ", fp);
	if (! self -> out)
		return result + fputs("[nil]\n", fp);
	return result + puto(self -> out, fp);
}

static void Ic_wire (void * to, void * _self) {
	struct Ic * self = cast(Ic(), _self);

	self -> out = to;
}

static enum react Ic_gate (void * _self, const void * item) {
	struct Ic * self = cast(Ic(), _self);

	return self -> out ? gate(self -> out, item) : reject;
}

static void * Mux_ctor (void * _self, va_list * app) {
	struct Mux * self = cast(Mux(), _self);

	self -> list = new(List(), 1);
	return self;
}

static void * Mux_dtor (void * _self) {
	struct Mux * self = cast(Mux(), _self);

	delete(self -> list), self -> list = 0;
	return super_dtor(Mux(), self);
}

static int Mux_puto (const void * _self, FILE * fp) {
	int result;
	const struct Mux * self = cast(Mux(), _self);

	result = super_puto(Mux(), self, fp);
	result += fputs("list ", fp);
	return result + puto(self -> list, fp);
}

static void Mux_wire (void * to, void * _self) {
	struct Mux * self = cast(Mux(), _self);

	addLast(self -> list, to);
}

static enum react Mux_gate (void * _self, const void * item) {
	unsigned i, n;
	enum react result = reject;
	struct Mux * self = cast(Mux(), _self);

	n = count(self -> list);
	for (i = 0; i < n; ++ i)
	{	result = gate(lookAt(self -> list, i), item);
		if (result == accept)
			break;
	}
	return result;
}

static enum react LineOut_gate (void * _self, const void * item) {
	struct LineOut * self = cast(LineOut(), _self);

	assert(item);
	puts(item);
	return accept;
}

static void * Button_ctor (void * _self, va_list * app) {
	struct Button * self = super_ctor(Button(), _self, app);

	self -> text = va_arg(* app, const char *);
	return self;
}

static int Button_puto (const void * _self, FILE * fp) {
	int result;
	const struct Button * self = cast(Button(), _self);

	result = super_puto(Button(), self, fp);
	return result + fprintf(fp, "text \"%s\"\n", self -> text);
}

static enum react Button_gate (void * _self, const void * item) {
	struct Button * self = cast(Button(), _self);

	if (item && kind(item) == 0
				&& strcmp(data(item), self -> text))
		return reject;
	return super_gate(Button(), self, self -> text);
}

static void * Event_ctor (void * _self, va_list * app) {
	struct Event * self = super_ctor(Event(), _self, app);

	self -> kind = va_arg(* app, int);
	self -> data = va_arg(* app, void *);
	return self;
}

static int Event_puto (const void * _self, FILE * fp) {
	int result;
	const struct Event * self = cast(Event(), _self);

	result = super_puto(Event(), self, fp);
	result += fprintf(fp, "kind %d, data ", self -> kind);
	return result + fprintf(fp, self -> kind ? "%p\n" : "\"%s\"\n",
					self -> data);
}

int kind (const void * _self) {
	const struct Event * self = cast(Event(), _self);

	return self -> kind;
}

void * data (const void * _self) {
	const struct Event * self = cast(Event(), _self);

	return self -> data;
}

static int Calc_puto (const void * _self, FILE * fp) {
	int result;
	const struct Calc * self = cast(Calc(), _self);

	result = super_puto(Calc(), self, fp);
	return result + fprintf(fp, "state %d, op '%c', value[] %d, %d\n",
		self -> state,
		self -> op ? self -> op : ' ',
		self -> value[0], self -> value[1]);
}

static char * itoa (int value)
{	static char buf [3 * sizeof value + 2];

	sprintf(buf, "%d", value);
	return buf;
}

static void cpu (struct Calc * self)
{
	if (self -> state)
		switch (self -> op) {
		case '+':
			self -> value[0] += self -> value[1];
			break;
		case '-':
			self -> value[0] -= self -> value[1];
			break;
		case '*':
			self -> value[0] *= self -> value[1];
			break;
		case '/':
			if (self -> value[1])
				self -> value[0] /= self -> value[1];
			else
				self -> value[0] |= ~0U >> 1;
		}
}

static enum react Calc_gate (void * _self, const void * item) {
	struct Calc * self = cast(Calc(), _self);

	const char * text = item;

	switch (text[0]) {
	case 'Q':
		exit(0);
	default:
		if (isdigit(text[0] & 0xff))
		{	self -> value[self -> state] *= 10;
			self -> value[self -> state] += text[0] - '0';
			super_gate(Calc(), self,
				itoa(self -> value[self -> state]));
		}
		break;
	case '+':
	case '-':
	case '*':
	case '/':
		cpu(self);
		if (self -> state)
			super_gate(Calc(), self, itoa(self -> value[0]));
		else
			self -> state = 1;
		self -> value[1] = 0;
		self -> op = text[0];
		break;
	case 'C':
		if (! self -> state)
			super_gate(Calc(), self, " ");
		else
			super_gate(Calc(), self, "0");
		self -> value[self -> state] = 0;
		break;
	case '=':
		cpu(self);
		super_gate(Calc(), self, itoa(self -> value[0]));
		self -> state = 0, self -> value[0] = 0;
	}
	return accept;
}

void wire (void * to, void * _self) {
	const struct IcClass * class = (const void *) classOf(_self);

	if (isOf(class, IcClass()) && class -> wire.method) {
	cast(Objct(), to);

		((void (*) ()) class -> wire.method)(to, _self);
	} else
		forward(_self, 0, (Method) wire, "wire", to, _self);
}

void super_wire (const void * _class, void * to, void * _self) {
	const struct IcClass * superclass = cast(IcClass(), super(_class));

	cast(Objct(), to);

	assert(superclass -> wire.method);
	((void (*) ()) superclass -> wire.method)(to, _self);
}

enum react gate (void * _self, const void * item) {
	enum react result;
	const struct IcClass * class = (const void *) classOf(_self);

	if (isOf(class, IcClass()) && class -> gate.method) {

		result = ((enum react (*) ()) class -> gate.method)(_self, item);
	} else
		forward(_self, & result, (Method) gate, "gate", _self, item);
	return result;
}

enum react super_gate (const void * _class, void * _self, const void * item) {
	const struct IcClass * superclass = cast(IcClass(), super(_class));

	assert(superclass -> gate.method);
	return ((enum react (*) ()) superclass -> gate.method)(_self, item);
}

static void * IcClass_ctor (void * _self, va_list * app) {
	struct IcClass * self = super_ctor(IcClass(), _self, app);
	Method selector;
#ifdef va_copy
	va_list ap; va_copy(ap, * app);
#else
	va_list ap = * app;
#endif

	while ((selector = va_arg(ap, Method)))
	{	const char * tag = va_arg(ap, const char *);
		Method method = va_arg(ap, Method);

		if (selector == (Method) wire)
		{	if (tag)
				self -> wire.tag = tag,
				self -> wire.selector = selector;
			self -> wire.method = method;
			continue;
		}
		if (selector == (Method) gate)
		{	if (tag)
				self -> gate.tag = tag,
				self -> gate.selector = selector;
			self -> gate.method = method;
			continue;
		}
	}
#ifdef va_copy
	va_end(ap);
#endif
	return self;
}

static const void * _IcClass;

const void * const IcClass (void) {
	return _IcClass ? _IcClass :
		(_IcClass = new(Class(),
			"IcClass", Class(), sizeof(struct IcClass),
			ctor, "", IcClass_ctor,
			(void *) 0));
}

static const void * _Ic;

const void * const Ic (void) {
	return _Ic ? _Ic :
		(_Ic = new(IcClass(),
			"Ic", Objct(), sizeof(struct Ic),
			puto, "puto", Ic_puto,
			wire, "wire", Ic_wire,
			gate, "gate", Ic_gate,
			(void *) 0));
}
static const void * _Mux;

const void * const Mux (void) {
	return _Mux ? _Mux :
		(_Mux = new(IcClass(),
			"Mux", Ic(), sizeof(struct Mux),
			ctor, "ctor", Mux_ctor,
			dtor, "dtor", Mux_dtor,
			puto, "puto", Mux_puto,
			wire, "wire", Mux_wire,
			gate, "gate", Mux_gate,
			(void *) 0));
}
static const void * _LineOut;

const void * const LineOut (void) {
	return _LineOut ? _LineOut :
		(_LineOut = new(IcClass(),
			"LineOut", Ic(), sizeof(struct LineOut),
			gate, "gate", LineOut_gate,
			(void *) 0));
}
static const void * _Button;

const void * const Button (void) {
	return _Button ? _Button :
		(_Button = new(IcClass(),
			"Button", Ic(), sizeof(struct Button),
			ctor, "ctor", Button_ctor,
			puto, "puto", Button_puto,
			gate, "gate", Button_gate,
			(void *) 0));
}
static const void * _Event;

const void * const Event (void) {
	return _Event ? _Event :
		(_Event = new(Class(),
			"Event", Objct(), sizeof(struct Event),
			ctor, "ctor", Event_ctor,
			puto, "puto", Event_puto,
			(void *) 0));
}
static const void * _Calc;

const void * const Calc (void) {
	return _Calc ? _Calc :
		(_Calc = new(IcClass(),
			"Calc", Ic(), sizeof(struct Calc),
			puto, "puto", Calc_puto,
			gate, "gate", Calc_gate,
			(void *) 0));
}
