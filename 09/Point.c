# include "Point.h"
# include "Point.r"

void move (void * _self, int dx, int dy) {
	struct Point * self = cast(Point(), _self);

	self -> x += dx, self -> y += dy;
}

static void * Point_ctor (void * _self, va_list * app) {
	struct Point * self = super_ctor(Point(), _self, app);

	self -> x = va_arg(* app, int);
	self -> y = va_arg(* app, int);
	return self;
}

static void Point_draw (const void * _self) {
	const struct Point * self = cast(Point(), _self);

	printf("\".\" at %d,%d\n", x(self), y(self));
}

void draw (const void * _self) {
	const struct PointClass * class = cast(PointClass(), classOf(_self));

	assert(class -> draw);

	class -> draw(_self);
}

void super_draw (const void * _class, const void * _self) {
	const struct PointClass * superclass = cast(PointClass(), super(_class));

	assert(superclass -> draw);
	superclass -> draw(_self);
}

static void * PointClass_ctor (void * _self, va_list * app) {
	struct PointClass * self = super_ctor(PointClass(), _self, app);
	typedef void (* voidf) ();
	voidf selector;
#ifdef va_copy
	va_list ap; va_copy(ap, * app);
#else
	va_list ap = * app;
#endif

	while ((selector = va_arg(ap, voidf)))
	{	voidf method = va_arg(ap, voidf);

		if (selector == (voidf) draw)
		{	* (voidf *) & self -> draw = method;
			continue;
		}
	}
#ifdef va_copy
	va_end(ap);
#endif
	return self;
}

static const void * _PointClass;

const void * const PointClass (void) {
	return _PointClass ? _PointClass :
		(_PointClass = new(Class(),
			"PointClass", Class(), sizeof(struct PointClass),
			ctor, PointClass_ctor,
			(void *) 0));
}

static const void * _Point;

const void * const Point (void) {
	return _Point ? _Point :
		(_Point = new(PointClass(),
			"Point", Object(), sizeof(struct Point),
			ctor, Point_ctor,
			draw, Point_draw,
			(void *) 0));
}
