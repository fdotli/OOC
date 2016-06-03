# include "String.h"
# include "String.r"

#include <stdlib.h>
#include <string.h>

static void * String_ctor (void * _self, va_list * app) {
	struct String * self = super_ctor(String(), _self, app);
	const char * text = va_arg(* app, const char *);

	self -> text = malloc(strlen(text) + 1);
	assert(self -> text);
	strcpy((char *) self -> text, text);
	return self;
}

static void * String_dtor (void * _self) {
	struct String * self = cast(String(), _self);

	free((void *) self -> text), self -> text = 0;
	return super_dtor(String(), self);
}

static int String_puto (const void * _self, FILE * fp) {
	const struct String * self = cast(String(), _self);

	return fputs(self -> text, fp);
}

static int String_differ (const void * _self, const void * b) {
	struct String * self = cast(String(), _self);
	struct String * sp = cast(String(), b);

	return self == sp ? 0 : strcmp(self -> text, sp -> text);
}

static const void * _String;

const void * const String (void) {
	return _String ? _String :
		(_String = new(Class(),
			"String", Object(), sizeof(struct String),
			ctor, "ctor", String_ctor,
			dtor, "dtor", String_dtor,
			puto, "puto", String_puto,
			differ, "differ", String_differ,
			(void *) 0));
}
