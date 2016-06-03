# include "Node.h"
# include "Node.r"

#include "parse.h"
#include "Symbol.h"

static void * nodes;

static struct Object * Node_new (const void * _self, va_list * app) {
	struct Node * result =
				cast(Node(), super_new(Node(), _self, app));

	result -> next = nodes, nodes = result;
	return (void *) result;
}

void sunder (void * _self) {
	struct Node * self = cast(Node(), _self);

	if (nodes == self)
		nodes = self -> next;
	else if (nodes)
	{	struct Node * np = nodes;

		while (np -> next && np -> next != self)
			np = np -> next;

		if (np -> next)
			np -> next = self -> next;
	}
	self -> next = 0;
}

static void Node_delete (void * _self) {
	struct Node * self = cast(Node(), _self);

	sunder(self);
	super_delete(Node(), self);
}

static void Node_reclaim (const void * _self, Method how) {
	const struct NodeClass * self = cast(NodeClass(), _self);

	while (nodes)
		how(nodes);
}

static void * Number_ctor (void * _self, va_list * app) {
	struct Number * self = super_ctor(Number(), _self, app);

	self -> value = va_arg(* app, double);
	return self;
}

static double Number_exec (const void * _self) {
	const struct Number * self = cast(Number(), _self);

	return self -> value;
}

static void * Monad_ctor (void * _self, va_list * app) {
	struct Monad * self = super_ctor(Monad(), _self, app);

	self -> down = va_arg(* app, void *);
	return self;
}

static double Val_exec (const void * _self) {
	const struct Val * self = cast(Val(), _self);

	return value(down(self));
}

static void * Unary_dtor (void * _self) {
	struct Unary * self = cast(Unary(), _self);

	delete(down(self));
	return super_dtor(Unary(), self);
}

static double Minus_exec (const void * _self) {
	const struct Minus * self = cast(Minus(), _self);

	return - exec(down(self));
}

static void * Dyad_ctor (void * _self, va_list * app) {
	struct Dyad * self = super_ctor(Dyad(), _self, app);

	self -> left = va_arg(* app, void *);
	self -> right = va_arg(* app, void *);
	return self;
}

static void * Ref_dtor (void * _self) {
	struct Ref * self = cast(Ref(), _self);

	delete(right(self));
	return super_dtor(Ref(), self);
}

static double Assign_exec (const void * _self) {
	const struct Assign * self = cast(Assign(), _self);

	return setvalue(left(self), exec(right(self)));
}

static double Builtin_exec (const void * _self) {
	const struct Builtin * self = cast(Builtin(), _self);

	return mathvalue(left(self), exec(right(self)));
}

static double User_exec (const void * _self) {
	const struct User * self = cast(User(), _self);

	return funvalue(left(self), exec(right(self)));
}

static void * Binary_dtor (void * _self) {
	struct Binary * self = cast(Binary(), _self);

	delete(left(self));
	delete(right(self));
	return super_dtor(Binary(), self);
}

static double Add_exec (const void * _self) {
	const struct Add * self = cast(Add(), _self);

	return exec(left(self)) + exec(right(self));
}

static double Sub_exec (const void * _self) {
	const struct Sub * self = cast(Sub(), _self);

	return exec(left(self)) - exec(right(self));
}

static double Mult_exec (const void * _self) {
	const struct Mult * self = cast(Mult(), _self);

	return exec(left(self)) * exec(right(self));
}

static double Div_exec (const void * _self) {
	double x;
	const struct Div * self = cast(Div(), _self);

	if ((x = exec(right(self))) == 0.0)
		error("division by zero");
	return exec(left(self)) / x;
}

double exec (const void * _self) {
	double result;
	const struct NodeClass * class = cast(NodeClass(), classOf(_self));

	assert(class -> exec.method);

	result = ((double (*) ()) class -> exec.method)(_self);
	return result;
}

double super_exec (const void * _class, const void * _self) {
	const struct NodeClass * superclass = cast(NodeClass(), super(_class));

	assert(superclass -> exec.method);
	return ((double (*) ()) superclass -> exec.method)(_self);
}

void reclaim (const void * _self, Method how) {
	const struct NodeClass * class = cast(NodeClass(), _self);

	assert(class -> reclaim.method);

	((void (*) ()) class -> reclaim.method)(_self, how);
}

void super_reclaim (const void * _class, const void * _self, Method how) {
	const struct NodeClass * superclass = cast(NodeClass(), super(_class));

	assert(superclass -> reclaim.method);
	((void (*) ()) superclass -> reclaim.method)(_self, how);
}

static void * NodeClass_ctor (void * _self, va_list * app) {
	struct NodeClass * self = super_ctor(NodeClass(), _self, app);
	Method selector;
#ifdef va_copy
	va_list ap; va_copy(ap, * app);
#else
	va_list ap = * app;
#endif

	while ((selector = va_arg(ap, Method)))
	{	const char * tag = va_arg(ap, const char *);
		Method method = va_arg(ap, Method);

		if (selector == (Method) exec)
		{	if (tag)
				self -> exec.tag = tag,
				self -> exec.selector = selector;
			self -> exec.method = method;
			continue;
		}
		if (selector == (Method) reclaim)
		{	if (tag)
				self -> reclaim.tag = tag,
				self -> reclaim.selector = selector;
			self -> reclaim.method = method;
			continue;
		}
	}
#ifdef va_copy
	va_end(ap);
#endif
	return self;
}

