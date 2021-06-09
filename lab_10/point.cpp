#include "point.h"

point::point(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}
double point::get_x(){
    return x;
}
double point::get_y(){
    return y;
}
double point::get_z(){
    return z;
}
void point::set_x(double x)
{
    this->x = x;
}
void point::set_y(double y)
{
    this->y = y;
}
void point::set_z(double z)
{
    this->z = z;
}
void point::rotate_x(double angle)
{
    point tmp;
    tmp.x = x;
    tmp.y = y * cos(angle) + z * sin(angle);
    tmp.z = -y * sin(angle) + z * cos(angle);
    *this = tmp;
}
void point::rotate_y(double angle)
{
    point tmp;
    tmp.x = x * cos(angle) - z * sin(angle);
    tmp.y = y;
    tmp.z = x * sin(angle) + z * cos(angle);
    *this = tmp;
}
void point::rotate_z(double angle)
{
    point tmp;
    tmp.x = x * cos(angle) + y * sin(angle);
    tmp.y = -x * sin(angle) + y * cos(angle);
    tmp.z = z;
    *this = tmp;
}
void point::rotate(double ax, double ay, double az)
{
    rotate_x(ax);
    rotate_y(ay);
    rotate_z(az);
}
point2d point::project(double dx, double dy, double kx, double ky)
{
    return point2d((x + dx) * kx, (y + dy) * ky);
}
void point2d::rotate_z(double angle)
{
    point tmp;
    tmp.set_x(x * cos(angle) + y * sin(angle));
    tmp.set_y( -x * sin(angle) + y * cos(angle));
    x = round(tmp.get_x());
    y = round(tmp.get_y());
}
