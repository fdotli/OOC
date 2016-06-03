# include "List.h"
# include "List.r"

#include <stdlib.h>
#include <string.h>

#ifndef	MIN
#define	MIN	32
#endif

static void * List_ctor (void * _self, va_list * app) {
	struct List * self = super_ctor(List(), _self, app);

	if (! (self -> dim = va_arg(* app, unsigned)))
		self -> dim = MIN;
	self -> buf = malloc(self -> dim * sizeof * self -> buf);
	assert(self -> buf);
	return self;
}

static void * List_dtor (void * _self) {
	struct List * self = cast(List(), _self);

	free(self -> buf), self -> buf = 0;
	return super_dtor(List(), self);
}

static void * add1 (struct List * self, const void * element)
{
	self -> end = self -> count = 1;
	return (void *) (self -> buf[self -> begin = 0] = element);
}

static void extend (struct List * self)
{
	if (self -> count >= self -> dim)
	{	self -> buf =
				realloc(self -> buf, 2 * self -> dim
						* sizeof * self -> buf);
		assert(self -> buf);
		if (self -> begin && self -> begin != self -> dim)
		{	memcpy(self -> buf + self -> dim + self -> begin,
				self -> buf + self -> begin,
				(self -> dim - self -> begin)
						* sizeof * self -> buf);
			self -> begin += self -> dim;
		}
		else
			self -> begin = 0;
		self -> dim *= 2;
	}
	++ self -> count;
}

struct Object * addFirst (void * _self, const void * element) {
	struct List * self = cast(List(), _self);

	cast(Object(), element);

	if (! self -> count)
		return add1(self, element);
	extend(self);
	if (self -> begin == 0)
		self -> begin = self -> dim;
	self -> buf[-- self -> begin] = element;
	return (void *) element;
}

struct Object * addLast (void * _self, const void * element) {
	struct List * self = cast(List(), _self);

	cast(Object(), element);

	if (! self -> count)
		return add1(self, element);
	extend(self);
	if (self -> end >= self -> dim)
		self -> end = 0;
	self -> buf[self -> end ++] = element;
	return (void *) element;
}

unsigned count (const void * _self) {
	const struct List * self = cast(List(), _self);

	return self -> count;
}

struct Object * lookAt (const void * _self, unsigned n) {
	const struct List * self = cast(List(), _self);

	return (void *) (n >= self -> count ? 0 :
		self -> buf[(self -> begin + n) % self -> dim]);
}

struct Object * takeFirst (void * _self) {
	struct List * self = cast(List(), _self);

	if (! self -> count)
		return 0;
	-- self -> count;
	if (self -> begin >= self -> dim)
		self -> begin = 0;
	return (void *) self -> buf[self -> begin ++];
}

struct Object * takeLast (void * _self) {
	struct List * self = cast(List(), _self);

	if (! self -> count)
		return 0;
	-- self -> count;
	if (self -> end == 0)
		self -> end = self -> dim;
	return (void *) self -> buf[-- self -> end];
}

struct Object * add (void * _self, const void * element) {
	struct Object * result;
	const struct ListClass * class = cast(ListClass(), classOf(_self));

	assert(class -> add.method);
	cast(Object(), element);

	result = ((struct Object * (*) ()) class -> add.method)(_self, element);
	return result;
}

struct Object * super_add (const void * _class, void * _self, const void * element) {
	const struct ListClass * superclass = cast(ListClass(), super(_class));

	cast(Object(), element);

	assert(superclass -> add.method);
	return ((struct Object * (*) ()) superclass -> add.method)(_self, element);
}

struct Object * take (void * _self) {
	struct Object * result;
	const struct ListClass * class = cast(ListClass(), classOf(_self));

	assert(class -> take.method);

	result = ((struct Object * (*) ()) class -> take.method)(_self);
	return result;
}

struct Object * super_take (const void * _class, void * _self) {
	const struct ListClass * superclass = cast(ListClass(), super(_class));

	assert(superclass -> take.method);
	return ((struct Object * (*) ()) superclass -> take.method)(_self);
}

static void * ListClass_ctor (void * _self, va_list * app) {
	struct ListClass * self = super_ctor(ListClass(), _self, app);
	Method selector;
#ifdef va_copy
	va_list ap; va_copy(ap, * app);
#else
	va_list ap = * app;
#endif

	while ((selector = va_arg(ap, Method)))
	{	const char * tag = va_arg(ap, const char *);
		Method method = va_arg(ap, Method);

		if (selector == (Method) add)
		{	if (tag)
				self -> add.tag = tag,
				self -> add.selector = selector;
			self -> add.method = method;
			continue;
		}
		if (selector == (Method) take)
		{	if (tag)
				self -> take.tag = tag,
				self -> take.selector = selector;
			self -> take.method = method;
			continue;
		}
	}
#ifdef va_copy
	va_end(ap);
#endif
	return self;
}

static const void * _ListClass;

const void * const ListClass (void) {
	return _ListClass ? _ListClass :
		(_ListClass = new(Class(),
			"ListClass", Class(), sizeof(struct ListClass),
			ctor, "", ListClass_ctor,
			(void *) 0));
}

static const void * _List;

const void * const List (void) {
	return _List ? _List :
		(_List = new(ListClass(),
			"List", Object(), sizeof(struct List),
			ctor, "ctor", List_ctor,
			dtor, "dtor", List_dtor,
			(void *) 0));
}
