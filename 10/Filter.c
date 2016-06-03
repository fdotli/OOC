# include "Filter.h"
# include "Filter.r"

#include <stdlib.h>
#include <string.h>

static void * Filter_ctor (void * _self, va_list * app) {
	struct Filter * self = super_ctor(Filter(), _self, app);

	self -> delegate = va_arg(* app, void *);

	self -> flag = (flagM) respondsTo(self -> delegate, "flag");
	self -> name = (nameM) respondsTo(self -> delegate, "name");
	self -> file = (fileM) respondsTo(self -> delegate, "file");
	self -> line = (lineM) respondsTo(self -> delegate, "line");
	self -> wrap = (wrapM) respondsTo(self -> delegate, "wrap");
	self -> quit = (quitM) respondsTo(self -> delegate, "quit");

	return self;
}

static void * Filter_dtor (void * _self) {
	struct Filter * self = cast(Filter(), _self);

	if (self -> buf)
		free(self -> buf);
	return super_dtor(Filter(), self);
}

int mainLoop (void * _self, char * * argv) {
	int result;
	struct Filter * self = cast(Filter(), _self);

	self -> progname = * argv ++;

	while (* argv && ** argv == '-')
	{	switch (* ++ * argv) {
		case 0:
			-- * argv;
			break;
		case '-':
			if (! (* argv)[1])
			{   ++ argv;
				break;
			}
		default:
			do
				if (self -> flag)
				{	self -> argv = argv;
					self -> flag(self -> delegate,
										self, ** argv);
					argv = self -> argv;
				}
				else
				{	fprintf(stderr,
						"%s: -%c: no flags allowed\n",
						self -> progname, ** argv);
					return 1;
				}
			while (* ++ * argv);
			++ argv;
			continue;
		}
		break;
	}

	if (* argv)
		do
			result = doit(self, * argv);
		while (! result && * ++ argv);
	else
		result = doit(self, NULL);

	if (self -> quit)
		result = self -> quit(self -> delegate, self);
	return result;
}

const char * argval (void * _self) {
	const char * result;
	struct Filter * self = cast(Filter(), _self);

	assert(self -> argv && * self -> argv);

	if ((* self -> argv)[1])
		result = ++ * self -> argv;
	else if (self -> argv[1])
		result = * ++ self -> argv;
	else
		result = NULL;

	while ((* self -> argv)[1])
		++ * self -> argv;

	return result;
}

const char * progname (const void * _self) {
	const struct Filter * self = cast(Filter(), _self);

	return self -> progname;
}

int doit (void * _self, const char * arg) {
	FILE * fp;
	int result = 0;
	struct Filter * self = cast(Filter(), _self);

	if (self -> name)
		return self -> name(self -> delegate, self, arg);

	if (! arg || strcmp(arg, "-") == 0)
		fp = stdin, clearerr(fp);
	else if (! * arg)
	{	fprintf(stderr, "%s: null filename\n",
								self -> progname);
		return 1;
	}
	else if (! (fp = fopen(arg, "r")))
	{	perror(arg);
		return 1;
	}

	if (self -> file)
		result = self -> file(self -> delegate, self, arg, fp);
	else
	{	if (! self -> buf)
		{	self -> blen = BUFSIZ;
			self -> buf = malloc(self -> blen);
			assert(self -> buf);
		}

		while (fgets(self -> buf, self -> blen, fp))
		{	while (strlen(self -> buf) == self -> blen - 1
					&& self -> buf[self -> blen - 2] != '\n')
			{	self -> blen *= 2;
				self -> buf = realloc(self -> buf, self -> blen);
				assert(self -> buf);
				if (! fgets(self->buf + self->blen/2 - 1,
							self->blen/2 + 1, fp))
					break;
			}
			if (self -> line && (result =
				self -> line(self -> delegate, self, arg,
												self -> buf)))
				break;
		}

		if (self -> wrap)
			result = self -> wrap(self -> delegate, self, arg);
	}

	if (fp != stdin)
		fclose(fp);

	if (fflush(stdout), ferror(stdout))
	{	fprintf(stderr, "%s: output error\n", self -> progname);
		result = 1;
	}
	return result;
}

static const void * _Filter;

const void * const Filter (void) {
	return _Filter ? _Filter :
		(_Filter = new(Class(),
			"Filter", Object(), sizeof(struct Filter),
			ctor, "ctor", Filter_ctor,
			dtor, "dtor", Filter_dtor,
			(void *) 0));
}
