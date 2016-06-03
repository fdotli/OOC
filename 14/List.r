# ifndef List_r
# define List_r

# include "Objct.r"

struct List { const struct Objct _;
	const void ** buf;
	unsigned dim;
	unsigned count;
	unsigned begin;
	unsigned end;
};

struct ListClass { const struct Class _;
	struct Method add;
	struct Method take;
};

struct Objct * super_add (const void * _class, void * _self, const void * element);
struct Objct * super_take (const void * _class, void * _self);

# endif
