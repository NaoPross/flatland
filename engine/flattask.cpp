#include "flattask.h"

/* Static variable definition */
std::list<task_s*> task_s::tasks;

task_s::task_s()
{
    /* Push into the public callback list */
    tasks.push_back(this);
}

task_s::~task_s()
{
    /* Remove from the public callback list */
    tasks.remove(this);
}

void task_s::executeAll()
{
    for (task_s * task : task_s::tasks)
        task->exec();
}

