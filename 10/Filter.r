# ifndef Filter_r
# define Filter_r

# include "Object.r"

struct Filter { const struct Object _;
	void * delegate;
	flagM flag;
	nameM name;
	fileM file;
	lineM line;
	wrapM wrap;
	quitM quit;
	const char * progname;
	char ** argv;
	char * buf;
	unsigned blen;
};

# endif
