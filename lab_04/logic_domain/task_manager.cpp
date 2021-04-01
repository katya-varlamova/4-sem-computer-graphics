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

void draw_ellipse(draw_params_t &params, const ellipse_t &ellipse)
{
    init_painter(params, ellipse.color);
    if (ellipse.algo == CANONICAL_EQ)
    {
        if (ellipse.radius_hor == ellipse.radius_ver)
            canonical_eq_circle(params, ellipse);
        else
            canonical_eq_ellipse(params, ellipse);
    }
    if (ellipse.algo == PARAMETRIC_EQ)
    {
        if (ellipse.radius_hor == ellipse.radius_ver)
            parametric_eq_circle(params, ellipse);
        else
            parametric_eq_ellipse(params, ellipse);
    }
    if (ellipse.algo == BRES)
    {
        if (ellipse.radius_hor == ellipse.radius_ver)
            bresenham_circle(params, ellipse);
        else
            bresenham_ellipse(params, ellipse);
    }
    if (ellipse.algo == MIDPOINT)
    {
        if (ellipse.radius_hor == ellipse.radius_ver)
            midpoint_circle(params, ellipse);
        else
            midpoint_ellipse(params, ellipse);
    }
    if (ellipse.algo == AUTO)
        auto_draw(params, ellipse);
}
