# ifndef Crt_r
# define Crt_r

# include "Ic.r"

struct Crt { const struct Ic _;
	void * window;
	int rows;
	int cols;
};

#define	rows(c)	(((const struct Crt *) c) -> rows)
#define	cols(c)	(((const struct Crt *) c) -> cols)

struct CLineOut { const struct Crt _;
};

struct CButton { const struct Crt _;
	void * button;
	int y;
	int x;
};

# endif
