# ifndef Symtab_r
# define Symtab_r

# include "Object.r"

struct Symtab { const struct Object _;
	const void ** buf;
	size_t dim;
	size_t count;
};

# endif
