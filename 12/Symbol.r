# ifndef Symbol_r
# define Symbol_r

# include "Object.r"

struct Symbol { const struct Object _;
	const char * name;
	int lex;
};

struct Reserved { const struct Symbol _;
};

struct Var { const struct Symbol _;
	double value;
};

struct VarClass { const struct Class _;
	struct Method move;
};

void super_move (const void * _class, void * _self, void * _from);

struct Const { const struct Var _;
};

struct Fun { const struct Var _;
	void * fun;
};

struct Math { const struct Symbol _;
	function fun;
};

# endif
