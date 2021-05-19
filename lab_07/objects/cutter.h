#ifndef CUTTER_H
#define CUTTER_H
#include "point.h"
#include <memory>
class Cutter
{
public:
    Cutter() = default;
    Cutter(int x1, int y1, int x2, int y2);
    void init_left_top(int x, int y);
    void init_right_bottom(int x, int y);
    bool is_left_built();
    bool is_right_built();
    Point get_left_top();
    Point get_right_bottom();
    void set_left_top(Point left_top);
    void set_right_bottom(Point right_bottom);
    void reset();
protected:
    std::shared_ptr<Point> left_top;
    std::shared_ptr<Point> right_bottom;
};

#endif // CUTTER_H
