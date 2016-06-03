# ifndef Point_r
# define Point_r

# include "Object.r"

struct Point { const struct Object _;
	int x;
	int y;
};

struct PointClass { const struct Class _;
	void (* draw) (const void * _self);
};

void super_draw (const void * _class, const void * _self);

#define	x(p)	(((const struct Point *)(p)) -> x)
#define	y(p)	(((const struct Point *)(p)) -> y)

# endif
