#include "task_manager.h"
#include <iostream>
static void init_painter(draw_params_t &params, const color_t &color)
{
    if (params.painter == NULL)
        return;
    if (color == RED)
        params.painter->setPen(QPen((Qt::red), 1, Qt::SolidLine));
    if (color == GREEN)
        params.painter->setPen(QPen((Qt::green), 1, Qt::SolidLine));
    if (color == WHITE)
        params.painter->setPen(QPen((Qt::white), 1, Qt::SolidLine));
    if (color == YELLOW)
        params.painter->setPen(QPen((Qt::yellow), 1, Qt::SolidLine));
    if (color == CYAN)
        params.painter->setPen(QPen((Qt::cyan), 1, Qt::SolidLine));
    if (color == GRAY)
        params.painter->setPen(QPen((Qt::lightGray), 1, Qt::SolidLine));
    if (color == MAGENTA)
        params.painter->setPen(QPen((Qt::magenta), 1, Qt::SolidLine));
    if (color == BACKGROUND)
        params.painter->setPen(QPen((QColor(50, 50, 50)), 1, Qt::SolidLine));

}

void draw_line(draw_params_t &params, const line_t &line)
{
    init_painter(params, line.color);
    if (line.algo == DDA)
        dda(params, line.p1, line.p2);
    if (line.algo == BRES_FLOAT)
        bresenham(params, line.p1, line.p2);
    if (line.algo == BRES_INT)
        bresenham_int(params, line.p1, line.p2);
    if (line.algo == AUTO)
        auto_draw(params, line.p1, line.p2);
    if (line.algo == BRES_ALIASED)
        bresenham_rem_of_grad(params, line.p1, line.p2);
    if (line.algo == VU)
        vu(params, line.p1, line.p2);

}
