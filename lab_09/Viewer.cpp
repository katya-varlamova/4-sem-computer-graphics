#include "Viewer.h"
#include "ui_Viewer.h"

Viewer::Viewer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Viewer)
{
    ui->setupUi(this);
    mode = 0;
    cutter = std::shared_ptr<Polygone>(new Polygone());
    polygone = std::shared_ptr<Polygone>(new Polygone());
    res_polygone = std::shared_ptr<Polygone>(new Polygone());
    line_direction = 0;
}

Viewer::~Viewer()
{
    delete ui;
}
void Viewer::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Shift)
    {
        line_direction = 1;
    }
    if (event->key() == Qt::Key_Alt)
    {
        line_direction = 2;
    }
    if (mode == 0 && (event->key() == Qt::Key_Return))
    {
        if (!cutter->finishPolygone())
            print_error("can't finish cutter");
    }
    if (mode == 1 && (event->key() == Qt::Key_Return))
    {
        if (!polygone->finishPolygone())
            print_error("can't finish polygone");
    }
    repaint();
}
void Viewer::mousePressEvent(QMouseEvent *event)
{
    if (mode == 0)
    {
        if (cutter->isFinished())
            return;
        if (line_direction == 0 || cutter->getPoints().empty())
        {
            cutter->addPoint(Point(event->position().x(), event->position().y()));
        }
        else if (line_direction == 1)
        {
            if (cutter->getLines().back().is_p2_built())
                cutter->addPoint(Point(event->position().x(), cutter->getLines().back().get_p2().getY()));
            else
                cutter->addPoint(Point(event->position().x(), cutter->getLines().back().get_p1().getY()));
        }
        else
        {
            if (cutter->getLines().back().is_p2_built())
                cutter->addPoint(Point(cutter->getLines().back().get_p2().getX(), event->position().y()));
            else
                cutter->addPoint(Point(cutter->getLines().back().get_p1().getX(), event->position().y()));
        }
        line_direction = 0;
    }
    if (mode == 1)
    {
        if (polygone->isFinished())
            return;
        if (line_direction == 0 || polygone->getPoints().empty())
        {
            polygone->addPoint(Point(event->position().x(), event->position().y()));
        }
        else if (line_direction == 1)
        {
            if (polygone->getLines().back().is_p2_built())
                polygone->addPoint(Point(event->position().x(), polygone->getLines().back().get_p2().getY()));
            else
                polygone->addPoint(Point(event->position().x(), polygone->getLines().back().get_p1().getY()));
        }
        else
        {
            if (polygone->getLines().back().is_p2_built())
                polygone->addPoint(Point(polygone->getLines().back().get_p2().getX(), event->position().y()));
            else
                polygone->addPoint(Point(polygone->getLines().back().get_p1().getX(), event->position().y()));
        }
        line_direction = 0;
    }
    repaint();
}
void Viewer::print_error(QString message)
{
    ui->error_text->setTextColor(Qt::red);
    ui->error_text->setText(message);
}
void Viewer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(Qt::green);
    for (auto &line : *cutter)
    {
        if (line.is_p1_built() && line.is_p2_built())
        {
        painter.drawLine(line.get_p1().getX(),
                         line.get_p1().getY(),
                         line.get_p2().getX(),
                         line.get_p2().getY());
        }
        char *str;
        asprintf(&str, "(%.0lf, %.0lf)",
                 line.get_p1().getX(),
                 line.get_p1().getY());
        painter.setPen(QPen(Qt::white, 3, Qt::SolidLine));
        painter.drawText(QPointF(line.get_p1().getX(),
                                 line.get_p1().getY()), str);
        free(str);
        painter.setPen(Qt::green);
    }

    painter.setPen(Qt::red);
    for (auto &line : *polygone)
    {
        if (line.is_p1_built() && line.is_p2_built())
        {
        painter.drawLine(line.get_p1().getX(),
                         line.get_p1().getY(),
                         line.get_p2().getX(),
                         line.get_p2().getY());
        }
        char *str;
        asprintf(&str, "(%.0lf, %.0lf)",
                 line.get_p1().getX(),
                 line.get_p1().getY());
        painter.setPen(QPen(Qt::white, 3, Qt::SolidLine));
        painter.drawText(QPointF(line.get_p1().getX(),
                                 line.get_p1().getY()), str);
        free(str);
        painter.setPen(Qt::red);
    }
    painter.setPen(QPen(Qt::blue, 2, Qt::SolidLine));
    for (auto &line : *res_polygone)
    {
        if (line.is_p1_built() && line.is_p2_built())
        {
        painter.drawLine(line.get_p1().getX(),
                         line.get_p1().getY(),
                         line.get_p2().getX(),
                         line.get_p2().getY());
        }
    }
    painter.setPen(QPen(Qt::green, 2, Qt::SolidLine));
    for (auto &line : lumber)
        painter.drawLine(line.get_p1().getX(),
                         line.get_p1().getY(),
                         line.get_p2().getX(),
                         line.get_p2().getY());

}
Point getVector(Point &p1, Point &p2)
{
    return Point(p2.getX() - p1.getX(), p2.getY() - p1.getY());
}
double getScalarMul(double *v1, Point &v2)
{
    return v1[0] * v2.getX() + v1[1] * v2.getY();
}
int getSignOfVectorMul(Point &v1, Point &v2)
{
    return v1.getX() * v2.getY() - v1.getY() * v2.getX();
}
#include <iostream>
void getNormal(Point p1, Point p2, Point p3, double *normal)
{
    Point vect = getVector(p1, p2);
    if (vect.getX() == 0)
    {
        normal[0] = 1;
        normal[1] = 0;
    }
    else
    {
        normal[0] = -vect.getY() * 1.0 / vect.getX();
        normal[1] = 1;
    }
    Point check = getVector(p1, p3);
    if (check.getX() * normal[0] + check.getY() * normal[1] < 0)
    {
        normal[0] = - normal[0];
        normal[1] = - normal[1];
    }
}
bool checkCutter(std::vector<Point> &cutter)
{
    if (cutter.size() < 3)
        return false;

    Point vect1 = getVector(cutter[0], cutter[1]);
    Point vect2 = getVector(cutter[1], cutter[2]);

   double sign;
    if (getSignOfVectorMul(vect1, vect2) > 0)
        sign = 1;
    else
        sign = -1;

    for (size_t i = 3; i < cutter.size(); i++)
    {
        Point vecti = getVector(cutter[i - 2], cutter[i - 1]);
        Point vectj = getVector(cutter[i - 1], cutter[i]);

        if (sign * getSignOfVectorMul(vecti, vectj) < 0)
            return false;
    }
    if (sign == -1)
    {
        Point tmp(0, 0);
        for (size_t i = 0; i < cutter.size() / 2; i++)
        {
           tmp = cutter[i];
           cutter[i] = cutter[cutter.size() - 1 - i];
           cutter[cutter.size() - 1 - i] = tmp;
        }
    }
    return true;

}
bool isVisiblePoint(Point &p, Point &c1, Point &c2)
{
    // a1a3 * a1a2 > 0 === (X3-X1)(Y2-Y1)-(X2-X1)(Y3-Y1) > 0
    return (p.getX() - c1.getX()) * (c2.getY() - c1.getY()) -
            (c2.getX() - c1.getX()) * (p.getY()-c1.getY()) < 0;

}
bool findIntersection(Point &p1, Point &p2, Point &c1, Point &c2, Point &c3, Point &r)
{
    double normal[2];
    Point d = getVector(p1, p2);
    getNormal(c1, c2, c3, normal);
    Point f = c1;
    Point w = getVector(f, p1);
    double scW = getScalarMul(normal, w);
    double scD = getScalarMul(normal, d);
    if (scD == 0)
    {
            return false;
    }
    else
    {
        double t = - scW / scD;
        if (t < 0 || t > 1)
            return false;
        r.setX(p1.getX() + (p2.getX() - p1.getX()) * t);
        r.setY(p1.getY() + (p2.getY() - p1.getY()) * t);
        return true;
    }
}
bool Viewer::cut(std::vector<Point> &polygone, std::vector<Point> &cutter)
{
    // внутренние нормали ориентированы вправо (то есть обход многоугольника по часовой)
    // соответственно видимые относительно ребра точки находятся справа от ребра
    if (!checkCutter(cutter))
        return false;
std::cout << cutter.front().getX() << std::endl;
    cutter.push_back(cutter.front());
    Point f(0, 0);
    Point s(0, 0);
    for (size_t i = 0; i < cutter.size() - 1; i++)
    {
        polygone.push_back(polygone.front());
        std::vector<Point> res;
        for (size_t j = 0; j < polygone.size() - 1; j++)
        {
            if (j == 0)
            {
                f = polygone[j];
            }
            else
            {
                Point intersection(0, 0);
                if (findIntersection(polygone[j - 1], polygone[j], cutter[i], cutter[(i + 1) % cutter.size()],
                        cutter[(i + 2) % cutter.size()], intersection))
                {
                    res.push_back(intersection);
                }
            }
            s = polygone[j];
            if (isVisiblePoint(s, cutter[i], cutter[i + 1]))
            {
                res.push_back(s);
            }
        }
        if (res.empty())
            return false;
        Point intersection(0, 0);
        if (findIntersection(s, f, cutter[i], cutter[(i + 1) % cutter.size()],
                cutter[(i + 2) % cutter.size()], intersection))
        {
                res.push_back(intersection);
        }
        polygone = res;
    }
    return true;
}
bool isOnLine(double x1,double y1,double x2,double y2,double xa,double ya)
{
    if (fabs((xa - x1) * (y2 - ya) - (ya - y1) * (x2 - xa)) < 0.01)
    {
        if (((x1 < x2) && (xa < x1 || xa > x2)) || ((x1 > x2) && (xa > x1 || xa < x2)))
        {
            return false;
        }
        if (((y1 < y2) && (ya < y1 || ya > y2)) || ((y1 > y2) && (ya > y1 || ya < y2)))
        {
            return false;
        }
        return true;
    }
    else
    {
        return false;
    }
}
void Viewer::on_cut_but_clicked()
{
    std::vector<Point> c = cutter->getPoints();
    if (!checkCutter(c))
    {
        print_error("cutter must be convex");
        return;
    }
    print_error("");
    std::vector<Point> p = polygone->getPoints();
    if (cut(p, c))
    {
        Polygone res;
        for (size_t i = 0; i < p.size(); i++)
            res.addPoint(p[i]);
        res.finishPolygone();
        std::vector<Line> lines = res.getLines();

        for (size_t i = 0; i < lines.size(); i++)
        {
            res_polygone->addPoint(lines[i].get_p1());
            res_polygone->addPoint(lines[i].get_p2());
            for (size_t j = 0; j < lines.size(); j++)
            {
                if (i != j && isOnLine(lines[i].get_p1().getX(),
                                       lines[i].get_p1().getY(),
                                       lines[i].get_p2().getX(),
                                       lines[i].get_p2().getY(),
                                       lines[j].get_p1().getX(),
                                       lines[j].get_p1().getY()
                                       )
                        && isOnLine(lines[i].get_p1().getX(),
                                    lines[i].get_p1().getY(),
                                    lines[i].get_p2().getX(),
                                    lines[i].get_p2().getY(),
                                    lines[j].get_p2().getX(),
                                    lines[j].get_p2().getY()))
                {
                    lumber.push_back(lines[j]);
                }
            }

        }

    }
    repaint();
}

