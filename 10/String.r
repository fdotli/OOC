# ifndef String_r
# define String_r

# include "Object.r"

struct String { const struct Object _;
	const char * text;
};

# endif
