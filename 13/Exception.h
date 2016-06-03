# ifndef Exception_h
# define Exception_h

# include "Object.h"

#include <setjmp.h>

#define	catch(e)	setjmp(catchException(e))

void cause (int number);

extern const void * const Exception (void);

void * catchException (void * _self);

# endif
