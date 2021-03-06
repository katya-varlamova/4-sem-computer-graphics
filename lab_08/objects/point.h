#ifndef POINT_H
#define POINT_H


class Point
{
public:
    Point(int x, int y);
    int getX();
    int getY();
    void setX(int x);
    void setY(int y);
    bool operator==(const Point& point) const;
protected:
    int x, y;
};

#endif // POINT_H