void Viewer::on_erase_cutter_but_clicked()
{
    cutter->clear();
    print_error("");
    repaint();
}

void Viewer::on_cutter_mode_but_clicked()
{
    mode = 0;
}

void Viewer::on_add_cutter_line_but_clicked()
{
    bool ok;
   double x = ui->cutter_x_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading cutter x");
        return;
    }
   double y = ui->cutter_y_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading cutter y");
        return;
    }
    cutter->addPoint(Point(x, y));
    repaint();
}

void Viewer::on_add_hor_cutter_line_but_clicked()
{
    bool ok;
   double delta = ui->cutter_delta_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading cutter delta");
        return;
    }
    cutter->addPoint(Point(cutter->getLines().back().get_p2().getX() + delta,
                           cutter->getLines().back().get_p2().getY()));
    repaint();
}
void Viewer::on_add_ver_cutter_line_but_clicked()
{
    bool ok;
   double delta = ui->polygone_delta_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading cutter delta");
        return;
    }
    cutter->addPoint(Point(cutter->getLines().back().get_p2().getX(),
                           cutter->getLines().back().get_p2().getY() + delta));
    repaint();
}
void Viewer::on_finish_cutter_line_but_clicked()
{
    if (!cutter->finishPolygone())
        print_error("unable to finish cutter");
}

