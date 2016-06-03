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

static int List_puto (const void * _self, FILE * fp) {
	int result;
	unsigned i;
	const struct List * self = cast(List(), _self);

	result = super_puto(List(), self, fp);
	result += fprintf(fp, "\tdim %u, count %u {\n",
							self -> dim, self -> count);
	for (i = 0; i < self -> count; ++ i)
		putc(' ', fp),
		result += 1 + puto(self -> buf[(self -> begin + i)
									% self -> dim], fp);
	return result + fputs("}\n", fp);
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

struct Objct * addFirst (void * _self, const void * element) {
	struct List * self = cast(List(), _self);

	cast(Objct(), element);

	if (! self -> count)
		return add1(self, element);
	extend(self);
	if (self -> begin == 0)
		self -> begin = self -> dim;
	self -> buf[-- self -> begin] = element;
	return (void *) element;
}

struct Objct * addLast (void * _self, const void * element) {
	struct List * self = cast(List(), _self);

	cast(Objct(), element);

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

struct Objct * lookAt (const void * _self, unsigned n) {
	const struct List * self = cast(List(), _self);

	return (void *) (n >= self -> count ? 0 :
		self -> buf[(self -> begin + n) % self -> dim]);
}

struct Objct * takeFirst (void * _self) {
	struct List * self = cast(List(), _self);

	if (! self -> count)
		return 0;
	-- self -> count;
	if (self -> begin >= self -> dim)
		self -> begin = 0;
	return (void *) self -> buf[self -> begin ++];
}

struct Objct * takeLast (void * _self) {
	struct List * self = cast(List(), _self);

	if (! self -> count)
		return 0;
	-- self -> count;
	if (self -> end == 0)
		self -> end = self -> dim;
	return (void *) self -> buf[-- self -> end];
}

struct Objct * add (void * _self, const void * element) {
	struct Objct * result;
	const struct ListClass * class = (const void *) classOf(_self);

	if (isOf(class, ListClass()) && class -> add.method) {
	cast(Objct(), element);

		result = ((struct Objct * (*) ()) class -> add.method)(_self, element);
	} else
		forward(_self, & result, (Method) add, "add", _self, element);
	return result;
}

struct Objct * super_add (const void * _class, void * _self, const void * element) {
	const struct ListClass * superclass = cast(ListClass(), super(_class));

	cast(Objct(), element);

	assert(superclass -> add.method);
	return ((struct Objct * (*) ()) superclass -> add.method)(_self, element);
}

struct Objct * take (void * _self) {
	struct Objct * result;
	const struct ListClass * class = (const void *) classOf(_self);

	if (isOf(class, ListClass()) && class -> take.method) {

		result = ((struct Objct * (*) ()) class -> take.method)(_self);
	} else
		forward(_self, & result, (Method) take, "take", _self);
	return result;
}

struct Objct * super_take (const void * _class, void * _self) {
	const struct ListClass * superclass = cast(ListClass(), super(_class));

	assert(superclass -> take.method);
	return ((struct Objct * (*) ()) superclass -> take.method)(_self);
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
			"List", Objct(), sizeof(struct List),
			ctor, "ctor", List_ctor,
			dtor, "dtor", List_dtor,
			puto, "puto", List_puto,
			(void *) 0));
}
