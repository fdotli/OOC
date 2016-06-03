# ifndef List_r
# define List_r

# include "Object.r"

struct List { const struct Object _;
	const void ** buf;
	unsigned dim;
	unsigned count;
	unsigned begin;
	unsigned end;
};

struct ListClass { const struct Class _;
	struct Object * (* add) (void * _self, const void * element);
	struct Object * (* take) (void * _self);
};

struct Object * super_add (const void * _class, void * _self, const void * element);
struct Object * super_take (const void * _class, void * _self);

# endif
