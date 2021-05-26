#include "point.h"

Point::Point(int x, int y)
{
    this->x = x;
    this->y = y;
}
int Point::getX()
{
    return x;
}
int Point::getY()
{
    return y;
}
void Point::setX(int x)
{
    this->x = x;
}
void Point::setY(int y)
{
    this->y = y;
}
bool Point::operator==(const Point& point) const
{
    return (point.x == this->x && point.y == this->y);
}
