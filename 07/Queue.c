# include "Queue.h"
# include "Queue.r"

static struct Object * Queue_add (void * _self, const void * element) {
	return addLast(_self, element);
}

static struct Object * Queue_take (void * _self) {
	return takeFirst(_self);
}

const void * Queue;

void initQueue (void) {
	initList();

	if (! Queue)
		Queue = new(ListClass,
			"Queue", List, sizeof(struct Queue),
			add, Queue_add,
			take, Queue_take,
			(void *) 0);
}
