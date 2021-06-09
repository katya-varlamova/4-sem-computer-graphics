#include "polygone.h"
Polygone::Polygone()
{
    finished = false;
}
void Polygone::addPoint(Point point)
{
    if (lines.empty())
    {
        lines.push_back(Line());
        lines.back().init_p1(point.getX(), point.getY());
        return;
    }
    if (lines.back().is_p1_built() && !lines.back().is_p2_built())
    {
        lines.back().init_p2(point.getX(),
                             point.getY());
        return;
    }
    if (lines.back().is_p1_built() && lines.back().is_p2_built())
    {
        lines.push_back(Line(lines.back().get_p2().getX(),
                             lines.back().get_p2().getY(),
                             point.getX(),
                             point.getY()));
        return;
    }
    return;
}
bool Polygone::finishPolygone()
{
    if (lines.empty())
        return false;
    if (lines.back().is_p1_built() && !lines.back().is_p2_built())
        return false;
    if (lines.back() == lines.front())
        return false;
    lines.push_back(Line(lines.back().get_p2().getX(),
                         lines.back().get_p2().getY(),
                         lines.front().get_p1().getX(),
                         lines.front().get_p1().getY()));
    finished = true;
    return true;
}
std::vector<Point> Polygone::getPoints()
{
    std::vector<Point> points;
    for (auto &line : lines)
        points.push_back(line.get_p1());
    return points;
}
std::vector<Line>::iterator Polygone::begin()
{
    return lines.begin();
}
std::vector<Line>::iterator Polygone::end()
{
    return lines.end();
}
bool Polygone::isFinished()
{
    return finished;
}
void Polygone::clear()
{
    lines.clear();
    finished = false;
}
std::vector<Line> Polygone::getLines()
{
    return lines;
}
std::ostream& operator<<(std::ostream& os, Polygone& p)
{
    for (auto &line : p.getLines())
        std::cout << line.get_p1().getX() << " " << line.get_p1().getY() << " - " <<
                     line.get_p2().getX() << " " << line.get_p2().getY() << std::endl;

    return os;
}
