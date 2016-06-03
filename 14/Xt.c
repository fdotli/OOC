# include "Xt.h"
# include "Xt.r"

#include <X11/StringDefs.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>

#include "Ic.h"

static int Xt_puto (const void * _self, FILE * fp) {
	int result;
	const struct Xt * self = cast(Xt(), _self);

	result = super_puto(Xt(), self, fp);
	return result + fprintf(fp, "widget %p\n", widget(self));
}

void * makeWidget (void * _self, WidgetClass wc, va_list * app) {
	void * parent = va_arg(* app, void *);
	const char * name = va_arg(* app, const char *);
	struct Xt * self = cast(Xt(), _self);

	if (! name || ! * name)
		name = "widget";
	widget(self) =
		XtCreateManagedWidget(name, wc, widget(parent), NULL, 0);
	return self;
}

void addAllAccelerators (void * _self) {
	struct Xt * self = cast(Xt(), _self);

	assert(widget(self));
	XtInstallAllAccelerators(widget(self), widget(self));
}

void setLabel (void * _self, const char * text) {
	Arg list;
	struct Xt * self = cast(Xt(), _self);

	list.name = XtNlabel, list.value = (XtArgVal) text;
	XtSetValues(widget(self), & list, 1);
}

void addCallback (void * _self, XtCallbackProc fun, XtPointer data) {
	struct Xt * self = cast(Xt(), _self);

	XtAddCallback(widget(self), XtNcallback, fun, data);
}

static void * XtApplicationShell_ctor (void * _self, va_list * app) {
	struct XtApplicationShell * self
				= super_ctor(XtApplicationShell(), _self, app);
	int * argcp = va_arg(* app, int *);
	char ** argv = va_arg(* app, char **);

	widget(self) = XtAppInitialize(& self -> appContext, "Xapp",
			NULL, 0,
			argcp, argv,
			NULL,
			NULL, 0);
	return self;
}

void mainLoop (void * _self) {
	struct XtApplicationShell * self = cast(XtApplicationShell(), _self);

	XtRealizeWidget(widget(self));
	XtAppMainLoop(self -> appContext);
}

static void * XawLabel_ctor (void * _self, va_list * app) {
	struct XawLabel * self = super_ctor(XawLabel(), _self, app);

	makeWidget(self, labelWidgetClass, app);
	return self;
}

static void * XawCommand_ctor (void * _self, va_list * app) {
	struct XawCommand * self = super_ctor(XawCommand(), _self, app);

	makeWidget(self, commandWidgetClass, app);
	return self;
}

static void * XawBox_ctor (void * _self, va_list * app) {
	struct XawBox * self = super_ctor(XawBox(), _self, app);

	makeWidget(self, boxWidgetClass, app);
	return self;
}

static void * XawForm_ctor (void * _self, va_list * app) {
	struct XawCommand * self = super_ctor(XawForm(), _self, app);

	makeWidget(self, formWidgetClass, app);
	return self;
}

static void * XLineOut_ctor (void * _self, va_list * app) {
	struct XLineOut * self =
						super_ctor(XLineOut(), _self, app);
	const char * text = va_arg(* app, const char *);

	gate(self, text);
	return self;
}

static void XLineOut_forward (const void * _self, void * result, Method selector, const char * name, va_list * app) {
	const struct XLineOut * self = cast(XLineOut(), _self);

	if (selector == (Method) gate)
	{	va_arg(* app, void *);
		setLabel((void *) self, va_arg(* app, const void *));
		* (enum react *) result = accept;
	}
	else
		super_forward(XLineOut(), _self, result,
									selector, name, app);
}

static void tell (Widget w, XtPointer client_data,
									XtPointer call_data)
{
	gate(client_data, NULL);
}

static void * XButton_ctor (void * _self, va_list * app) {
	struct XButton * self = super_ctor(XButton(), _self, app);
	const char * text = va_arg(* app, const char *);

	self -> button = new(Button(), text);
	setLabel(self, text);
	addCallback(self, tell, self -> button);
	return self;
}

static int XButton_puto (const void * _self, FILE * fp) {
	int result;
	const struct XButton * self = cast(XButton(), _self);

	result = super_puto(XButton(), self, fp);
	return result + puto(self -> button, fp);
}

static void XButton_forward (const void * _self, void * result, Method selector, const char * name, va_list * app) {
	const struct XButton * self = cast(XButton(), _self);

	if (selector == wire)
		wire(va_arg(* app, void *), self -> button);
	else
		super_forward(XButton(), _self, result,
									selector, name, app);
}

static const void * _Xt;

const void * const Xt (void) {
	return _Xt ? _Xt :
		(_Xt = new(Class(),
			"Xt", Objct(), sizeof(struct Xt),
			puto, "puto", Xt_puto,
			(void *) 0));
}
static const void * _XtApplicationShell;

const void * const XtApplicationShell (void) {
	return _XtApplicationShell ? _XtApplicationShell :
		(_XtApplicationShell = new(Class(),
			"XtApplicationShell", Xt(), sizeof(struct XtApplicationShell),
			ctor, "ctor", XtApplicationShell_ctor,
			(void *) 0));
}
static const void * _XawLabel;

const void * const XawLabel (void) {
	return _XawLabel ? _XawLabel :
		(_XawLabel = new(Class(),
			"XawLabel", Xt(), sizeof(struct XawLabel),
			ctor, "ctor", XawLabel_ctor,
			(void *) 0));
}
static const void * _XawCommand;

const void * const XawCommand (void) {
	return _XawCommand ? _XawCommand :
		(_XawCommand = new(Class(),
			"XawCommand", Xt(), sizeof(struct XawCommand),
			ctor, "ctor", XawCommand_ctor,
			(void *) 0));
}
static const void * _XawBox;

const void * const XawBox (void) {
	return _XawBox ? _XawBox :
		(_XawBox = new(Class(),
			"XawBox", Xt(), sizeof(struct XawBox),
			ctor, "ctor", XawBox_ctor,
			(void *) 0));
}
static const void * _XawForm;

const void * const XawForm (void) {
	return _XawForm ? _XawForm :
		(_XawForm = new(Class(),
			"XawForm", Xt(), sizeof(struct XawForm),
			ctor, "ctor", XawForm_ctor,
			(void *) 0));
}
static const void * _XLineOut;

const void * const XLineOut (void) {
	return _XLineOut ? _XLineOut :
		(_XLineOut = new(Class(),
			"XLineOut", XawLabel(), sizeof(struct XLineOut),
			ctor, "ctor", XLineOut_ctor,
			forward, "forward", XLineOut_forward,
			(void *) 0));
}
static const void * _XButton;

const void * const XButton (void) {
	return _XButton ? _XButton :
		(_XButton = new(Class(),
			"XButton", XawCommand(), sizeof(struct XButton),
			ctor, "ctor", XButton_ctor,
			puto, "puto", XButton_puto,
			forward, "forward", XButton_forward,
			(void *) 0));
}
