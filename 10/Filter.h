# ifndef Filter_h
# define Filter_h

# include "Object.h"

typedef void (* flagM) (void *, void *, char);
typedef int (* nameM) (void *, const void *, const char *);
typedef int (* fileM) (void *, const void *, const char *,
													FILE *);
typedef int (* lineM) (void *, const void *, const char *,
													char *);
typedef int (* wrapM) (void *, const void *, const char *);
typedef int (* quitM) (void *, const void *);

extern const void * const Filter (void);

int mainLoop (void * _self, char * * argv);
const char * argval (void * _self);
const char * progname (const void * _self);
int doit (void * _self, const char * arg);

# endif
