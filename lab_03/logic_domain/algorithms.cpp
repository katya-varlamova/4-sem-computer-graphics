#include "algorithms.h"
#include <iostream>
#define EPS 1e-6
point_f& point_f::operator = (const std::initializer_list<double> &list)
{
   x = *list.begin();
   y = *(list.begin() + 1);
   return *this;
}
point_f& point_f::operator = (const point right)
{
    x = right.x;
    y = right.y;
    return *this;
}
point& point::operator = (const std::initializer_list<int> &list)
{
   x = *list.begin();
   y = *(list.begin() + 1);
   return *this;
}
point& point::operator = (const point_f right)
{
    x = round(right.x);
    y = round(right.y);
    return *this;
}
typedef enum
{
    x, y
} differ_t;
void draw_point(draw_params_t &params, int x, int y)
{
    if (params.draw && params.painter)
        params.painter->drawPoint(x, y);
}
void draw_line(draw_params_t &params, const point& p1, const point &p2)
{
    if (params.draw && params.painter)
        params.painter->drawLine(p1.x, p1.y, p2.x, p2.y);
}
void set_intens(draw_params_t &params, double intens)
{
    if (params.draw && params.painter)
    {
        QColor c = params.painter->pen().color();
        c.setAlpha(intens);
        params.painter->setPen(c);
    }
}
int sign(double c)
{
    if (c < -EPS)
        return -1;
    else
        return 1;
}
static int are_differ(const point &p1, const point &p2, differ_t dif)
{
    if (dif == x)
        return p1.x != p2.x ? 1 : 0;
    else
        return p1.y != p2.y ? 1 : 0;
}
void dda(draw_params_t &params, const point &p_b, const point &p_e)
{
    struct timeval tv_start, tv_stop;
    differ_t dif;
    point_f cur;
    point cur_rounded, prev_rounded;

    params.time = 0;
    params.num_of_grad = 0;
    if (!are_differ(p_b, p_e, x) && !are_differ(p_b, p_e, y))
    {
        draw_point(params, round(p_b.x), round(p_b.y));
        params.len = 1;
        return;
    }
    gettimeofday(&tv_start, NULL);
    double dx = abs(p_e.x - p_b.x);
    double dy = abs(p_e.y - p_b.y);
    cur = p_b;
    if (dx >= dy)
    {
        params.len = abs(p_e.x - p_b.x);
        dx = sign(p_e.x - p_b.x);
        dy = 1.0 * (p_e.y - p_b.y) / (1.0 * params.len);
        dif = y;
    }
    else
    {
        params.len = abs(p_e.y - p_b.y);
        dy = sign(p_e.y - p_b.y);
        dx = 1.0 * (p_e.x - p_b.x) / (1.0 * params.len);
        dif = x;
    }
    cur_rounded = cur;
    prev_rounded = cur_rounded;
    for (int i = 0; i < params.len; i++)
    {
        if (are_differ(cur_rounded, prev_rounded, dif))
            params.num_of_grad++;
        draw_point(params, cur_rounded.x, cur_rounded.y);

        cur.x += dx;
        cur.y += dy;
        prev_rounded = cur_rounded;
        cur_rounded = cur;
    }
    gettimeofday(&tv_stop, NULL);
    params.time += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
}
void bresenham(draw_params_t &params, const  point &p_b, const point &p_e)
{
    struct timeval tv_start, tv_stop;
    params.time = 0;
    params.num_of_grad = 0;
    if (!are_differ(p_b, p_e, x) && !are_differ(p_b, p_e, y))
    {
        draw_point(params, round(p_b.x), round(p_b.y));
        params.len = 1;
        return;
    }
    gettimeofday(&tv_start, NULL);
    point cur;
    double dx, dy, m, f;
    bool fl = 0;
    double sx, sy;
    dx = p_e.x - p_b.x;
    dy = p_e.y - p_b.y;
    sx = sign(dx);
    sy = sign(dy);
    dx = abs(dx);
    dy = abs(dy);
    if (dy >= dx)
    {
        double tmp = dx;
        dx = dy;
        dy = tmp;
        fl = 1;
    }
    m = dy / dx;
    f = m - 0.5;
    cur = p_b;
    params.len = int(dx);
    for (int i = 0; i < params.len; i++)
    {
        draw_point(params, cur.x, cur.y);
        if (f >= 0.)
        {
            if (fl == 1)
                cur.x += sx;
            else
                cur.y += sy;
            params.num_of_grad++;
            f -= 1.;
        }
        if (f < 0.)
        {
            if (fl == 1)
                cur.y += sy;
            else
                cur.x += sx;
        }
        f += m;
    }
    gettimeofday(&tv_stop, NULL);
    params.time += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
}

