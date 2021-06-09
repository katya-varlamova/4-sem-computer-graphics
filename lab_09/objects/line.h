#ifndef LINE_H
#define LINE_H

#include "point.h"
#include <memory>
class Line
{
public:
    Line() = default;
    Line(double x1,double y1,double x2,double y2);
    void init_p1(double x,double y);
    void init_p2(double x,double y);
    Point get_p1();
    Point get_p2();
    void set_p1(Point p1);
    void set_p2(Point p2);
    bool is_p1_built();
    bool is_p2_built();
    void reset();
    bool operator==(const Line& line) const;
protected:
    std::shared_ptr<Point> p1;
    std::shared_ptr<Point> p2;
};
#endif // LINE_H
