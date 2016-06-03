# include "Objct.h"
# include "Objct.r"

#include <signal.h>
#include <stdlib.h>
#include <string.h>

static struct Objct * Objct_new (const void * _self, va_list * app) {
	const struct Class * self = cast(Class(), _self);

	return ctor(allocate(self), app);
}

static void * Objct_ctor (void * _self, va_list * app) {
	struct Objct * self = cast(Objct(), _self);

	return self;
}

static void * Objct_dtor (void * _self) {
	struct Objct * self = cast(Objct(), _self);

	return self;
}

static int Objct_puto (const void * _self, FILE * fp) {
	const struct Class * class;
	const struct Objct * self = cast(Objct(), _self);

	class = classOf(self);
	return fprintf(fp, "%s at %p\n", class -> name, self);
}

static void Objct_delete (void * _self) {
	struct Objct * self = cast(Objct(), _self);

	free(dtor(self));
}

static void Objct_forward (const void * _self, void * result, Method selector, const char * name, va_list * app) {
	const struct Objct * self = cast(Objct(), _self);

	fprintf(stderr, "%s at %p does not answer %s\n",
						nameOf(classOf(self)), self, name);
	assert(0);
}
const struct Class * classOf (const void * _self) {
	const struct Objct * self = cast(Objct(), _self);

	return self -> class;
}

size_t sizeOf (const void * _self) {
	const struct Class * class = classOf(_self);

	return class -> size;
}

int isA (const void * _self, const void * class) {
	if (_self) {
	const struct Objct * self = cast(Objct(), _self);

	cast(Class(), class);

		return classOf(self) == class;
	}
	return 0;
}

int isOf (const void * _self, const void * class) {
	if (_self)
	{	const struct Class * myClass;
	const struct Objct * self = cast(Objct(), _self);

	cast(Class(), class);

		myClass = classOf(self);
		if (class != Objct()) {
			while (myClass != class)
				if (myClass != Objct())
					myClass = super(myClass);
				else
					return 0;
		}
		return 1;
	}
	return 0;
}

static void catch (int sig)
{
	assert(sig == 0);
}

#define	isObjct(p) ( assert(p), assert(((struct Objct *) p) -> magic == MAGIC), p )

void * cast (const void * class, const void * _self) {
	void (* sigsegv)(int) = signal(SIGSEGV, catch);
#ifdef	SIGBUS
	void (* sigbus)(int) = signal(SIGBUS, catch);
#endif
	const struct Objct * self = isObjct(_self);
	const struct Class * myClass = isObjct(self -> class);

	if (class != Objct())
	{	isObjct(class);
		while (myClass != class)
		{	assert(myClass != Objct());
			myClass = myClass -> super;
		}
	}

#ifdef	SIGBUS
	signal(SIGBUS, sigbus);
#endif
	signal(SIGSEGV, sigsegv);
	return (void *) self;
}

Method respondsTo (const void * _self, const char * tag) {
	if (tag && * tag) {
		const struct Class * class = classOf(_self);
		const struct Method * p = & class -> ctor;
		int nmeth =
			(sizeOf(class) - offsetof(struct Class, ctor))
			/ sizeof(struct Method);

		do
			if (p -> tag && strcmp(p -> tag, tag) == 0)
				return p -> method ? p -> selector : 0;
		while (++ p, -- nmeth);
	}
	return 0;
}

static void * Class_dtor (void * _self) {
	assert(0);
	return 0;
}

static void Class_delete (void * _self) {
	struct Class * self = cast(Class(), _self);

	fprintf(stderr, "%s: cannot delete class\n", self -> name);
}

struct Objct * allocate (const void * _self) {
	struct Objct * object;
	const struct Class * self = cast(Class(), _self);

	assert(self -> size);
	object = calloc(1, self -> size);
	assert(object);
	object -> magic = MAGIC;
	object -> class = self;
	return object;
}

const struct Class * super (const void * _self) {
	const struct Class * self = cast(Class(), _self);

	return self -> super;
}

const char * nameOf (const void * _self) {
	const struct Class * self = cast(Class(), _self);

	return self -> name;
}

static const struct Class _Objct;
static const struct Class _Class;

void * ctor (void * _self, va_list * app) {
	void * result;
	const struct Class * class = classOf(_self);

	if (class -> ctor.method) {

		result = ((void * (*) ()) class -> ctor.method)(_self, app);
	} else
		forward(_self, & result, (Method) ctor, "ctor", _self, app);
	return result;
}

void * super_ctor (const void * _class, void * _self, va_list * app) {
	const struct Class * superclass = super(_class);

	assert(superclass -> ctor.method);
	return ((void * (*) ()) superclass -> ctor.method)(_self, app);
}

void * dtor (void * _self) {
	void * result;
	const struct Class * class = classOf(_self);

	if (class -> dtor.method) {

		result = ((void * (*) ()) class -> dtor.method)(_self);
	} else
		forward(_self, & result, (Method) dtor, "dtor", _self);
	return result;
}

void * super_dtor (const void * _class, void * _self) {
	const struct Class * superclass = super(_class);

	assert(superclass -> dtor.method);
	return ((void * (*) ()) superclass -> dtor.method)(_self);
}