static const void * _NodeClass;

const void * const NodeClass (void) {
	return _NodeClass ? _NodeClass :
		(_NodeClass = new(Class(),
			"NodeClass", Class(), sizeof(struct NodeClass),
			ctor, "", NodeClass_ctor,
			(void *) 0));
}

static const void * _Node;

const void * const Node (void) {
	return _Node ? _Node :
		(_Node = new(NodeClass(),
			"Node", Object(), sizeof(struct Node),
			new, "", Node_new,
			delete, "delete", Node_delete,
			reclaim, "", Node_reclaim,
			(void *) 0));
}
static const void * _Monad;

const void * const Monad (void) {
	return _Monad ? _Monad :
		(_Monad = new(NodeClass(),
			"Monad", Node(), sizeof(struct Monad),
			ctor, "ctor", Monad_ctor,
			(void *) 0));
}
static const void * _Dyad;

const void * const Dyad (void) {
	return _Dyad ? _Dyad :
		(_Dyad = new(NodeClass(),
			"Dyad", Node(), sizeof(struct Dyad),
			ctor, "ctor", Dyad_ctor,
			(void *) 0));
}
static const void * _Number;

const void * const Number (void) {
	return _Number ? _Number :
		(_Number = new(NodeClass(),
			"Number", Node(), sizeof(struct Number),
			ctor, "ctor", Number_ctor,
			exec, "exec", Number_exec,
			(void *) 0));
}
static const void * _Val;

const void * const Val (void) {
	return _Val ? _Val :
		(_Val = new(NodeClass(),
			"Val", Monad(), sizeof(struct Val),
			exec, "exec", Val_exec,
			(void *) 0));
}
static const void * _Global;

const void * const Global (void) {
	return _Global ? _Global :
		(_Global = new(NodeClass(),
			"Global", Val(), sizeof(struct Global),
			(void *) 0));
}
static const void * _Parm;

const void * const Parm (void) {
	return _Parm ? _Parm :
		(_Parm = new(NodeClass(),
			"Parm", Val(), sizeof(struct Parm),
			(void *) 0));
}
static const void * _Unary;

const void * const Unary (void) {
	return _Unary ? _Unary :
		(_Unary = new(NodeClass(),
			"Unary", Monad(), sizeof(struct Unary),
			dtor, "dtor", Unary_dtor,
			(void *) 0));
}
static const void * _Minus;

const void * const Minus (void) {
	return _Minus ? _Minus :
		(_Minus = new(NodeClass(),
			"Minus", Unary(), sizeof(struct Minus),
			exec, "exec", Minus_exec,
			(void *) 0));
}
static const void * _Ref;

const void * const Ref (void) {
	return _Ref ? _Ref :
		(_Ref = new(NodeClass(),
			"Ref", Dyad(), sizeof(struct Ref),
			dtor, "dtor", Ref_dtor,
			(void *) 0));
}
static const void * _Assign;

const void * const Assign (void) {
	return _Assign ? _Assign :
		(_Assign = new(NodeClass(),
			"Assign", Ref(), sizeof(struct Assign),
			exec, "exec", Assign_exec,
			(void *) 0));
}
static const void * _Builtin;

const void * const Builtin (void) {
	return _Builtin ? _Builtin :
		(_Builtin = new(NodeClass(),
			"Builtin", Ref(), sizeof(struct Builtin),
			exec, "exec", Builtin_exec,
			(void *) 0));
}
static const void * _User;

const void * const User (void) {
	return _User ? _User :
		(_User = new(NodeClass(),
			"User", Ref(), sizeof(struct User),
			exec, "exec", User_exec,
			(void *) 0));
}
static const void * _Binary;

const void * const Binary (void) {
	return _Binary ? _Binary :
		(_Binary = new(NodeClass(),
			"Binary", Dyad(), sizeof(struct Binary),
			dtor, "dtor", Binary_dtor,
			(void *) 0));
}
static const void * _Add;

const void * const Add (void) {
	return _Add ? _Add :
		(_Add = new(NodeClass(),
			"Add", Binary(), sizeof(struct Add),
			exec, "exec", Add_exec,
			(void *) 0));
}
static const void * _Sub;

const void * const Sub (void) {
	return _Sub ? _Sub :
		(_Sub = new(NodeClass(),
			"Sub", Binary(), sizeof(struct Sub),
			exec, "exec", Sub_exec,
			(void *) 0));
}
static const void * _Mult;

const void * const Mult (void) {
	return _Mult ? _Mult :
		(_Mult = new(NodeClass(),
			"Mult", Binary(), sizeof(struct Mult),
			exec, "exec", Mult_exec,
			(void *) 0));
}
static const void * _Div;

const void * const Div (void) {
	return _Div ? _Div :
		(_Div = new(NodeClass(),
			"Div", Binary(), sizeof(struct Div),
			exec, "exec", Div_exec,
			(void *) 0));
}
