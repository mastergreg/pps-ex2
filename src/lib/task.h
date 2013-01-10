/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : task.h
* Creation Date : 09-01-2013
* Last Modified : Fri 11 Jan 2013 12:10:41 AM EET
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
    uint32_t dependencies_count;
};


extern task_node *task_nodes_config;
extern int *task_number_config;

#endif /* TASK_H */
