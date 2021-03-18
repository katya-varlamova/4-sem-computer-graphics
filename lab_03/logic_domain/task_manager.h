#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H
#include <QPainter>
#include "algorithms.h"
typedef enum
{
    DDA, BRES_FLOAT, BRES_INT, BRES_ALIASED, VU, AUTO
} algorithm_t;

typedef enum
{
    RED, GREEN, WHITE, BACKGROUND, YELLOW, MAGENTA, CYAN, GRAY
} color_t;

typedef struct
{
    point p1, p2;
    algorithm_t algo;
    color_t color;
} line_t;

void draw_line(draw_params_t &params, const line_t &line);

#endif // TASK_MANAGER_H
