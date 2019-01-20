#include "flatland.h"
#include "flatwindow.h"
#include "exceptions/forcequit.h"

int count = 0;

void loop(float);

int main()
{
    FlatWindow win(600, 900, "Stocazzo");
    flat_status status;

    init_flatland(&win, loop, status, 60);

    return 0;
}

#include <iostream>

using namespace std;

void loop(float dt)
{
    ++count;

    if (count == 1000)
        throw ForceQuit("1000 steps reached");

    cout << "Loop number: " << count << endl;
}
