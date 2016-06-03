# include "Object.h"
# include "Object.r"

#include <signal.h>
#include <stdlib.h>
#include <string.h>

static void * Object_ctor (void * _self, va_list * app) {
	struct Object * self = cast(Object(), _self);

	return self;
}

static void * Object_dtor (void * _self) {
	struct Object * self = cast(Object(), _self);

	return self;
}

static int Object_differ (const void * _self, const void * b) {
	const struct Object * self = cast(Object(), _self);

	return self != b;
}

static int Object_puto (const void * _self, FILE * fp) {
	const struct Class * class;
	const struct Object * self = cast(Object(), _self);

	class = classOf(self);
	return fprintf(fp, "%s at %p\n", class -> name, self);
}

void delete (void * _self) {
	if (_self)
		free(dtor(_self));
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

static void * Class_dtor (void * _self) {
	struct Class * self = cast(Class(), _self);

	fprintf(stderr, "%s: cannot destroy class\n", self -> name);
	return 0;
}

struct Object * new (const void * _self, ...) {
	struct Object * object;
	va_list ap;
	const struct Class * self = cast(Class(), _self);

	assert(self -> size);
	object = calloc(1, self -> size);
	assert(object);
	object -> magic = MAGIC;
	object -> class = self;
	va_start(ap, _self);
	object = ctor(object, & ap);
	va_end(ap);
	return object;
}

const struct Class * super (const void * _self) {
	const struct Class * self = cast(Class(), _self);

	return self -> super;
}

static const struct Class _Object;
static const struct Class _Class;

void * ctor (void * _self, va_list * app) {
	void * result;
	const struct Class * class = classOf(_self);

	assert(class -> ctor);

	result = class -> ctor(_self, app);
	return result;
}

void * super_ctor (const void * _class, void * _self, va_list * app) {
	const struct Class * superclass = super(_class);

	assert(superclass -> ctor);
	return superclass -> ctor(_self, app);
}

void * dtor (void * _self) {
	void * result;
	const struct Class * class = classOf(_self);

	assert(class -> dtor);

	result = class -> dtor(_self);
	return result;
}

void * super_dtor (const void * _class, void * _self) {
	const struct Class * superclass = super(_class);

	assert(superclass -> dtor);
	return superclass -> dtor(_self);
}

int differ (const void * _self, const void * b) {
	int result;
	const struct Class * class = classOf(_self);

	assert(class -> differ);
	cast(Object(), b);

	result = class -> differ(_self, b);
	return result;
}

int super_differ (const void * _class, const void * _self, const void * b) {
	const struct Class * superclass = super(_class);

	cast(Object(), b);

	assert(superclass -> differ);
	return superclass -> differ(_self, b);
}

int puto (const void * _self, FILE * fp) {
	int result;
	const struct Class * class = classOf(_self);

	assert(class -> puto);

	result = class -> puto(_self, fp);
	return result;
}

int super_puto (const void * _class, const void * _self, FILE * fp) {
	const struct Class * superclass = super(_class);

	assert(superclass -> puto);
	return superclass -> puto(_self, fp);
}

static void * Class_ctor (void * _self, va_list * app) {
	struct Class * self = _self;
	const size_t offset = offsetof(struct Class, ctor);
	typedef void (* voidf) ();
	voidf selector;
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
	while ((selector = va_arg(ap, voidf)))
	{	voidf method = va_arg(ap, voidf);

		if (selector == (voidf) ctor)
		{	* (voidf *) & self -> ctor = method;
			continue;
		}
		if (selector == (voidf) dtor)
		{	* (voidf *) & self -> dtor = method;
			continue;
		}
		if (selector == (voidf) differ)
		{	* (voidf *) & self -> differ = method;
			continue;
		}
		if (selector == (voidf) puto)
		{	* (voidf *) & self -> puto = method;
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
	Object_ctor, Object_dtor, Object_differ, Object_puto
};

static const struct Class _Class = {
	{ MAGIC, & _Class },
	"Class", & _Object, sizeof(struct Class),
	Class_ctor, Class_dtor, Object_differ, Object_puto
};
