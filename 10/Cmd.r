# ifndef Cmd_r
# define Cmd_r

# include "Object.r"

struct Cmd { const struct Object _;
};

struct CmdClass { const struct Class _;
	struct Method flag;
	struct Method name;
};

void super_flag (const void * _class, void * _self, void * filter, char flg);
int super_name (const void * _class, void * _self, const void * filter, const char * arg);

# endif
