# include "Stack.h"
# include "Stack.r"

static struct Object * Stack_add (void * _self, const void * element) {
	return addLast(_self, element);
}

static struct Object * Stack_take (void * _self) {
	return takeLast(_self);
}

const void * Stack;

void initStack (void) {
	initList();

	if (! Stack)
		Stack = new(ListClass,
			"Stack", List, sizeof(struct Stack),
			add, Stack_add,
			take, Stack_take,
			(void *) 0);
}
