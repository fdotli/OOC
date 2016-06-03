# ifndef Wc_r
# define Wc_r

# include "Object.r"

struct Wc { const struct Object _;
	unsigned lines;
	unsigned allLines;
	unsigned chars;
	unsigned allChars;
	unsigned files;
};

struct WcClass { const struct Class _;
	struct Method wc;
	struct Method printFile;
	struct Method printTotal;
};

int super_wc (const void * _class, void * _self, const void * filter, const char * fnm, char * buf);
int super_printFile (const void * _class, void * _self, const void * filter, const char * fnm);
int super_printTotal (const void * _class, void * _self, const void * filter);

# endif