void bresenham_int(draw_params_t &params, const point &p_b, const point &p_e)
{
    struct timeval tv_start, tv_stop;
    params.time = 0;
    params.num_of_grad = 0;
    if (!are_differ(p_b, p_e, x) && !are_differ(p_b, p_e, y))
    {
        draw_point(params, round(p_b.x), round(p_b.y));
        params.len = 1;
        return;
    }
    gettimeofday(&tv_start, NULL);
    point cur;
    int dx, dy;
    bool fl = 0;
    int sx, sy, f;
    dx = p_e.x - p_b.x;
    dy = p_e.y - p_b.y;
    sx = sign(dx);
    sy = sign(dy);
    dx = abs(dx);
    dy = abs(dy);
    if (dy >= dx)
    {
        int tmp = dx;
        dx = dy;
        dy = tmp;
        fl = 1;
    }
    f = 2 * dy - dx;
    cur = p_b;
    params.len = dx;
    for (int i = 0; i < params.len; i++)
    {
        draw_point(params, cur.x, cur.y);
        if (f >= 0)
        {
            params.num_of_grad++;
            if (fl == 1)
                cur.x += sx;
            else
                cur.y += sy;
            f -= 2 * dx;
        }
        if (f < 0)
        {
            if (fl == 1)
                cur.y += sy;
            else
                cur.x += sx;
        }
        f += 2 * dy;
    }
    gettimeofday(&tv_stop, NULL);
    params.time += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
}
void auto_draw(draw_params_t &params, const point &p_b, const point &p_e)
{
    params.len = 0;
    params.num_of_grad = 0;
    params.time = 0;
    draw_line(params, p_b, p_e);
}
void bresenham_rem_of_grad(draw_params_t &params, const point &p_b, const point &p_e)
{
    struct timeval tv_start, tv_stop;
    params.time = 0;
    params.num_of_grad = 0;
    double I = 255;
    if (!are_differ(p_b, p_e, x) && !are_differ(p_b, p_e, y))
    {
        draw_point(params, round(p_b.x), round(p_b.y));
        return;
    }
    gettimeofday(&tv_start, NULL);
    point cur;
    double dx, dy, m, f, w;
    bool fl = 0;
    int sx, sy;
    dx = p_e.x - p_b.x;
    dy = p_e.y - p_b.y;
    sx = sign(dx);
    sy = sign(dy);
    dx = abs(dx);
    dy = abs(dy);

    if (dy >= dx)
    {
        double tmp = dx;
        dx = dy;
        dy = tmp;
        fl = 1;
    }
    m = dy / dx * I;
    f = I / 2;
    w = I - m;
    cur = p_b;
    params.len = int(dx);
    for (int i = 0; i < params.len; i++)
    {
        set_intens(params, round(f));
        draw_point(params, cur.x, cur.y);
        if (f <= w)
        {
            if (fl == 1)
                cur.y += sy;
            else
                cur.x += sx;
            f += m;
        }
        else
        {
            params.num_of_grad++;
            cur.x += sx;
            cur.y += sy;
            f -= w;
        }
    }
    gettimeofday(&tv_stop, NULL);
    params.time += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
}
void swap(point &p1, point &p2)
{
    point tmp = p1;
    p1 = p2;
    p2 = tmp;
}
void vu(draw_params_t &params, const point &p_b, const point &p_e)
{
    struct timeval tv_start, tv_stop;
    params.time = 0;
    params.num_of_grad = 0;
    point b = p_b, e = p_e;
    double I = 255;
    if (!are_differ(p_b, p_e, x) && !are_differ(p_b, p_e, y))
    {
        draw_point(params, round(p_b.x), round(p_b.y));
        return;
    }
    gettimeofday(&tv_start, NULL);
    double dx, dy;
    int sx, sy;
    dx = e.x - b.x;
    dy = e.y - b.y;
    dx = abs(dx);
    dy = abs(dy);

    bool fl = 0;
    if (dy >= dx)
    {
        double tmp = dx;
        dx = dy;
        dy = tmp;
        fl = 1;
    }
    if ((fl && e.y - b.y < 0) || (!fl && e.x - b.x < 0))
        swap(b, e);
    sx = sign(e.x - b.x);
    sy = sign(e.y - b.y);
    double m = dy / dx;
    double ch;
    int st;
    if (fl == 0)
    {
        ch = b.y;
        st = b.x;
    }
    else
    {
        ch = b.x;
        st = b.y;
    }
    int ipart = trunc(ch), ipart_prev;
    double fpart;
    params.len = int(dx);
    for (int i = 0; i < dx; i++)
    {
        fpart = ch - ipart;
        if (fl == 0)
        {
            set_intens(params, I * (1.0 - fpart));
            draw_point(params, st, ipart);
            set_intens(params, I * fpart);
            draw_point(params, st, ipart + 1);
            ipart_prev = ipart;
            st += sx;
            ch += sy * m;
        }
        else
        {
            set_intens(params, I * (1.0 - fpart));
            draw_point(params, ipart, st);
            set_intens(params, I * fpart);
            draw_point(params, ipart + 1, st);
            ipart_prev = ipart;
            ch += sx * m;
            st += sy;
        }
        ipart = trunc(ch);
        if (ipart != ipart_prev)
            params.num_of_grad++;
    }
    gettimeofday(&tv_stop, NULL);
    params.time += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
}
