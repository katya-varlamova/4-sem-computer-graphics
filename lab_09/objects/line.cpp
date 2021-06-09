#include "line.h"

Line::Line(double x1,double y1,double x2,double y2)
{
    p1 = std::shared_ptr<Point>(new Point(x1, y1));
    p2 = std::shared_ptr<Point>(new Point(x2, y2));
}

void Line::init_p1(double x,double y)
{
    p1 = std::shared_ptr<Point>(new Point(x, y));
}
void Line::init_p2(double x,double y)
{
    p2 = std::shared_ptr<Point>(new Point(x, y));
}
Point Line::get_p1()
{
    return *p1;
}
Point Line::get_p2()
{
    return *p2;
}
void Line::set_p1(Point p1)
{
    this->p1->setX(p1.getX());
    this->p1->setY(p1.getY());
}
void Line::set_p2(Point p2)
{
    this->p2->setX(p2.getX());
    this->p2->setY(p2.getY());
}
bool Line::is_p1_built()
{
    return (p1) ? true : false;
}
bool Line::is_p2_built()
{
    return (p2) ? true : false;
}
void Line::reset()
{
    p1.reset();
    p2.reset();
}
bool Line::operator==(const Line& line) const
{
    return (*(this->p1) == *(line.p1));
}
