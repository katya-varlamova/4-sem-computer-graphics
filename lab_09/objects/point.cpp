#include "point.h"

Point::Point(double x, double y)
{
    this->x = x;
    this->y = y;
}
double Point::getX()
{
    return x;
}
double Point::getY()
{
    return y;
}
void Point::setX(double x)
{
    this->x = x;
}
void Point::setY(double y)
{
    this->y = y;
}
bool Point::operator==(const Point& point) const
{
    return (point.x == this->x && point.y == this->y);
}
