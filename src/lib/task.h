/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : task.h
* Creation Date : 09-01-2013
* Last Modified : Tue 22 Jan 2013 03:48:30 PM EET
* Created By : Greg Liras <gregliras@gmail.com>
_._._._._._._._._._._._._._._._._._._._._.*/
#ifndef TASK_H 
#define TASK_H

#ifndef MAX_TASKS
#define MAX_TASKS 42
#endif /* MAX_TASKS */

#include <stdlib.h>

struct task {
    void *(*func)(void *, int);
    void *args;
    void *value;
};

typedef struct task struct_task;
cilk void execute(struct_task *);


struct task_node {
    uint32_t id;
    uint32_t children_count;
    uint32_t *children;
    //volatile gint lock;
    GMutex  lock;
    uint32_t dependencies_count;
    struct_task *mtask;
};
typedef struct task_node struct_task_node;



void tasks_initialize(void);

#endif /* TASK_H */
