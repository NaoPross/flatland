#include "flatsignal.h"

using namespace std;

vector<FlatSignal> FlatSignal::sig_stack;

FlatSignal::FlatSignal(int type, void *data)
    : type(type), data(data)
{
}

FlatSignal::emit(const FlatSignal &signal)
{
    FlatSignal::sig_stack.push_back(signal);
}


