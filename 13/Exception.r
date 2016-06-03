# ifndef Exception_r
# define Exception_r

# include "Object.r"

struct Exception { const struct Object _;
	int armed;
	jmp_buf label;
};

# endif
