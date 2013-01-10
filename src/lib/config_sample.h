/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : config_sample.h
* Creation Date : 09-01-2013
* Last Modified : Thu 10 Jan 2013 11:34:33 PM EET
* Created By : Greg Liras <gregliras@gmail.com>
_._._._._._._._._._._._._._._._._._._._._.*/


#include "task.h"


#ifndef TASKS_CONFIG
#define TASKS_CONFIG


int tasks = 6;

task_node task_nodes_config[] = {
    { 
        id = 0,
        children_count = 3,
        children = {1, 2, 3},
        dependencies_count = 0,
        dependencies = NULL,
        
    },
    { 
        id = 1,
        children_count = 1,
        children = {4},
        dependencies_count = 1,
        dependencies = {0}
    },
    { 
        id = 2,
        children_count = 1,
        children = {4},
        dependencies_count = 1,
        dependencies = {0}
    },
    { 
        id = 3,
        children_count = 1,
        children = {5},
        dependencies_count = 2,
        dependencies = {1, 2}
    },
    { 
        id = 4,
        children_count = 1,
        children = {5},
        dependencies_count = 2,
        dependencies = {3, 4}
    },

};



#endif /* TASKS_CONFIG */
