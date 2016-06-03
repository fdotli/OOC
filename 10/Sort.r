# ifndef Sort_r
# define Sort_r

# include "List.r"

struct Sort { const struct List _;
	char rflag;
};

struct SortClass { const struct ListClass _;
	struct Method flags;
	struct Method line;
	struct Method quit;
};

void super_flags (const void * _class, void * _self, void * filter, char flag);
int super_line (const void * _class, void * _self, const void * filter, const char * fnm, char * buf);
int super_quit (const void * _class, void * _self, const void * filter);

# endif
