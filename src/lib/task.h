/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : task.h
* Creation Date : 09-01-2013
* Last Modified : Wed 09 Jan 2013 01:19:34 AM EET
* Created By : Greg Liras <gregliras@gmail.com>
_._._._._._._._._._._._._._._._._._._._._.*/
#ifndef TASK_H 
#define TASK_H

#ifndef MAX_TASKS
#define MAX_TASKS 42
#endif /* MAX_TASKS */

#include <stdlib.h>

struct task {
    void (*func)(void *);
    void *args;
};

typedef struct task struct_task;
cilk void execute(struct_task *);


struct task_node {
    uint32_t id;
    uint32_t children_count;
    uint32_t *children;
    uint32_t dependencies_count;
};

#endif /* TASK_H */
