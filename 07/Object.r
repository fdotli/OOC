# ifndef Object_r
# define Object_r

#include <assert.h>

struct Object {
	const void * class;
};

struct Class { struct Object _;
	const char * name;
	const void * super;
	size_t size;

	void * (* ctor) (void * _self, va_list * app);
	void * (* dtor) (void * _self);
	int (* differ) (const void * _self, const void * b);
	int (* puto) (const void * _self, FILE * fp);
};

void * super_ctor (const void * _class, void * _self, va_list * app);
void * super_dtor (const void * _class, void * _self);
int super_differ (const void * _class, const void * _self, const void * b);
int super_puto (const void * _class, const void * _self, FILE * fp);

# endif
