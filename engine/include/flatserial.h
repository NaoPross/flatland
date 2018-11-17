#ifndef __FLATSERIAL_H__
#define __FLATSERIAL_H__

/* SDL serial events handling */

class Focusable;

void process_events();

void registerFocusable(Focusable*);

void unregisterFocusable(Focusable*);

#endif