void Viewer::on_add_polygone_line_but_clicked()
{
    bool ok;
   double x = ui->polygone_x_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading polygone x");
        return;
    }
   double y = ui->polygone_y_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading polygone y");
        return;
    }
    polygone->addPoint(Point(x, y));
    repaint();
}

void Viewer::on_add_hor_polygone_line_but_clicked()
{
    bool ok;
   double delta = ui->polygone_delta_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading polygone delta");
        return;
    }
    polygone->addPoint(Point(polygone->getLines().back().get_p2().getX() + delta,
                           polygone->getLines().back().get_p2().getY()));
    repaint();
}

void Viewer::on_add_ver_polygone_line_but_clicked()
{
    bool ok;
   double delta = ui->polygone_delta_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading polygone delta");
        return;
    }
    polygone->addPoint(Point(polygone->getLines().back().get_p2().getX(),
                              polygone->getLines().back().get_p2().getY() + delta));
    repaint();
}

void Viewer::on_finish_polygone_but_clicked()
{
    if (!polygone->finishPolygone())
        print_error("unable to finish polygone");
}

void Viewer::on_erase_polygone_but_clicked()
{
    polygone->clear();
    res_polygone->clear();
    lumber.clear();
    print_error("");
    repaint();
}

void Viewer::on_lines_mode_but_clicked()
{
    mode = 1;
}
