/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : task_config.h
* Creation Date : 22-01-2013
* Last Modified : Tue 22 Jan 2013 06:43:19 PM EET
* Created By : Greg Liras <gregliras@gmail.com>
_._._._._._._._._._._._._._._._._._._._._.*/
#ifndef TASK_CONFIG
#define TASK_CONFIG

#include "task.h"

struct_task_node TASK_GRAPH_A[48];


/* K = 0 */

TASK_GRAPH_A[0] = {.id = 0, .children_count = 6,   \
    .children = { 1, 2, 3, 4, 5, 6 },   \
    .dependencies_count = 0, .lock = NULL, .mtask = NULL};


TASK_GRAPH_A[1] = {.id = 1, .children_count = 3,    \
    .children = { 7, 10, 13 },          \
    .dependencies_count = 2, .lock = NULL, .mtask = NULL};

TASK_GRAPH_A[2] = {.id = 2, .children_count = 3,    \
    .children = { 8, 11, 14 },          \
    .dependencies_count = 2, .lock = NULL, .mtask = NULL};

TASK_GRAPH_A[3] = {.id = 3, .children_count = 3,    \
    .children = { 9, 12, 15 },          \
    .dependencies_count = 2, .lock = NULL, .mtask = NULL};

TASK_GRAPH_A[4] = {.id = 4, .children_count = 3,    \
    .children = { 7, 8, 9 },            \
    .dependencies_count = 2, .lock = NULL, .mtask = NULL};

TASK_GRAPH_A[5] = {.id = 5, .children_count = 3,    \
    .children = { 10, 11, 12 },         \
    .dependencies_count = 2, .lock = NULL, .mtask = NULL};

TASK_GRAPH_A[6] = {.id = 6, .children_count = 3,    \
    .children = { 13, 14, 15 },         \
    .dependencies_count = 2, .lock = NULL, .mtask = NULL};

TASK_GRAPH_A[7] = {.id = 7, .children_count = 1,    \
    .children = { 16 },                 \
    .dependencies_count = 2, .lock = NULL, .mtask = NULL};

TASK_GRAPH_A[8] = {.id = 8, .children_count = 1,    \
    .children = { 17 },                 \
    .dependencies_count = 2, .lock = NULL, .mtask = NULL};

TASK_GRAPH_A[9] = {.id = 9, .children_count = 1,    \
    .children = { 18 },                 \
    .dependencies_count = 2, .lock = NULL, .mtask = NULL};

TASK_GRAPH_A[10] = {.id = 10, .children_count = 1,    \
    .children = { 19 },                 \
    .dependencies_count = 2, .lock = NULL, .mtask = NULL};

TASK_GRAPH_A[11] = {.id = 11, .children_count = 1,    \
    .children = { 21 },                 \
    .dependencies_count = 2, .lock = NULL, .mtask = NULL};

TASK_GRAPH_A[12] = {.id = 12, .children_count = 1,    \
    .children = { 22 },                 \
    .dependencies_count = 2, .lock = NULL, .mtask = NULL};

TASK_GRAPH_A[13] = {.id = 13, .children_count = 1,    \
    .children = { 20 },                 \
    .dependencies_count = 2, .lock = NULL, .mtask = NULL};

TASK_GRAPH_A[14] = {.id = 14, .children_count = 1,    \
    .children = { 23 },                 \
    .dependencies_count = 2, .lock = NULL, .mtask = NULL};

TASK_GRAPH_A[15] = {.id = 15, .children_count = 1,    \
    .children = { 24 },                 \
    .dependencies_count = 2, .lock = NULL, .mtask = NULL};

/* K = 1 */

TASK_GRAPH_A[16] = {.id = 16, .children_count = 4,             \
    .children = { 17, 18, 19, 20 },                 \
    .dependencies_count = 1, .lock = NULL, .mtask = NULL};


TASK_GRAPH_A[17] = {.id = 17, .children_count = 4,             \
    .children = { 21, 22, 23, 24, 25},                 \
    .dependencies_count = 2, .lock = NULL, .mtask = NULL};

TASK_GRAPH_A[18] = {.id = 18, .children_count = 4,             \
    .children = { 21, 22, 23, 24, 25},                 \
    .dependencies_count = 2, .lock = NULL, .mtask = NULL};

TASK_GRAPH_A[19] = {.id = 19, .children_count = 4,             \
    .children = { 21, 22, 23, 24, 25},                 \
    .dependencies_count = 2, .lock = NULL, .mtask = NULL};

TASK_GRAPH_A[20] = {.id = 20, .children_count = 4,             \
    .children = { 21, 22, 23, 24, 25},                 \
    .dependencies_count = 2, .lock = NULL, .mtask = NULL};

TASK_GRAPH_A[21] = {.id = 21, .children_count = 1,             \
    .children = { 25 },                             \
    .dependencies_count = 3, .lock = NULL, .mtask = NULL};

TASK_GRAPH_A[22] = {.id = 22, .children_count = 1,             \
    .children = { 26 },                             \
    .dependencies_count = 3, .lock = NULL, .mtask = NULL};

TASK_GRAPH_A[23] = {.id = 23, .children_count = 1,             \
    .children = { 27 },                             \
    .dependencies_count = 3, .lock = NULL, .mtask = NULL};

TASK_GRAPH_A[24] = {.id = 24, .children_count = 1,             \
    .children = { 28 },                             \
    .dependencies_count = 3, .lock = NULL, .mtask = NULL};

/* K = 2 */

TASK_GRAPH_A[25] = {.id = 25, .children_count = 2,             \
    .children = { 26, 27 },                             \
    .dependencies_count = 1, .lock = NULL, .mtask = NULL};

TASK_GRAPH_A[26] = {.id = 26, .children_count = 1,             \
    .children = { 28 },                             \
    .dependencies_count = 2, .lock = NULL, .mtask = NULL};

TASK_GRAPH_A[27] = {.id = 27, .children_count = 1,             \
    .children = { 28 },                             \
    .dependencies_count = 2, .lock = NULL, .mtask = NULL};

TASK_GRAPH_A[28] = {.id = 28, .children_count = 1,             \
    .children = { 28 },                             \
    .dependencies_count = 3, .lock = NULL, .mtask = NULL};
#endif /* TASK_CONFIG */

