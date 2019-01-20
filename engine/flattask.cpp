#include "flattask.h"

bool task_prior::operator()(task_s* s, task_s* g) const
{
    return s->getPriority() <= g->getPriority();
}

/* Static variable definition */
task_set task_s::pre_process_tasks;
task_set task_s::post_process_tasks;

task_s::task_s(bool pre_process, Uint8 priority) 

    : pre_process(pre_process), priority(priority)
{
    /* Push into the public callback list */
    if (pre_process)
        pre_process_tasks.insert(this);
    else
        post_process_tasks.insert(this);
}

task_s::~task_s()
{
    /* Remove from the public callback list */
    if (pre_process)
        pre_process_tasks.erase(this);
    else
        post_process_tasks.erase(this);
}

Uint8 task_s::getPriority() const
{
    return priority;
}
    
void task_s::setPriority(Uint8 priority)
{
    this->priority = priority;
}

void task_s::executePreProcess()
{
    for (task_s * task : task_s::pre_process_tasks)
        task->exec();
}

void task_s::executePostProcess()
{
    for (task_s * task : task_s::post_process_tasks)
        task->exec();
}

