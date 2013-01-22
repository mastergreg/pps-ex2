/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : task.h
* Creation Date : 09-01-2013
* Last Modified : Tue 22 Jan 2013 07:46:42 PM EET
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
void execute(struct_task *);


struct task_node {
    unsigned int id;
    unsigned int children_count;
    unsigned int *children;
    //volatile gint lock;
    void  *lock;
    unsigned int dependencies_count;
    struct_task *mtask;
};
typedef struct task_node struct_task_node;


struct_task * set_task(void *func, void *args);

//void tasks_initialize(void);

#endif /* TASK_H */
