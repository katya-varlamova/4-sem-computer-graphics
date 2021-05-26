#ifndef POLYGONE_H
#define POLYGONE_H

#include "line.h"
#include <vector>
class Polygone
{
public:
    Polygone();
    void addPoint(Point point);
    bool finishPolygone();
    bool isFinished();
    std::vector<Line>::iterator begin();
    std::vector<Line>::iterator end();
    std::vector<Line> getLines();
    std::vector<Point> getPoints();
    void clear();
protected:
    std::vector<Line> lines;
    bool finished;
};

#endif // POLYGONE_H
