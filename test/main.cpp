#include "flatland.hpp"
#include "window.hpp"
#include "exceptions/forcequit.hpp"

#include "serial/keyfocusable.hpp"
#include <SDL2/SDL.h>

#include <iostream>
#include <string>

#include "core/signal.hpp"

using namespace std;

int count = 0;

void loop(float);

class KeyPrinter : public KeyFocusable
{
    virtual void key_cb(const SDL_KeyboardEvent* key) override
    {
        string msg = (key->type == SDL_KEYDOWN) ? "pressed" : "released";
        cout << "KeyPrinter: key " << (char)key->keysym.sym << " " << msg << endl;
    };

public:

    using KeyFocusable::KeyFocusable;
};

class KeyCaller : public KeyFocusable
{
    int counter;
    char last;

    virtual void key_cb(const SDL_KeyboardEvent* key) override
    {
        if (key->type == SDL_KEYDOWN) {

            char c = key->keysym.sym;
            
            if (c == last)
                ++counter;
            else {
                last = c;
                counter = 0;
                cout << "KeyCaller: Counter set to 0" << endl;
            }
        }

        if (counter == 20)
        {
            FlatSignal signal(this, "char", (void*)&last);
            signal.emit("alpha");

            cout << "KeyCaller: 20 times reached" << endl;
            cout << "KeyCaller: sending " << last << endl;
        }

        if (counter == 50)
        {
            cout << "KeyCaller: 50 times pressed " << last << endl;

            /* Exit application */
            FlatSignal signal(this, "quit");
            signal.emit("core");
        }
    };

public:

    KeyCaller() : counter(0), last('\0') 
    {
        setID("CALLER");
    }
};

class TwentyListener : public FlatListener
{
    virtual void callback(FlatObject *sender, void *data) override
    {
        char c = *(char*)data;
        cout << "TwentyListener: received " << c << " from " << sender->getID() << endl;
    }

public:

    TwentyListener() : FlatListener({"char"}) {}
};

KeyPrinter printer;
KeyCaller caller;
TwentyListener twlistener;

SignalChannel alpha("alpha");

int main()
{
    FlatWindow win(600, 900, "Test 3");
    flat_status status;

    /* Connect listener to alpha channel */
    alpha.connect(&twlistener);

    init_flatland(&win, loop, status, 60);

    return 0;
}

void loop(float dt)
{
    ++count;

    if (count == 10000)
        throw ForceQuit("10000 steps reached");

    //cout << "Loop number: " << count << endl;
}

