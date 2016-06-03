# include "Crt.h"
# include "Crt.r"

#include <curses.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

static void * Crt_ctor (void * _self, va_list * app) {
	struct Crt * self = super_ctor(Crt(), _self, app);
	static unsigned twice;

	if (! twice ++)
	{
#if! defined SYSV && ! defined GNUDOS
		int endwin (void);
#endif
		initscr(), atexit((void (*) (void)) endwin);
		noecho(), cbreak();
		signal(SIGINT, (void (*) (int)) exit);
		refresh();
	}
	return self;
}

static int Crt_puto (const void * _self, FILE * fp) {
	int result;
	const struct Crt * self = cast(Crt(), _self);

	result = super_puto(Crt(), self, fp);
	return result + fprintf(fp, "window %p %d/%d\n",
				self -> window, cols(self), rows(self));
}

void makeWindow (void * _self, int rows, int cols, int y, int x) {
	struct Crt * self = cast(Crt(), _self);

	if ((self -> rows = rows))
	{	self -> cols = cols;
		self -> window = newwin(self -> rows, self -> cols, y, x);
	}
	else
	{	self -> rows = LINES;
		self -> cols = COLS;
		self -> window = stdscr;
	}
	assert(self -> window);
	wmove(self -> window, rows(self)/2, cols(self)/2);
}

void addStr (void * _self, int y, int x, const char * s) {
	struct Crt * self = cast(Crt(), _self);

	assert(self -> window);
	werase(self -> window);
	mvwaddstr(self -> window, y, x, s);
	wrefresh(self -> window);
}

void crtBox (void * _self) {
	struct Crt * self = cast(Crt(), _self);

	assert(self -> window);
	box(self -> window, '|', '-');
	wrefresh(self -> window);
}

static enum react Crt_gate (void * _self, const void * item) {
# define x v[0]
# define y v[1]
	int v[2];
	static char buf [2];
	struct Crt * self = cast(Crt(), _self);

	assert(self -> window);
	getyx(((WINDOW *) self -> window), y, x);
#if defined SYSV || defined GNUDOS
	keypad(self -> window, TRUE);
#endif
	for (;;)
	{	void * e;
		int ch;

		wmove(self -> window, y, x), wrefresh(self -> window);
		switch (ch = wgetch(self -> window)) {
		case EOF:
		case 4:
			return reject;
		case '\n': case '\r':
			e = new(Event(), 1, v);
			break;
		default:
			buf[0] = ch;
			e = new(Event(), 0, buf);
			break;
#ifdef KEY_LEFT
		case KEY_LEFT:
#else
		case 'h':
#endif
			if (x > 0)
				-- x;
			continue;
#ifdef KEY_DOWN
		case KEY_DOWN:
#else
		case 'j':
#endif
			if (y < self -> rows-1)
				++ y;
			continue;
#ifdef KEY_UP
		case KEY_UP:
#else
		case 'k':
#endif
			if (y > 0)
				-- y;
			continue;
#ifdef KEY_RIGHT
		case KEY_RIGHT:
#else
		case 'l':
#endif
			if (x < self -> cols-1)
				++ x;
			continue;
		}
		super_gate(Crt(), self, e);
		delete(e);
	}
# undef	x
# undef	y
}

static void * CLineOut_ctor (void * _self, va_list * app) {
	struct CLineOut * self = super_ctor(CLineOut(), _self, app);
	int y = va_arg(* app, int);
	int x = va_arg(* app, int);
	int len = va_arg(* app, int);

	makeWindow(self, 1, len, y, x);
	return self;
}

static enum react CLineOut_gate (void * _self, const void * item) {
	struct CLineOut * self = cast(CLineOut(), _self);

	addStr(self, 0, 0, item);
	return accept;
}

static void * CButton_ctor (void * _self, va_list * app) {
	struct CButton * self = super_ctor(CButton(), _self, app);

	self -> button =
				new(Button(), va_arg(* app, const char *));
	self -> y = va_arg(* app, int);
	self -> x = va_arg(* app, int);

	makeWindow(self, 3, strlen(text(self -> button)) + 4,
						self -> y, self -> x);
	addStr(self, 1, 2, text(self -> button));
	crtBox(self);
	return self;
}

static int CButton_puto (const void * _self, FILE * fp) {
	int result;
	const struct CButton * self = cast(CButton(), _self);

	result = super_puto(CButton(), self, fp);
	result += puto(self -> button, fp);
	return result + fprintf(fp, "at %d/%d\n", self -> y, self -> x);
}

static enum react CButton_gate (void * _self, const void * item) {
	struct CButton * self = cast(CButton(), _self);

	if (kind(item) == 1)
	{	int * v = data(item);

		if (v[0] >= self -> x && v[0] < self -> x + cols(self)
		&& v[1] >= self -> y && v[1] < self -> y + rows(self))
			return gate(self -> button, 0);
		return reject;
	}
	return gate(self -> button, item);
}

static void CButton_wire (void * to, void * _self) {
	struct CButton * self = cast(CButton(), _self);

	wire(to, self -> button);
}

static const void * _Crt;

const void * const Crt (void) {
	return _Crt ? _Crt :
		(_Crt = new(IcClass(),
			"Crt", Ic(), sizeof(struct Crt),
			ctor, "ctor", Crt_ctor,
			puto, "puto", Crt_puto,
			gate, "gate", Crt_gate,
			(void *) 0));
}
static const void * _CLineOut;

const void * const CLineOut (void) {
	return _CLineOut ? _CLineOut :
		(_CLineOut = new(IcClass(),
			"CLineOut", Crt(), sizeof(struct CLineOut),
			ctor, "ctor", CLineOut_ctor,
			gate, "gate", CLineOut_gate,
			(void *) 0));
}
static const void * _CButton;

const void * const CButton (void) {
	return _CButton ? _CButton :
		(_CButton = new(IcClass(),
			"CButton", Crt(), sizeof(struct CButton),
			ctor, "ctor", CButton_ctor,
			puto, "puto", CButton_puto,
			gate, "gate", CButton_gate,
			wire, "wire", CButton_wire,
			(void *) 0));
}
