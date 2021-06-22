#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QPainter>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <initializer_list>
typedef struct p point;

typedef struct pf point_f;

struct pf
{
    double x, y;

    point_f& operator = (const std::initializer_list<double> &list);

    point_f& operator= (const point right);

};
struct p
{
    int x, y;

    point& operator = (const std::initializer_list<int> &list);

    point& operator = (const point_f right);

};

typedef struct
{
    QPainter *painter;
    int64_t time;
    int num_of_grad;
    int len;
    bool draw;
} draw_params_t;

void dda(draw_params_t &params, const point &p_b, const point &p_e);

void bresenham(draw_params_t &params, const point &p_b, const point &p_e);

void bresenham_int(draw_params_t &params, const point &p_b, const point &p_e);

void auto_draw(draw_params_t &params, const point &p_b, const point &p_e);

void bresenham_rem_of_grad(draw_params_t &params, const point &p_b, const point &p_e);

void vu(draw_params_t &params, const point &p_b, const point &p_e);

#endif // ALGORITHMS_H
