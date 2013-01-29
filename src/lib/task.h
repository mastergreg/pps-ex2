/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : task.h
* Creation Date : 09-01-2013
* Last Modified : Tue 29 Jan 2013 02:52:44 PM EET
* Created By : Greg Liras <gregliras@gmail.com>
_._._._._._._._._._._._._._._._._._._._._.*/
#ifndef TASK_H
#define TASK_H

#ifndef MAX_TASKS
#define MAX_TASKS 42
#endif /* MAX_TASKS */

#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <glib.h>

struct task {
    void *(*func)(void *, int);
    void *args;
    void *value;
};

typedef struct task struct_task;
void execute(struct_task *, int id);


struct task_node {
    unsigned int id;
    unsigned int children_count;
    gint dependencies_count;
    struct_task *mtask;
    unsigned int *children;
};
typedef struct task_node struct_task_node;


struct_task * set_task(void *func, void *args);
void execute_node(struct_task_node *TASK_GRAPH_A, int id);

//void tasks_initialize(void);

#endif /* TASK_H */
