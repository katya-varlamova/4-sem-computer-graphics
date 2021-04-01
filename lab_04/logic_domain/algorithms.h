#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QPainter>
#include <cmath>
#include <sys/time.h>
#include <initializer_list>
typedef struct p point;

typedef struct pf point_f;

struct pf
{
    double x, y;

};
struct p
{
    int x, y;
};

typedef enum
{
    CANONICAL_EQ, PARAMETRIC_EQ, BRES, MIDPOINT, AUTO
} algorithm_t;

typedef enum
{
    RED, GREEN, WHITE, BACKGROUND, YELLOW, MAGENTA, CYAN, GRAY
} color_t;

typedef struct
{
    point center;
    int radius_hor, radius_ver;
    algorithm_t algo;
    color_t color;
} ellipse_t;

typedef struct
{
    QPainter *painter;
    bool draw;
} draw_params_t;

void canonical_eq_circle(draw_params_t &params, const ellipse_t &ellipse);

void canonical_eq_ellipse(draw_params_t &params, const ellipse_t &ellipse);

void parametric_eq_circle(draw_params_t &params, const ellipse_t &ellipse);

void parametric_eq_ellipse(draw_params_t &params, const ellipse_t &ellipse);

void bresenham_circle(draw_params_t &params, const ellipse_t &ellipse);

void bresenham_ellipse(draw_params_t &params, const ellipse_t &ellipse);

void midpoint_circle(draw_params_t &params, const ellipse_t &ellipse);

void midpoint_ellipse(draw_params_t &params, const ellipse_t &ellipse);

void auto_draw(draw_params_t &params, const ellipse_t &ellipse);

#endif // ALGORITHMS_H
