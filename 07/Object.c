# include "Object.h"
# include "Object.r"

#include <stdlib.h>
#include <string.h>

static void * Object_ctor (void * _self, va_list * app) {
	return _self;
}

static void * Object_dtor (void * _self) {
	return _self;
}

static int Object_differ (const void * _self, const void * b) {
	return _self != b;
}

static int Object_puto (const void * _self, FILE * fp) {
	const struct Class * class = classOf(_self);

	return fprintf(fp, "%s at %p\n", class -> name, _self);
}

void delete (void * _self) {
	if (_self)
		free(dtor(_self));
}

const void * classOf (const void * _self) {
	const struct Object * self = _self;

	assert(self);

	assert(self -> class);
	return self -> class;
}

size_t sizeOf (const void * _self) {
	const struct Class * class = classOf(_self);

	return class -> size;
}

static void * Class_dtor (void * _self) {
	struct Class * self = _self;

	fprintf(stderr, "%s: cannot destroy class\n", self -> name);
	return 0;
}

struct Object * new (const void * _self, ...) {
	struct Object * object;
	va_list ap;
	const struct Class * self = _self;

	assert(self);

	assert(self -> size);
	object = calloc(1, self -> size);
	assert(object);
	object -> class = self;
	va_start(ap, _self);
	object = ctor(object, & ap);
	va_end(ap);
	return object;
}

const void * super (const void * _self) {
	const struct Class * self = _self;

	assert(self);

	assert(self -> super);
	return self -> super;
}

static const struct Class _Object;
static const struct Class _Class;

void * ctor (void * _self, va_list * app) {
	void * result;
	const struct Class * class = classOf(_self);
	struct Object * self = _self;

	assert(class -> ctor);
	assert(self);

	result = class -> ctor(_self, app);
	return result;
}

void * super_ctor (const void * _class, void * _self, va_list * app) {
	const struct Class * superclass = super(_class);
	struct Object * self = _self;

	assert(self);

	assert(superclass -> ctor);
	return superclass -> ctor(_self, app);
}

void * dtor (void * _self) {
	void * result;
	const struct Class * class = classOf(_self);
	struct Object * self = _self;

	assert(class -> dtor);
	assert(self);

	result = class -> dtor(_self);
	return result;
}

void * super_dtor (const void * _class, void * _self) {
	const struct Class * superclass = super(_class);
	struct Object * self = _self;

	assert(self);

	assert(superclass -> dtor);
	return superclass -> dtor(_self);
}

int differ (const void * _self, const void * b) {
	int result;
	const struct Class * class = classOf(_self);
	const struct Object * self = _self;

	assert(class -> differ);
	assert(self);
	assert(b);

	result = class -> differ(_self, b);
	return result;
}

int super_differ (const void * _class, const void * _self, const void * b) {
	const struct Class * superclass = super(_class);
	const struct Object * self = _self;

	assert(self);
	assert(b);

	assert(superclass -> differ);
	return superclass -> differ(_self, b);
}

int puto (const void * _self, FILE * fp) {
	int result;
	const struct Class * class = classOf(_self);
	const struct Object * self = _self;

	assert(class -> puto);
	assert(self);

	result = class -> puto(_self, fp);
	return result;
}

int super_puto (const void * _class, const void * _self, FILE * fp) {
	const struct Class * superclass = super(_class);
	const struct Object * self = _self;

	assert(self);

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
	self -> super = va_arg(* app, void *);
	self -> size = va_arg(* app, size_t);

	assert(self -> super);

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

static const struct Class _Object = {
	{ & _Class },
	"Object", & _Object, sizeof(struct Object),
	Object_ctor, Object_dtor, Object_differ, Object_puto
};

static const struct Class _Class = {
	{ & _Class },
	"Class", & _Object, sizeof(struct Class),
	Class_ctor, Class_dtor, Object_differ, Object_puto
};

const void * Object = & _Object;
const void * Class = & _Class;
