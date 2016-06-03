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
static const void * _Var;

const void * const Var (void) {
	return _Var ? _Var :
		(_Var = new(Class(),
			"Var", Symbol(), sizeof(struct Var),
			(void *) 0));
}
static const void * _Const;

const void * const Const (void) {
	return _Const ? _Const :
		(_Const = new(Class(),
			"Const", Var(), sizeof(struct Const),
			ctor, "ctor", Const_ctor,
			delete, "", Const_delete,
			(void *) 0));
}
static const void * _Fun;

const void * const Fun (void) {
	return _Fun ? _Fun :
		(_Fun = new(Class(),
			"Fun", Var(), sizeof(struct Fun),
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
