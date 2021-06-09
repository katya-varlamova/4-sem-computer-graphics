#ifndef POINT_H
#define POINT_H
#include <math.h>
class point2d
{
public:
    point2d(double x = 0, double y = 0)
    {
        this->x = round(x);
        this->y = round(y);
        xd = x;
        yd = y;

    }
    void rotate_z(double az);
    int x, y;
    double xd, yd;
};
class point
{
public:
    point(double x = 0, double y = 0, double z = 0);
    double get_x();
    double get_y();
    double get_z();
    void set_x(double x);
    void set_y(double y);
    void set_z(double z);
    void rotate(double ax, double ay, double az);
    point2d project(double dx, double dy, double kx, double ky);
private:
    void rotate_x(double ax);
    void rotate_y(double ay);
    void rotate_z(double az);
    double x, y, z;
};

#endif // POINT_H
