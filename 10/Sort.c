# include "Sort.h"
# include "Sort.r"

#include "Filter.h"
#include "String.h"
#include <stdlib.h>

static void Sort_flags (void * _self, void * filter, char flag) {
	struct Sort * self = cast(Sort(), _self);

	assert((flagM) flags == flags);

	if (flag == 'r')
		self -> rflag = 1;
	else
		fprintf(stderr, "usage: %s [-r] [file...]\n",
							progname(filter)),
		exit(1);
}

static int Sort_line (void * _self, const void * filter, const char * fnm, char * buf) {
	struct Sort * self = cast(Sort(), _self);

	assert((lineM) line == line);

	addLast(self, new(String(), buf));
	return 0;
}

static int Sort_quit (void * _self, const void * filter) {
	struct Sort * self = cast(Sort(), _self);

	assert((quitM) quit == quit);

	if (count(self))
	{	sort(self);
		do
			puto(self -> rflag ? takeLast(self)
							: takeFirst(self), stdout);
		while (count(self));
	}
	return 0;
}

void flags (void * _self, void * filter, char flag) {
	const struct SortClass * class = cast(SortClass(), classOf(_self));

	assert(class -> flags.method);
	cast(Object(), filter);

	((void (*) ()) class -> flags.method)(_self, filter, flag);
}

void super_flags (const void * _class, void * _self, void * filter, char flag) {
	const struct SortClass * superclass = cast(SortClass(), super(_class));

	cast(Object(), filter);

	assert(superclass -> flags.method);
	((void (*) ()) superclass -> flags.method)(_self, filter, flag);
}

int line (void * _self, const void * filter, const char * fnm, char * buf) {
	int result;
	const struct SortClass * class = cast(SortClass(), classOf(_self));

	assert(class -> line.method);
	cast(Object(), filter);

	result = ((int (*) ()) class -> line.method)(_self, filter, fnm, buf);
	return result;
}

int super_line (const void * _class, void * _self, const void * filter, const char * fnm, char * buf) {
	const struct SortClass * superclass = cast(SortClass(), super(_class));

	cast(Object(), filter);

	assert(superclass -> line.method);
	return ((int (*) ()) superclass -> line.method)(_self, filter, fnm, buf);
}

int quit (void * _self, const void * filter) {
	int result;
	const struct SortClass * class = cast(SortClass(), classOf(_self));

	assert(class -> quit.method);
	cast(Object(), filter);

	result = ((int (*) ()) class -> quit.method)(_self, filter);
	return result;
}

int super_quit (const void * _class, void * _self, const void * filter) {
	const struct SortClass * superclass = cast(SortClass(), super(_class));

	cast(Object(), filter);

	assert(superclass -> quit.method);
	return ((int (*) ()) superclass -> quit.method)(_self, filter);
}

static void * SortClass_ctor (void * _self, va_list * app) {
	struct SortClass * self = super_ctor(SortClass(), _self, app);
	Method selector;
#ifdef va_copy
	va_list ap; va_copy(ap, * app);
#else
	va_list ap = * app;
#endif

	while ((selector = va_arg(ap, Method)))
	{	const char * tag = va_arg(ap, const char *);
		Method method = va_arg(ap, Method);

		if (selector == (Method) flags)
		{	if (tag)
				self -> flags.tag = tag,
				self -> flags.selector = selector;
			self -> flags.method = method;
			continue;
		}
		if (selector == (Method) line)
		{	if (tag)
				self -> line.tag = tag,
				self -> line.selector = selector;
			self -> line.method = method;
			continue;
		}
		if (selector == (Method) quit)
		{	if (tag)
				self -> quit.tag = tag,
				self -> quit.selector = selector;
			self -> quit.method = method;
			continue;
		}
	}
#ifdef va_copy
	va_end(ap);
#endif
	return self;
}

static const void * _SortClass;

const void * const SortClass (void) {
	return _SortClass ? _SortClass :
		(_SortClass = new(Class(),
			"SortClass", ListClass(), sizeof(struct SortClass),
			ctor, "", SortClass_ctor,
			(void *) 0));
}

static const void * _Sort;

const void * const Sort (void) {
	return _Sort ? _Sort :
		(_Sort = new(SortClass(),
			"Sort", List(), sizeof(struct Sort),
			flags, "flag", Sort_flags,
			line, "line", Sort_line,
			quit, "quit", Sort_quit,
			(void *) 0));
}

int main (int argc, char * argv [])
{	void * filter = new(Filter(), new(Sort(), 0));

	return mainLoop(filter, argv);
}
