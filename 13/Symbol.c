# include "Symbol.h"
# include "Symbol.r"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"
#include "Node.h"

static void * Symbol_ctor (void * _self, va_list * app) {
	struct Symbol * self = super_ctor(Symbol(), _self, app);

	self -> name = va_arg(* app, const char *);
	self -> lex = va_arg(* app, int);
	return self;
}

static int Symbol_puto (const void * _self, FILE * fp) {
	int result;
	const struct Symbol * self = cast(Symbol(), _self);

	result = super_puto(Symbol(), _self, fp);
	return result + fprintf(fp, "\tname %s\n\tlex %d\n",
							self -> name, self -> lex);
}

static void * Symbol_geto (void * _self, FILE * fp) {
	struct Symbol * self = super_geto(Symbol(), _self, fp);
	char buf [BUFSIZ];

	if (fscanf(fp, "\tname %s\n\tlex %d\n",
							buf, & self -> lex) != 2)
		assert(0);
	self -> name = malloc(strlen(buf) + 1);
	assert(self -> name);
	strcpy((char *) self -> name, buf);
	return self;
}

const char * name (const void * _self) {
	const struct Symbol * self = cast(Symbol(), _self);

	return self -> name;
}

int lex (const void * _self) {
	const struct Symbol * self = cast(Symbol(), _self);

	return self -> lex;
}

static void Reserved_delete (void * _self) {
}

static int Var_puto (const void * _self, FILE * fp) {
	int result;
	const struct Var * self = cast(Var(), _self);

	result = super_puto(Var(), _self, fp);
	return result + fprintf(fp, "\tvalue %g\n", self -> value);
}

static void * Var_geto (void * _self, FILE * fp) {
	struct Var * self = super_geto(Var(), _self, fp);

	if (fscanf(fp, "\tvalue %lg\n", & self -> value) != 1)
		assert(0);
	return self;
}

static void Var_move (void * _self, void * _from) {
	struct Var * self = cast(Var(), _self);
	struct Var * from = cast(Var(), _from);

	setvalue(self, from -> value);
}

double value (const void * _self) {
	const struct Var * self = cast(Var(), _self);

	return self -> value;
}

double setvalue (void * _self, double value) {
	struct Var * self = cast(Var(), _self);

	return self -> value = value;
}

static void * Const_ctor (void * _self, va_list * app) {
	struct Const * self = super_ctor(Const(), _self, app);

	setvalue(self, va_arg(* app, double));
	return self;
}

static void Const_delete (void * _self) {
}

static void Const_move (void * _self, void * _from) {
}

static int Fun_puto (const void * _self, FILE * fp) {
	int result;
	const struct Fun * self = cast(Fun(), _self);

	result = super_puto(Fun(), _self, fp);
	if (self -> fun)
	{	result += fputs("=\n", fp);
		return result + puto(self -> fun, fp);
	}
	return result + fputs(".\n", fp);
}

static void * Fun_geto (void * _self, FILE * fp) {
	struct Fun * self = super_geto(Fun(), _self, fp);
	int ch = getc(fp);

	if (getc(fp) != '\n')
		assert(0);
	switch (ch) {
	case '=':
		cast(Node(), self -> fun = retrieve(fp));
	case '.':
		break;
	default:
		assert(0);
	}
	return self;
}

static void Fun_move (void * _self, void * _from) {
	struct Fun * self = cast(Fun(), _self);
	struct Fun * from = cast(Fun(), _from);

	setfun(self, from -> fun), from -> fun = 0;
}

void setfun (void * _self, void * fun) {
	struct Fun * self = cast(Fun(), _self);

	cast(Node(), fun);

	if (self -> fun)
		delete(self -> fun);
	self -> fun = fun;
}

double funvalue (void * _self, double value) {
	struct Fun * self = cast(Fun(), _self);

	if (! self -> fun)
		error("undefined function");
	setvalue(self, value);
	return exec(self -> fun);
}

static void * Math_ctor (void * _self, va_list * app) {
	struct Math * self = super_ctor(Math(), _self, app);

	self -> fun = va_arg(* app, function);
	return self;
}

static void Math_delete (void * _self) {
}

double mathvalue (const void * _self, double value) {
	double result;
	const struct Math * self = cast(Math(), _self);

	errno = 0;
	result = self -> fun(value);
	if (errno)
		error("error in %s: %s", name(self), strerror(errno));
	return result;
}

static const void * _Symbol;

const void * const Symbol (void) {
	return _Symbol ? _Symbol :
		(_Symbol = new(Class(),
			"Symbol", Object(), sizeof(struct Symbol),
			ctor, "ctor", Symbol_ctor,
			puto, "puto", Symbol_puto,
			geto, "", Symbol_geto,
			(void *) 0));
}
static const void * _Reserved;

const void * const Reserved (void) {
	return _Reserved ? _Reserved :
		(_Reserved = new(Class(),
			"Reserved", Symbol(), sizeof(struct Reserved),
			delete, "", Reserved_delete,
			(void *) 0));
}
void move (void * _self, void * _from) {
	const struct VarClass * class = cast(VarClass(), classOf(_self));

	assert(class -> move.method);

	((void (*) ()) class -> move.method)(_self, _from);
}

void super_move (const void * _class, void * _self, void * _from) {
	const struct VarClass * superclass = cast(VarClass(), super(_class));

	assert(superclass -> move.method);
	((void (*) ()) superclass -> move.method)(_self, _from);
}

static void * VarClass_ctor (void * _self, va_list * app) {
	struct VarClass * self = super_ctor(VarClass(), _self, app);
	Method selector;
#ifdef va_copy
	va_list ap; va_copy(ap, * app);
#else
	va_list ap = * app;
#endif

	while ((selector = va_arg(ap, Method)))
	{	const char * tag = va_arg(ap, const char *);
		Method method = va_arg(ap, Method);

		if (selector == (Method) move)
		{	if (tag)
				self -> move.tag = tag,
				self -> move.selector = selector;
			self -> move.method = method;
			continue;
		}
	}
#ifdef va_copy
	va_end(ap);
#endif
	return self;
}

static const void * _VarClass;

const void * const VarClass (void) {
	return _VarClass ? _VarClass :
		(_VarClass = new(Class(),
			"VarClass", Class(), sizeof(struct VarClass),
			ctor, "", VarClass_ctor,
			(void *) 0));
}

static const void * _Var;

const void * const Var (void) {
	return _Var ? _Var :
		(_Var = new(VarClass(),
			"Var", Symbol(), sizeof(struct Var),
			puto, "puto", Var_puto,
			geto, "", Var_geto,
			move, "move", Var_move,
			(void *) 0));
}
static const void * _Const;

const void * const Const (void) {
	return _Const ? _Const :
		(_Const = new(VarClass(),
			"Const", Var(), sizeof(struct Const),
			ctor, "ctor", Const_ctor,
			delete, "", Const_delete,
			move, "", Const_move,
			(void *) 0));
}
static const void * _Fun;

const void * const Fun (void) {
	return _Fun ? _Fun :
		(_Fun = new(VarClass(),
			"Fun", Var(), sizeof(struct Fun),
			puto, "puto", Fun_puto,
			geto, "", Fun_geto,
			move, "move", Fun_move,
			(void *) 0));
}
static const void * _Math;

const void * const Math (void) {
	return _Math ? _Math :
		(_Math = new(Class(),
			"Math", Symbol(), sizeof(struct Math),
			ctor, "ctor", Math_ctor,
			delete, "", Math_delete,
			(void *) 0));
}
