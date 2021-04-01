#include "algorithms.h"
#include <iostream>
point mirror_xEQzero(point &p)
{
    point rp;
    rp.x = -p.x;
    rp.y = p.y;
    return rp;
}
point mirror_yEQzero(point &p)
{
    point rp;
    rp.x = p.x;
    rp.y = -p.y;
    return rp;
}
point mirror_yEQx(point &p)
{
    point rp;
    rp.x = p.y;
    rp.y = p.x;
    return rp;
}
void draw_point(draw_params_t &params, point &p)
{
    if (params.draw && params.painter)
        params.painter->drawPoint(p.x, p.y);
}
void mirror_and_draw_eighth(point &p, draw_params_t &params, const point &center)
{
    point points[8];
    points[0] = p;
    points[1] = mirror_yEQx(points[0]);
    points[2] = mirror_xEQzero(points[0]);
    points[3] = mirror_xEQzero(points[1]);
    for (int i = 0; i < 4; i++)
        points[i + 4] = mirror_yEQzero(points[i]);
    for (int i = 0; i < 8; i++)
    {
        points[i].x += center.x;
        points[i].y += center.y;
        draw_point(params, points[i]);
    }
}
void mirror_and_draw_forth(point &p, draw_params_t &params, const point &center)
{
    point points[4];
    points[0] = p;
    points[1] = mirror_xEQzero(points[0]);
    points[2] = mirror_yEQzero(points[0]);
    points[3] = mirror_yEQzero(points[1]);
    for (int i = 0; i < 4; i++)
    {
        points[i].x += center.x;
        points[i].y += center.y;
        draw_point(params, points[i]);
    }
}
void canonical_eq_circle(draw_params_t &params, const ellipse_t &ellipse)
{
    point p;
    for (int i = 0; i < ellipse.radius_hor / sqrt(2); i++)
    {
        p.y = round(sqrt(ellipse.radius_hor * ellipse.radius_hor - i * i));
        p.x = i;
        mirror_and_draw_eighth(p, params, ellipse.center);
    }

}
void canonical_eq_ellipse(draw_params_t &params, const ellipse_t &ellipse)
{
    int a = ellipse.radius_hor;
    int b = ellipse.radius_ver;
    int asq = a * a;
    int bsq = b * b;
    long x_vel = asq / (sqrt(asq + bsq));
    long y_vel = bsq / (sqrt(asq + bsq));
    point p;
    for (int i = 0; i < x_vel + 2; i++)
    {
        p.y = round( b * sqrt(1 - i * double(i) / asq));
        p.x = i;
        mirror_and_draw_forth(p, params, ellipse.center);
    }
    for (int i = 0; i < y_vel + 2; i++)
    {
        p.x = round( a * sqrt(1 - i * double(i) / bsq));
        p.y = i;
        mirror_and_draw_forth(p, params, ellipse.center);
    }
}

