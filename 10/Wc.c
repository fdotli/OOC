# include "Wc.h"
# include "Wc.r"

#include <string.h>

#include "Filter.h"

static int Wc_wc (void * _self, const void * filter, const char * fnm, char * buf) {
	struct Wc * self = cast(Wc(), _self);

	assert((lineM) wc == wc);

	++ self -> lines;
	self -> chars += strlen(buf);
	return 0;
}

static int Wc_printFile (void * _self, const void * filter, const char * fnm) {
	struct Wc * self = cast(Wc(), _self);

	assert((wrapM) printFile == printFile);

	if (fnm && strcmp(fnm, "-"))
		printf("%7u %7u %s\n",
					self -> lines, self -> chars, fnm);
	else
		printf("%7u %7u\n", self -> lines, self -> chars);

	self -> allLines += self -> lines, self -> lines = 0;
	self -> allChars += self -> chars, self -> chars = 0;
	++ self -> files;
	return 0;
}

static int Wc_printTotal (void * _self, const void * filter) {
	struct Wc * self = cast(Wc(), _self);

	assert((quitM) printTotal == printTotal);

	if (self -> files > 1)
		printf("%7u %7u in %u files\n",
			self -> allLines, self -> allChars, self -> files);
	return 0;
}

int wc (void * _self, const void * filter, const char * fnm, char * buf) {
	int result;
	const struct WcClass * class = cast(WcClass(), classOf(_self));

	assert(class -> wc.method);
	cast(Object(), filter);

	result = ((int (*) ()) class -> wc.method)(_self, filter, fnm, buf);
	return result;
}

int super_wc (const void * _class, void * _self, const void * filter, const char * fnm, char * buf) {
	const struct WcClass * superclass = cast(WcClass(), super(_class));

	cast(Object(), filter);

	assert(superclass -> wc.method);
	return ((int (*) ()) superclass -> wc.method)(_self, filter, fnm, buf);
}

int printFile (void * _self, const void * filter, const char * fnm) {
	int result;
	const struct WcClass * class = cast(WcClass(), classOf(_self));

	assert(class -> printFile.method);
	cast(Object(), filter);

	result = ((int (*) ()) class -> printFile.method)(_self, filter, fnm);
	return result;
}

int super_printFile (const void * _class, void * _self, const void * filter, const char * fnm) {
	const struct WcClass * superclass = cast(WcClass(), super(_class));

	cast(Object(), filter);

	assert(superclass -> printFile.method);
	return ((int (*) ()) superclass -> printFile.method)(_self, filter, fnm);
}

int printTotal (void * _self, const void * filter) {
	int result;
	const struct WcClass * class = cast(WcClass(), classOf(_self));

	assert(class -> printTotal.method);
	cast(Object(), filter);

	result = ((int (*) ()) class -> printTotal.method)(_self, filter);
	return result;
}

int super_printTotal (const void * _class, void * _self, const void * filter) {
	const struct WcClass * superclass = cast(WcClass(), super(_class));

	cast(Object(), filter);

	assert(superclass -> printTotal.method);
	return ((int (*) ()) superclass -> printTotal.method)(_self, filter);
}

static void * WcClass_ctor (void * _self, va_list * app) {
	struct WcClass * self = super_ctor(WcClass(), _self, app);
	Method selector;
#ifdef va_copy
	va_list ap; va_copy(ap, * app);
#else
	va_list ap = * app;
#endif

	while ((selector = va_arg(ap, Method)))
	{	const char * tag = va_arg(ap, const char *);
		Method method = va_arg(ap, Method);

		if (selector == (Method) wc)
		{	if (tag)
				self -> wc.tag = tag,
				self -> wc.selector = selector;
			self -> wc.method = method;
			continue;
		}
		if (selector == (Method) printFile)
		{	if (tag)
				self -> printFile.tag = tag,
				self -> printFile.selector = selector;
			self -> printFile.method = method;
			continue;
		}
		if (selector == (Method) printTotal)
		{	if (tag)
				self -> printTotal.tag = tag,
				self -> printTotal.selector = selector;
			self -> printTotal.method = method;
			continue;
		}
	}
#ifdef va_copy
	va_end(ap);
#endif
	return self;
}

static const void * _WcClass;

const void * const WcClass (void) {
	return _WcClass ? _WcClass :
		(_WcClass = new(Class(),
			"WcClass", Class(), sizeof(struct WcClass),
			ctor, "", WcClass_ctor,
			(void *) 0));
}

static const void * _Wc;

const void * const Wc (void) {
	return _Wc ? _Wc :
		(_Wc = new(WcClass(),
			"Wc", Object(), sizeof(struct Wc),
			wc, "line", Wc_wc,
			printFile, "wrap", Wc_printFile,
			printTotal, "quit", Wc_printTotal,
			(void *) 0));
}

int main (int argc, char * argv [])
{	void * filter = new(Filter(), new(Wc()));

	return mainLoop(filter, argv);
}
