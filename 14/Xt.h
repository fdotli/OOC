# ifndef Xt_h
# define Xt_h

# include "Objct.h"

#include <X11/Intrinsic.h>

extern const void * const Xt (void);

void * makeWidget (void * _self, WidgetClass wc, va_list * app);
void addAllAccelerators (void * _self);
void setLabel (void * _self, const char * text);
void addCallback (void * _self, XtCallbackProc fun, XtPointer data);

extern const void * const XtApplicationShell (void);

void mainLoop (void * _self);

extern const void * const XawLabel (void);

extern const void * const XawCommand (void);

extern const void * const XawBox (void);

extern const void * const XawForm (void);

extern const void * const XLineOut (void);

extern const void * const XButton (void);

# endif
