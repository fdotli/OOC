# ifndef Ic_r
# define Ic_r

# include "Objct.r"

struct Ic { const struct Objct _;
	void * out;
};

struct IcClass { const struct Class _;
	struct Method wire;
	struct Method gate;
};

void super_wire (const void * _class, void * to, void * _self);
enum react super_gate (const void * _class, void * _self, const void * item);

struct Mux { const struct Ic _;
	void * list;
};

struct LineOut { const struct Ic _;
};

struct Button { const struct Ic _;
	const char * text;
};

#define	text(b)	(((const struct Button *) b) -> text)

struct Event { const struct Objct _;
	int kind;
	void * data;
};

typedef	int values[2];

struct Calc { const struct Ic _;
	values value;
	int op;
	int state;
};

# endif
