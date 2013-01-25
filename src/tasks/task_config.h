/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : task_config.h
* Creation Date : 22-01-2013
* Last Modified : Wed 23 Jan 2013 03:59:41 PM EET
* Created By : Greg Liras <gregliras@gmail.com>
_._._._._._._._._._._._._._._._._._._._._.*/
#ifndef TASK_CONFIG
#define TASK_CONFIG

#include "task.h"

struct_task_node TASK_GRAPH_A[30] = {
    {
        .id = 0, .children_count = 6,
        .dependencies_count = 0, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 1, 2, 3, 4, 5, 6 }
    },
    {
        .id = 1, .children_count = 3,
        .dependencies_count = 1, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 7, 10, 13 }
    },
    {
        .id = 2, .children_count = 3,
        .dependencies_count = 1, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 8, 11, 14 }
    },
    {
        .id = 3, .children_count = 3,
        .dependencies_count = 1, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 9, 12, 15 }
    },
    {
        .id = 4, .children_count = 3,
        .dependencies_count = 1, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 7, 8, 9 }
    },
    {
        .id = 5, .children_count = 3,
        .dependencies_count = 1, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 10, 11, 12 }
    },
    {
        .id = 6, .children_count = 3,
        .dependencies_count = 1, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 13, 14, 15 }
    },
    {
        .id = 7, .children_count = 1,
        .dependencies_count = 2, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 16 }
    },
    {
        .id = 8, .children_count = 1,
        .dependencies_count = 2, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 17 }
    },
    {
        .id = 9, .children_count = 1,
        .dependencies_count = 2, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 18 }
    },
    {
        .id = 10, .children_count = 1,
        .dependencies_count = 2, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 19 }
    },
    {
        .id = 11, .children_count = 1,
        .dependencies_count = 2, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 21 }
    },
    {
        .id = 12, .children_count = 1,
        .dependencies_count = 2, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 22 }
    },
    {
        .id = 13, .children_count = 1,
        .dependencies_count = 2, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 20 }
    },
    {
        .id = 14, .children_count = 1,
        .dependencies_count = 2, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 23 }
    },
    {
        .id = 15, .children_count = 1,
        .dependencies_count = 2, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 24 }
    },

    /* K = 1 */
    {
        .id = 16, .children_count = 4,
        .dependencies_count = 1, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 17, 18, 19, 20 }
    },
    {
        .id = 17, .children_count = 2,
        .dependencies_count = 2, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 21, 23 }
    },
    {
        .id = 18, .children_count = 2,
        .dependencies_count = 2, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 22, 24 }
    },
    {
        .id = 19, .children_count = 2,
        .dependencies_count = 2, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 21, 22, }
    },
    {
        .id = 20, .children_count = 2,
        .dependencies_count = 2, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 23, 24 }
    },
    {
        .id = 21, .children_count = 1,
        .dependencies_count = 3, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 25 }
    },
    {
        .id = 22, .children_count = 1,
        .dependencies_count = 3, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 26 }
    },
    {
        .id = 23, .children_count = 1,
        .dependencies_count = 3, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 27 }
    },
    {
        .id = 24, .children_count = 1,
        .dependencies_count = 3, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 28 }
    },

    /* K = 2 */
    {
        .id = 25, .children_count = 2,
        .dependencies_count = 1, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 26, 27 }
    },
    {
        .id = 26, .children_count = 1,
        .dependencies_count = 2, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 28 }
    },
    {
        .id = 27, .children_count = 1,
        .dependencies_count = 2, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 28 }
    },
    {
        .id = 28, .children_count = 1,
        .dependencies_count = 3, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) { 29 }
    },
    {
        .id = 29, .children_count = 0,
        .dependencies_count = 1, .lock = NULL, .mtask = NULL,
        .children =  (unsigned int []) {}
    }
};
#endif /* TASK_CONFIG */

