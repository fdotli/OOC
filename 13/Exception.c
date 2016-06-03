# include "Exception.h"
# include "Exception.r"

#include "List.h"

static void * stack;

static void * Exception_ctor (void * _self, va_list * app) {
	void * self = super_ctor(Exception(), _self, app);

	if (! stack)
		stack = new(List(), 10);
	addLast(stack, self);
	return self;
}

static void * Exception_dtor (void * _self) {
	void * top;
	struct Exception * self = cast(Exception(), _self);

	assert(stack);
	top = takeLast(stack);
	assert(top == self);
	return super_dtor(Exception(), self);
}

void * catchException (void * _self) {
	struct Exception * self = cast(Exception(), _self);

	self -> armed = 1;
	return self -> label;
}

void cause (int number) {
	unsigned cnt;

	if (number && stack && (cnt = count(stack)))
	{	void * top = lookAt(stack, cnt-1);
		struct Exception * e = cast(Exception(), top);

		if (e -> armed)
			longjmp(e -> label, number);
	}
}

static const void * _Exception;

const void * const Exception (void) {
	return _Exception ? _Exception :
		(_Exception = new(Class(),
			"Exception", Object(), sizeof(struct Exception),
			ctor, "ctor", Exception_ctor,
			dtor, "dtor", Exception_dtor,
			(void *) 0));
}
