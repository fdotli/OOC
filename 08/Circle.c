# include "Circle.h"
# include "Circle.r"

static void * Circle_ctor (void * _self, va_list * app) {
	struct Circle * self = super_ctor(Circle, _self, app);

	self -> rad = va_arg(* app, int);
	return self;
}

static void Circle_draw (const void * _self) {
	const struct Circle * self = cast(Circle, _self);

	printf("circle at %d,%d rad %d\n",
			x(self), y(self), self -> rad);
}

const void * Circle;

void initCircle (void) {
	initPoint();

	if (! Circle)
		Circle = new(PointClass,
			"Circle", Point, sizeof(struct Circle),
			ctor, Circle_ctor,
			draw, Circle_draw,
			(void *) 0);
}
