# include "Symtab.h"
# include "Symtab.r"

#include <stdlib.h>
#include <string.h>
#include "binary.h"

#include "Symbol.h"

static void * Symtab_ctor (void * _self, va_list * app) {
	struct Symtab * self = super_ctor(Symtab(), _self, app);

	if (! (self -> dim = va_arg(* app, size_t)))
		self -> dim = 1;

	self -> buf = malloc(self -> dim * sizeof(void *));
	assert(self -> buf);
	return self;
}

static void Symtab_delete (void * _self) {
}

static int cmp (const void * _key, const void * _elt)
{	const char * const * key = _key;
	const void * const * elt = _elt;

	return strcmp(* key, name(* elt));
}

static void ** search (struct Symtab * self, const char ** np)
{
	if (self -> count >= self -> dim)
	{	self -> buf = realloc(self -> buf,
						(self -> dim *= 2) * sizeof(void *));
		assert(self -> buf);
	}
	return binary(np, self -> buf, & self -> count,
										sizeof(void *), cmp);
}

void install (void * _self, const void * entry) {
	const char * nm;
	void ** pp;
	struct Symtab * self = cast(Symtab(), _self);

	cast(Symbol(), entry);

	nm = name(entry);
	pp = search(self, & nm);
	if (* pp != nm)
		delete(* pp);
	* pp = (void *) entry;
}

struct Symbol * screen (void * _self, const char * name, int lex) {
	void ** pp;
	struct Symtab * self = cast(Symtab(), _self);

	pp = search(self, & name);
	if (* pp == name)
	{	char * copy = malloc(strlen(name) + 1);

		assert(copy);
		* pp = new(Symbol(), strcpy(copy, name), lex);
	}
	return * pp;
}

static const void * _Symtab;

const void * const Symtab (void) {
	return _Symtab ? _Symtab :
		(_Symtab = new(Class(),
			"Symtab", Object(), sizeof(struct Symtab),
			ctor, "ctor", Symtab_ctor,
			delete, "", Symtab_delete,
			(void *) 0));
}
