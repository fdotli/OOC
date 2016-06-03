# ifndef Node_r
# define Node_r

# include "Object.r"

struct Node { const struct Object _;
	void * next;
};

struct NodeClass { const struct Class _;
	struct Method exec;
	struct Method reclaim;
};

double super_exec (const void * _class, const void * _self);

void super_reclaim (const void * _class, const void * _self, Method how);

struct Monad { const struct Node _;
	void * down;
};

#define	down(x)		(((struct Monad *)(x)) -> down)

struct Dyad { const struct Node _;
	void * left;
	void * right;
};

#define	left(x)		(((struct Dyad *)(x)) -> left)
#define	right(x)	(((struct Dyad *)(x)) -> right)

struct Number { const struct Node _;
	double value;
};

struct Val { const struct Monad _;
};

struct Global { const struct Val _;
};

struct Parm { const struct Val _;
};

struct Unary { const struct Monad _;
};

struct Minus { const struct Unary _;
};

struct Ref { const struct Dyad _;
};

struct Assign { const struct Ref _;
};

struct Builtin { const struct Ref _;
};

struct User { const struct Ref _;
};

struct Binary { const struct Dyad _;
};

struct Add { const struct Binary _;
};

struct Sub { const struct Binary _;
};

struct Mult { const struct Binary _;
};

struct Div { const struct Binary _;
};

# endif
