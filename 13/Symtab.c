# include "Symtab.h"
# include "Symtab.r"

#include <stdlib.h>
#include <string.h>
#include "binary.h"

#include "Symbol.h"
#include "Exception.h"

static void * Symtab_ctor (void * _self, va_list * app) {
	struct Symtab * self = super_ctor(Symtab(), _self, app);

	if (! (self -> dim = va_arg(* app, size_t)))
		self -> dim = 1;

	self -> buf = malloc(self -> dim * sizeof(void *));
	assert(self -> buf);
	return self;
}

static int Symtab_puto (const void * _self, FILE * fp) {
	return 0;
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

int save (const void * _self, const void * entry, const char * fnm) {
	const struct Symtab * self = cast(Symtab(), _self);
	FILE * fp;

	if (entry)
	{	if (! respondsTo(entry, "move"))
			return EOF;
		if (! (fp = fopen(fnm, "w")))
			return EOF;
		puto(entry, fp);
	}
	else
	{	int i;

		if (! (fp = fopen(fnm, "w")))
			return EOF;
		for (i = 0; i < self -> count; ++ i)
			if (respondsTo(self -> buf[i], "move"))
					puto(self -> buf[i], fp);
	}

	return fclose(fp);
}

int load (void * _self, void * entry, const char * fnm) {
	struct Symtab * self = cast(Symtab(), _self);
	const char * target = NULL;
	FILE * fp;
	int result = EOF;
	void * in;
	void * cleanup;

	if (entry) {
		if (isOf(entry, Symbol())
				|| respondsTo(entry, "move"))
			target = name(entry);
		else
			return EOF;
	}

	if (! (fp = fopen(fnm, "r")))
		return EOF;

	cleanup = new(Exception());
	if (catch(cleanup))
	{	fclose(fp);
		delete(cleanup);
		cause(1);
		assert(0);
	}

	while ((in = retrieve(fp)))
	{	const char * nm;
		void ** pp;

		if (! respondsTo(in, "move"))
			break;

		if (target && strcmp(name(in), target))
			continue;

		nm = name(in);
		pp = search(self, & nm);

		if (* pp == nm)
			* pp = in;

		else if (isA(* pp, Symbol()))
		{	nm = name(* pp), delete(* pp), free((void *) nm);
			* pp = in;
		}

		else if (! respondsTo(* pp, "move"))
		{	nm = name(in); delete(in); free((void *) nm);
			continue;
		}

		else
		{	move(* pp, in);
			delete(in), free((void *) nm);
		}

		if (target)
		{	result = 0;
			break;
		}
	}
	if (! target && feof(fp))
		result = 0;

	fclose(fp);
	delete(cleanup);
	return result;
}

static const void * _Symtab;

const void * const Symtab (void) {
	return _Symtab ? _Symtab :
		(_Symtab = new(Class(),
			"Symtab", Object(), sizeof(struct Symtab),
			ctor, "ctor", Symtab_ctor,
			puto, "", Symtab_puto,
			delete, "", Symtab_delete,
			(void *) 0));
}
