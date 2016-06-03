# include "Cmd.h"
# include "Cmd.r"

static void Cmd_flag (void * _self, void * filter, char flg) {
	const char * value;
	struct Cmd * self = cast(Cmd(), _self);

	assert((flagM) flag == flag);

	switch (flg) {
	case 'f':
		puts("-f");
		break;
	case 'v':
		if ((value = argval(filter)))
			printf("-v \"%s\"\n", value);
		else
			puts("-v NULL");
		break;
	default:
		fprintf(stderr, "%s: unknown flag '%c'\n",
			progname(filter), flg);
	}
}

static int Cmd_name (void * _self, const void * filter, const char * arg) {
	struct Cmd * self = cast(Cmd(), _self);

	assert((nameM) name == name);

	if (arg)
		printf("name \"%s\"\n", arg);
	else
		puts("name NULL");
	return 0;
}

void flag (void * _self, void * filter, char flg) {
	const struct CmdClass * class = cast(CmdClass(), classOf(_self));

	assert(class -> flag.method);
	cast(Object(), filter);

	((void (*) ()) class -> flag.method)(_self, filter, flg);
}

void super_flag (const void * _class, void * _self, void * filter, char flg) {
	const struct CmdClass * superclass = cast(CmdClass(), super(_class));

	cast(Object(), filter);

	assert(superclass -> flag.method);
	((void (*) ()) superclass -> flag.method)(_self, filter, flg);
}

int name (void * _self, const void * filter, const char * arg) {
	int result;
	const struct CmdClass * class = cast(CmdClass(), classOf(_self));

	assert(class -> name.method);
	cast(Object(), filter);

	result = ((int (*) ()) class -> name.method)(_self, filter, arg);
	return result;
}

int super_name (const void * _class, void * _self, const void * filter, const char * arg) {
	const struct CmdClass * superclass = cast(CmdClass(), super(_class));

	cast(Object(), filter);

	assert(superclass -> name.method);
	return ((int (*) ()) superclass -> name.method)(_self, filter, arg);
}

static void * CmdClass_ctor (void * _self, va_list * app) {
	struct CmdClass * self = super_ctor(CmdClass(), _self, app);
	Method selector;
#ifdef va_copy
	va_list ap; va_copy(ap, * app);
#else
	va_list ap = * app;
#endif

	while ((selector = va_arg(ap, Method)))
	{	const char * tag = va_arg(ap, const char *);
		Method method = va_arg(ap, Method);

		if (selector == (Method) flag)
		{	if (tag)
				self -> flag.tag = tag,
				self -> flag.selector = selector;
			self -> flag.method = method;
			continue;
		}
		if (selector == (Method) name)
		{	if (tag)
				self -> name.tag = tag,
				self -> name.selector = selector;
			self -> name.method = method;
			continue;
		}
	}
#ifdef va_copy
	va_end(ap);
#endif
	return self;
}

static const void * _CmdClass;

const void * const CmdClass (void) {
	return _CmdClass ? _CmdClass :
		(_CmdClass = new(Class(),
			"CmdClass", Class(), sizeof(struct CmdClass),
			ctor, "", CmdClass_ctor,
			(void *) 0));
}

static const void * _Cmd;

const void * const Cmd (void) {
	return _Cmd ? _Cmd :
		(_Cmd = new(CmdClass(),
			"Cmd", Object(), sizeof(struct Cmd),
			flag, "flag", Cmd_flag,
			name, "name", Cmd_name,
			(void *) 0));
}

int main (int argc, char * argv [])
{	void * filter = new(Filter(), new(Cmd()));

	return mainLoop(filter, argv);
}
