# ifndef Xt_r
# define Xt_r

# include "Objct.r"

struct Xt { const struct Objct _;
	Widget widget;
};

#define	widget(xt)	(((struct Xt *) xt) -> widget)

struct XtApplicationShell { const struct Xt _;
	XtAppContext appContext;
};

struct XawLabel { const struct Xt _;
};

struct XawCommand { const struct Xt _;
};

struct XawBox { const struct Xt _;
};

struct XawForm { const struct Xt _;
};

struct XLineOut { const struct XawLabel _;
};

struct XButton { const struct XawCommand _;
	void * button;
};

# endif
