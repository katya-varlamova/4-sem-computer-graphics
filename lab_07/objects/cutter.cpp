#include "cutter.h"

Cutter::Cutter(int x1, int y1, int x2, int y2)
{
    left_top = std::shared_ptr<Point>(new Point(x1, y1));
    right_bottom = std::shared_ptr<Point>(new Point(x2, y2));
}

void Cutter::init_left_top(int x, int y)
{
    left_top = std::shared_ptr<Point>(new Point(x, y));
}
void Cutter::init_right_bottom(int x, int y)
{
    right_bottom = std::shared_ptr<Point>(new Point(x, y));
}

bool Cutter::is_left_built()
{
    return (left_top) ? true : false;
}
bool Cutter::is_right_built()
{
    return (right_bottom) ? true : false;
}
void Cutter::reset()
{
    left_top.reset();
    right_bottom.reset();
}
Point Cutter::get_left_top()
{
    return *left_top;
}
Point Cutter::get_right_bottom()
{
    return *right_bottom;
}
void Cutter::set_left_top(Point left_top)
{
    this->left_top->setX(left_top.getX());
    this->left_top->setY(left_top.getY());
}
void Cutter::set_right_bottom(Point right_bottom)
{
    this->right_bottom->setX(right_bottom.getX());
    this->right_bottom->setY(right_bottom.getY());
}