void parametric_eq_circle(draw_params_t &params, const ellipse_t &ellipse)
{
    double step = 1.0 / ellipse.radius_hor;
    point p;
    double t = 0;
    while (t < M_PI / 4 + step)
    {
        p.x = round(ellipse.radius_hor * cos(t));
        p.y = round(ellipse.radius_hor * sin(t));
        mirror_and_draw_eighth(p, params, ellipse.center);
        t += step;
    }
}
void parametric_eq_ellipse(draw_params_t &params, const ellipse_t &ellipse)
{
    double step = ellipse.radius_hor > ellipse.radius_ver ? 1.0 / ellipse.radius_hor : 1.0 / ellipse.radius_ver;
    point p;
    double t = 0;
    while (t < M_PI / 2 + step)
    {
        p.x = round(ellipse.radius_hor * cos(t));
        p.y = round(ellipse.radius_ver * sin(t));
        mirror_and_draw_forth(p, params, ellipse.center);
        t += step;
    }
}
void horisontal_step_circle(int &D, int &x)
{
    x++;
    D += 2 * x + 1;
}
void diagonal_step_circle(int &D, int &x, int &y)
{
    x++;
    y--;
    D += 2 * (x - y + 1);
}
void vertical_step_circle(int &D, int &y)
{
    y--;
    D += 1 - 2 * y;
}
void bresenham_circle(draw_params_t &params, const ellipse_t &ellipse)
{
    point p;
    p.x = 0;
    p.y = ellipse.radius_hor;
    int D = 2 * (1 - ellipse.radius_hor);
    int D1, D2;
    mirror_and_draw_eighth(p, params, ellipse.center);
    while (p.y > ellipse.radius_hor / sqrt(2))
    {
        if (D < 0)
        {
           D1 = 2 * D + 2 * p.y - 1;
           if (D1 <= 0)
               horisontal_step_circle(D, p.x);
           else
               diagonal_step_circle(D, p.x, p.y);
        }
        if (D == 0)
            diagonal_step_circle(D, p.x, p.y);
        if (D > 0)
        {
            D2 = 2 * D - 2 * p.x - 1;
            if (D2 <= 0)
                diagonal_step_circle(D, p.x, p.y);
            else
                vertical_step_circle(D, p.y);
        }
        mirror_and_draw_eighth(p, params, ellipse.center);
    }
}
void horisontal_step_ellipse(int &D, int &x, int bsq)
{
    x++;
    D += bsq * (2 * x + 1);
}
void diagonal_step_ellipse(int &D, int &x, int &y, int asq, int bsq)
{
    x++;
    y--;
    D += (2 * x + 1) * bsq + (1 - 2 * y) * asq;
}
void vertical_step_ellipse(int &D, int &y, int asq)
{
    y--;
    D += asq * (1 - 2 * y);
}
void bresenham_ellipse(draw_params_t &params, const ellipse_t &ellipse)
{
    point p;
    p.x = 0;
    p.y = ellipse.radius_ver;
    int a = ellipse.radius_hor;
    int b = ellipse.radius_ver;
    int asq = a * a;
    int bsq = b * b;

    int D = bsq - asq * (2 * b - 1);

    int D1, D2;
    mirror_and_draw_forth(p, params, ellipse.center);
    while (p.y > 0)
    {
        if (D < 0)
        {
           D1 = 2 * D + asq * (2 * p.y - 1);
           if (D1 <= 0)
               horisontal_step_ellipse(D, p.x, bsq);
           else
               diagonal_step_ellipse(D, p.x, p.y, asq, bsq);
        }
        if (D == 0)
            diagonal_step_ellipse(D, p.x, p.y, asq, bsq);
        if (D > 0)
        {
            D2 = 2 * D - bsq * (2 * p.x + 1);
            if (D2 <= 0)
                diagonal_step_ellipse(D, p.x, p.y, asq, bsq);
            else
                vertical_step_ellipse(D, p.y, asq);
        }
        mirror_and_draw_forth(p, params, ellipse.center);
    }
}

void midpoint_circle(draw_params_t &params, const ellipse_t &ellipse)
{
    point p;
    p.x = 0;
    p.y = ellipse.radius_ver;
    long r = ellipse.radius_hor;

    mirror_and_draw_forth(p, params, ellipse.center);

    int f = 1.25 - r;
    int x_lim = r / sqrt(2);

    for (int i = 0; i < x_lim; i++)
    {
        if (f < 0)
        {
            p.x++;
            f += 2 * p.x + 1;
        }
        else
        {
            p.x++;
            p.y--;
            f += 2 * p.x + 1 - 2 * p.y;
        }
        mirror_and_draw_eighth(p, params, ellipse.center);
    }
}
void midpoint_ellipse(draw_params_t &params, const ellipse_t &ellipse)
{
    point p;
    p.x = 0;
    p.y = ellipse.radius_ver;
    long a = ellipse.radius_hor;
    long b = ellipse.radius_ver;
    long asq = a * a;
    long bsq = b * b;
    long dasq = 2 * asq;
    long dbsq = 2 * bsq;

    mirror_and_draw_forth(p, params, ellipse.center);

    long f = bsq - asq * b + asq * 0.25;
    int x_lim = asq / (sqrt(asq + bsq));

    for (int i = 0; i < x_lim; i++)
    {
        if (f < 0)
        {

            p.x++;
            f += dbsq * p.x + bsq;
        }
        else
        {

            p.x++;
            p.y--;
            f += dbsq * p.x + bsq - dasq * p.y;
        }
        mirror_and_draw_forth(p, params, ellipse.center);
    }

    f += (asq - bsq) * 0.75 - (bsq * p.x + asq * p.y);

    while (p.y >= 0)
    {
        if (f > 0)
        {
            p.y--;
            f += -dasq * p.y + asq;
        }
        else
        {
            p.x++;
            p.y--;
            f += -dasq * p.y + asq + dbsq * p.x;
        }
        mirror_and_draw_forth(p, params, ellipse.center);
    }
}

void auto_draw(draw_params_t &params, const ellipse_t &ellipse)
{
    if (params.draw && params.painter)
        params.painter->drawEllipse(ellipse.center.x - ellipse.radius_hor,
                                    ellipse.center.y - ellipse.radius_ver,
                                    ellipse.radius_hor * 2,
                                    ellipse.radius_ver * 2);
}
