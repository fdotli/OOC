# include "Object.h"
# include "Object.r"

#include <signal.h>
#include <stdlib.h>
#include <string.h>

static struct Object * Object_new (const void * _self, va_list * app) {
	const struct Class * self = cast(Class(), _self);

	return ctor(allocate(self), app);
}

static void * Object_ctor (void * _self, va_list * app) {
	struct Object * self = cast(Object(), _self);

	return self;
}

static void * Object_dtor (void * _self) {
	struct Object * self = cast(Object(), _self);

	return self;
}

static int Object_puto (const void * _self, FILE * fp) {
	const struct Class * class;
	const struct Object * self = cast(Object(), _self);

	class = classOf(self);
	return fprintf(fp, "%s at %p\n", class -> name, self);
}

static void Object_delete (void * _self) {
	struct Object * self = cast(Object(), _self);

	free(dtor(self));
}

static void * Object_geto (void * _self, FILE * fp) {
	void * dummy;
	struct Object * self = cast(Object(), _self);

	if (fscanf(fp, " at %p\n", & dummy) != 1)
		assert(0);
	return self;
}

const struct Class * classOf (const void * _self) {
	const struct Object * self = cast(Object(), _self);

	return self -> class;
}

size_t sizeOf (const void * _self) {
	const struct Class * class = classOf(_self);

	return class -> size;
}

int isA (const void * _self, const void * class) {
	if (_self) {
	const struct Object * self = cast(Object(), _self);

	cast(Class(), class);

		return classOf(self) == class;
	}
	return 0;
}

int isOf (const void * _self, const void * class) {
	if (_self)
	{	const struct Class * myClass;
	const struct Object * self = cast(Object(), _self);

	cast(Class(), class);

		myClass = classOf(self);
		if (class != Object()) {
			while (myClass != class)
				if (myClass != Object())
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

#define	isObject(p) ( assert(p), assert(((struct Object *) p) -> magic == MAGIC), p )

void * cast (const void * class, const void * _self) {
	void (* sigsegv)(int) = signal(SIGSEGV, catch);
#ifdef	SIGBUS
	void (* sigbus)(int) = signal(SIGBUS, catch);
#endif
	const struct Object * self = isObject(_self);
	const struct Class * myClass = isObject(self -> class);

	if (class != Object())
	{	isObject(class);
		while (myClass != class)
		{	assert(myClass != Object());
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

struct classList { const char * name; const void * class; };

static int cmp (const struct classList * a, const struct classList * b)
{
	return strcmp(a -> name, b -> name);
}

void * retrieve (FILE * fp)
{	char buf [BUFSIZ];
	extern const void * (* classes[]) (void);
	static struct classList * cL;
	static int cD = -1;

	if (cD < 0)
	{	for (cD = 0; classes[cD]; ++ cD)
			;
		if (cD > 0)
		{	cL = malloc(cD * sizeof(struct classList));
			assert(cL);
			for (cD = 0; classes[cD]; ++ cD)
				cL[cD].class = classes[cD](),
				cL[cD].name = nameOf(cL[cD].class);
		}
	}

	if (! cD)
		fputs("no classes known\n", stderr);

	else if (fp && ! feof(fp) && fscanf(fp, "%s", buf) == 1)
	{	struct classList key, * p;

		key.name = buf;
		if ((p = bsearch(& key, cL, cD, sizeof key,
			  (int (*) (const void *, const void *)) cmp)))
			return geto(allocate(p -> class), fp);
		fprintf(stderr, "%s: cannot retrieve\n", buf);
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

static void * Class_geto (void * _self, FILE * fp) {
	assert(0);
	return 0;
}

struct Object * allocate (const void * _self) {
	struct Object * object;
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

static const struct Class _Object;
static const struct Class _Class;

void * ctor (void * _self, va_list * app) {
	void * result;
	const struct Class * class = classOf(_self);

	assert(class -> ctor.method);

	result = ((void * (*) ()) class -> ctor.method)(_self, app);
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

	assert(class -> dtor.method);

	result = ((void * (*) ()) class -> dtor.method)(_self);
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

	assert(class -> puto.method);

	result = ((int (*) ()) class -> puto.method)(_self, fp);
	return result;
}

int super_puto (const void * _class, const void * _self, FILE * fp) {
	const struct Class * superclass = super(_class);

	assert(superclass -> puto.method);
	return ((int (*) ()) superclass -> puto.method)(_self, fp);
}

void delete (void * _self) {
	const struct Class * class = classOf(_self);

	assert(class -> delete.method);

	((void (*) ()) class -> delete.method)(_self);
}

void super_delete (const void * _class, void * _self) {
	const struct Class * superclass = super(_class);

	assert(superclass -> delete.method);
	((void (*) ()) superclass -> delete.method)(_self);
}

void * geto (void * _self, FILE * fp) {
	void * result;
	const struct Class * class = classOf(_self);

	assert(class -> geto.method);

	result = ((void * (*) ()) class -> geto.method)(_self, fp);
	return result;
}

void * super_geto (const void * _class, void * _self, FILE * fp) {
	const struct Class * superclass = super(_class);

	assert(superclass -> geto.method);
	return ((void * (*) ()) superclass -> geto.method)(_self, fp);
}

struct Object * new (const void * _self, ...) {
	struct Object * result;
	va_list ap;
	const struct Class * class = cast(Class(), _self);

	va_start(ap, _self);
	assert(class -> new.method);

	result = ((struct Object * (*) ()) class -> new.method)(_self, & ap);
	va_end(ap);
	return result;
}

struct Object * super_new (const void * _class, const void * _self, va_list * app) {
	const struct Class * superclass = super(_class);

	assert(superclass -> new.method);
	return ((struct Object * (*) ()) superclass -> new.method)(_self, app);
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
		if (selector == (Method) geto)
		{	if (tag)
				self -> geto.tag = tag,
				self -> geto.selector = selector;
			self -> geto.method = method;
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

const void * const Object (void) {
	return & _Object;
}

const void * const Class (void) {
	return & _Class;
}

static const struct Class _Object = {
	{ MAGIC, & _Class },
	"Object", & _Object, sizeof(struct Object),
	{ "",		(Method) 0,		(Method) Object_ctor },
	{ "",		(Method) 0,		(Method) Object_dtor },
	{ "puto",	(Method) puto,	(Method) Object_puto },
	{ "delete",	(Method) delete,(Method) Object_delete },
	{ "",	(Method) geto,	(Method) Object_geto },
	{ "",		(Method) 0,		(Method) Object_new },
};

static const struct Class _Class = {
	{ MAGIC, & _Class },
	"Class", & _Object, sizeof(struct Class),
	{ "",		(Method) 0,		(Method) Class_ctor },
	{ "",		(Method) 0,		(Method) Class_dtor },
	{ "puto",	(Method) puto,	(Method) Object_puto },
	{ "delete",	(Method) delete,(Method) Class_delete },
	{ "",		(Method) 0,		(Method) Class_geto },
	{ "",		(Method) 0,		(Method) Object_new },
};