int puto (const void * _self, FILE * fp) {
	int result;
	const struct Class * class = classOf(_self);

	if (class -> puto.method) {

		result = ((int (*) ()) class -> puto.method)(_self, fp);
	} else
		forward(_self, & result, (Method) puto, "puto", _self, fp);
	return result;
}

int super_puto (const void * _class, const void * _self, FILE * fp) {
	const struct Class * superclass = super(_class);

	assert(superclass -> puto.method);
	return ((int (*) ()) superclass -> puto.method)(_self, fp);
}

void delete (void * _self) {
	const struct Class * class = classOf(_self);

	if (class -> delete.method) {

		((void (*) ()) class -> delete.method)(_self);
	} else
		forward(_self, 0, (Method) delete, "delete", _self);
}

void super_delete (const void * _class, void * _self) {
	const struct Class * superclass = super(_class);

	assert(superclass -> delete.method);
	((void (*) ()) superclass -> delete.method)(_self);
}

void forward (const void * _self, void * result, Method selector, const char * name, ...) {
	va_list ap;
	const struct Class * class = classOf(_self);

	va_start(ap, name);
	if (class -> forward.method) {

		((void (*) ()) class -> forward.method)(_self, result, selector, name, & ap);
	} else
		assert(0);
	va_end(ap);
}

void super_forward (const void * _class, const void * _self, void * result, Method selector, const char * name, va_list * app) {
	const struct Class * superclass = super(_class);

	assert(superclass -> forward.method);
	((void (*) ()) superclass -> forward.method)(_self, result, selector, name, app);
}

struct Objct * new (const void * _self, ...) {
	struct Objct * result;
	va_list ap;
	const struct Class * class = cast(Class(), _self);

	va_start(ap, _self);
	if (class -> new.method) {

		result = ((struct Objct * (*) ()) class -> new.method)(_self, & ap);
	} else
		forward(_self, & result, (Method) new, "new", _self, & ap);
	va_end(ap);
	return result;
}

struct Objct * super_new (const void * _class, const void * _self, va_list * app) {
	const struct Class * superclass = super(_class);

	assert(superclass -> new.method);
	return ((struct Objct * (*) ()) superclass -> new.method)(_self, app);
}

static void * Class_ctor (void * _self, va_list * app) {
	struct Class * self = _self;
	const size_t offset = offsetof(struct Class, ctor);
	Method selector;
	va_list ap;

	self -> name = va_arg(* app, char *);
	self -> super = cast(Class(), va_arg(* app, void *));
	self -> size = va_arg(* app, size_t);

	memcpy((char *) self + offset, (char *) self -> super + offset,
		sizeOf(self -> super) - offset);

#ifdef va_copy
	va_copy(ap, * app);
#else
	ap = * app;
#endif
	while ((selector = va_arg(ap, Method)))
	{	const char * tag = va_arg(ap, const char *);
		Method method = va_arg(ap, Method);

		if (selector == (Method) ctor)
		{	if (tag)
				self -> ctor.tag = tag,
				self -> ctor.selector = selector;
			self -> ctor.method = method;
			continue;
		}
		if (selector == (Method) dtor)
		{	if (tag)
				self -> dtor.tag = tag,
				self -> dtor.selector = selector;
			self -> dtor.method = method;
			continue;
		}
		if (selector == (Method) puto)
		{	if (tag)
				self -> puto.tag = tag,
				self -> puto.selector = selector;
			self -> puto.method = method;
			continue;
		}
		if (selector == (Method) delete)
		{	if (tag)
				self -> delete.tag = tag,
				self -> delete.selector = selector;
			self -> delete.method = method;
			continue;
		}
		if (selector == (Method) forward)
		{	if (tag)
				self -> forward.tag = tag,
				self -> forward.selector = selector;
			self -> forward.method = method;
			continue;
		}
		if (selector == (Method) new)
		{	if (tag)
				self -> new.tag = tag,
				self -> new.selector = selector;
			self -> new.method = method;
			continue;
		}
	}
#ifdef va_copy
	va_end(ap);
#endif
	return self;
}

const void * const Objct (void) {
	return & _Objct;
}

const void * const Class (void) {
	return & _Class;
}

static const struct Class _Objct = {
	{ MAGIC, & _Class },
	"Objct", & _Objct, sizeof(struct Objct),
	{ "",		(Method) 0,		(Method) Objct_ctor },
	{ "",		(Method) 0,		(Method) Objct_dtor },
	{ "puto",	(Method) puto,	(Method) Objct_puto },
	{ "delete",	(Method) delete,(Method) Objct_delete },
	{ "forward",(Method) forward,(Method) Objct_forward },
	{ "",		(Method) 0,		(Method) Objct_new },
};

static const struct Class _Class = {
	{ MAGIC, & _Class },
	"Class", & _Objct, sizeof(struct Class),
	{ "",		(Method) 0,		(Method) Class_ctor },
	{ "",		(Method) 0,		(Method) Class_dtor },
	{ "puto",	(Method) puto,	(Method) Objct_puto },
	{ "delete",	(Method) delete,(Method) Class_delete },
	{ "forward",(Method) forward,(Method) Objct_forward },
	{ "",		(Method) 0,		(Method) Objct_new },
};
