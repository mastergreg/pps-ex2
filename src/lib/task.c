/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : task.c
* Creation Date : 09-01-2013
* Last Modified : Tue 22 Jan 2013 10:57:34 PM EET
* Created By : Greg Liras <gregliras@gmail.com>
_._._._._._._._._._._._._._._._._._._._._.*/

#include "task.h"

#include <glib.h>

// static struct_task task_array[MAX_TASKS];
// static struct task_node task_nodes_array[MAX_TASKS];
// static uint32_t free_tasks[MAX_TASKS];
// static uint32_t free_tasks_next = 0; /* */
//
// static uint32_t ready_tasks[MAX_TASKS];
// static uint32_t ready_tasks_count = 0;


void execute(struct_task *t, int id)
{
    //*(t->value) = (*t->func)(t->args, id);
    (*(t->func))(t->args, id);
}

struct_task * set_task(void *func, void *args)
{
    struct_task *t = malloc(sizeof(struct task));
    t -> func = func;
    t -> args = args;
    return t;
}
//
// void run_task(uint32_t id)
// {
//     struct_task *t = task_array+id;
//     t->value = (*t->func)(t->args, id);
// }
//
//
//
// void tasks_initialize(void)
// {
//     int i;
//     for(i = 0; i < MAX_TASKS; i++) {
//         free_tasks[i] = i;
//         ready_tasks[i] = 0;
//     }
// }
//
// uint32_t register_task(void *func, void *args)
// {
//     uint32_t id = -1;
//     if(free_tasks_next < MAX_TASKS) {
//         id = free_tasks[free_tasks_next];
//         struct_task *t = task_array + id;
//         t -> func = func;
//         t -> args = args;
//         free_tasks_next++;
//     }
//     return id;
// }
//
// void del_task(uint32_t id)
// {
//     //struct_task *t = task_array + id;
//     /* XXX: should we free the args?
//      * How can we check if it's in the heap?
//      * free(t -> args);
//      */
//
//     if(free_tasks_next > 0) {
//         free_tasks_next--;
//         free_tasks[free_tasks_next] = id;
//     }
// }

void execute_node(struct_task_node *TASK_GRAPH_A, int id)
{
    struct_task_node *tn = TASK_GRAPH_A+id;
    int i;
    struct_task_node *child;


    execute(tn->mtask, tn->id);

    printf("ping\n");

    for(i = 0; i < tn->children_count; ++i) {
        child = &(TASK_GRAPH_A[tn->children[i]]);
        if(g_atomic_int_dec_and_test(&(child->dependencies_count))) {
            cilk_spawn execute_node(TASK_GRAPH_A, child->id);
            printf("ping\n");
        }
    }
    cilk_sync;
}
