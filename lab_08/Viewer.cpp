#include "Viewer.h"
#include "ui_Viewer.h"

Viewer::Viewer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Viewer)
{
    ui->setupUi(this);
    mode = 0;
    cutter = std::shared_ptr<Polygone>(new Polygone());
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
    if (event->key() == Qt::Key_Backspace)
    {
        line_direction = 2;
    }
    if (mode == 0 && (event->key() == Qt::Key_Return))
    {
        if (!cutter->finishPolygone())
            print_error("can't finish cutter");
    }
    repaint();
}
void Viewer::mousePressEvent(QMouseEvent *event)
{
    if (mode == 0)
    {
        if (line_direction == 0 || cutter->getPoints().empty())
            cutter->addPoint(Point(event->position().x(), event->position().y()));
        else if (line_direction == 1)
            cutter->addPoint(Point(event->position().x(), cutter->getPoints().back().getY()));
        else
            cutter->addPoint(Point(cutter->getPoints().back().getX(), event->position().y()));

    }
    if (mode == 1)
    {
        if (lines.empty() || (lines.back()->is_p1_built() && lines.back()->is_p2_built()))
        {
            lines.push_back(std::shared_ptr<Line>(new Line()));
            lines.back()->init_p1(event->position().x(), event->position().y());
            print_error("");
        }
        else
        {
            if (line_direction == 0)
                lines.back()->init_p2(event->position().x(), event->position().y());
            if (line_direction == 1)
            {
                lines.back()->init_p2(event->position().x(), lines.back()->get_p1().getY());
            }
            if (line_direction == 2)
                lines.back()->init_p2(lines.back()->get_p1().getX(), event->position().y());
            line_direction = 0;
            print_error("");
        }
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
        asprintf(&str, "(%d, %d)",
                 line.get_p1().getX(),
                 line.get_p1().getY());
        painter.setPen(QPen(Qt::white, 3, Qt::SolidLine));
        painter.drawText(QPointF(line.get_p1().getX(),
                                 line.get_p1().getY()), str);
        free(str);
        painter.setPen(Qt::green);
    }

    painter.setPen(Qt::red);
    for (auto &line : lines)
    {
        if (line->is_p1_built() && line->is_p2_built())
        {
            painter.drawLine(line->get_p1().getX(),
                             line->get_p1().getY(),
                             line->get_p2().getX(),
                             line->get_p2().getY());
        }
        if (line->is_p1_built() && !line->is_p2_built())
            painter.drawPoint(line->get_p1().getX(),
                             line->get_p1().getY());
    }
    painter.setPen(Qt::yellow);
    for (auto &line : res_lines)
    {
        if (line->is_p1_built() && line->is_p2_built())
        {
            painter.drawLine(line->get_p1().getX(),
                             line->get_p1().getY(),
                             line->get_p2().getX(),
                             line->get_p2().getY());
        }
        if (line->is_p1_built() && !line->is_p2_built())
            painter.drawPoint(line->get_p1().getX(),
                             line->get_p1().getY());
    }
//    painter.setPen(Qt::blue);
//    for (auto point : points)
//        painter.drawPoint(point.getX(), point.getY());
}
void Viewer::on_add_line_but_clicked()
{
    bool ok;
    int x1 = ui->line_x1_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading x1");
        return;
    }
    int x2 = ui->line_x2_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading x2");
        return;
    }
    int y1 = ui->line_y1_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading y1");
        return;
    }
    int y2 = ui->line_y2_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading y2");
        return;
    }
    print_error("");
    lines.push_back(std::shared_ptr<Line>(new Line(x1, y1, x2, y2)));
    repaint();
}

void Viewer::on_add_hor_line_but_clicked()
{
    bool ok;
    int x1 = ui->line_x1_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading x1");
        return;
    }
    int y1 = ui->line_y1_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading y1");
        return;
    }
    int delta = ui->line_delta_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading delta");
        return;
    }
    print_error("");
    lines.push_back(std::shared_ptr<Line>(new Line(x1, y1, x1 + delta, y1)));
    repaint();
}

void Viewer::on_add_ver_line_but_clicked()
{
    bool ok;
    int x1 = ui->line_x1_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading x1");
        return;
    }
    int y1 = ui->line_y1_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading y1");
        return;
    }
    int delta = ui->line_delta_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading delta");
        return;
    }
    print_error("");
    lines.push_back(std::shared_ptr<Line>(new Line(x1, y1, x1, y1 + delta)));
    repaint();
}
void fill_code(Point p, int *t, int xl, int yt, int xr, int yb)
{
    for (int i = 0; i < 4; i++)
        t[i] = 0;
    if (p.getX() < xl)
        t[3] = 1;
    if (p.getX() > xr)
        t[2] = 1;
    if (p.getY() < yb)
        t[1] = 1;
    if (p.getY() > yt)
        t[0] = 1;
}
int sum_code(int *t)
{
    int s = 0;
    for (int i = 0; i < 4; i++)
        s += t[i];
    return s;
}
int mul_codes(int *t1, int *t2)
{
    int s = 0;
    for (int i = 0; i < 4; i++)
        s += t1[i] * t2[i];
    return s;
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
//    std::cout << "normal_bef = " << normal.getX() << " " << normal.getY() << std::endl;
    Point check = getVector(p1, p3);
//    std::cout << "check = " << check.getX() << " " << check.getY() << std::endl;
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

    int sign;
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
//    if (sign < 0)
//    {
//        Point tmp(0, 0);
//        for (size_t i = 0; i < cutter.size() / 2; i++)
//        {
//            tmp = cutter[i];
//            cutter[i] = cutter[cutter.size() - 1 - i];
//            cutter[cutter.size() - 1 - i] = tmp;
//        }

//    }
    return true;

}
bool Viewer::cut(Point p1, Point p2, std::vector<Point> cutter, Point *r)
{
//    std::cout << p1.getX() << " " << p1.getY() << " " << p2.getX() << " " << p2.getY() << std::endl;
    if (!checkCutter(cutter))
        return false;
    Point d = getVector(p1, p2);
//    std::cout << "dir = " << d.getX() << " " << d.getY() << std::endl;
    double tn = 0, tv = 1;
    for (size_t i = 0; i < cutter.size(); i++)
    {
        double normal[2];
        getNormal(cutter[i], cutter[(i + 1) % cutter.size()], cutter[(i + 2) % cutter.size()], normal);
//        std::cout << "normal = " << normal[0] << " " << normal[1] << std::endl;
        Point f = cutter[i];
//        std::cout << "f = " << f.getX() << " " << f.getY() << std::endl;
        Point w = getVector(f, p1);
//        std::cout << "w = " << w.getX() << " " << w.getY() << std::endl;
        double scW = getScalarMul(normal, w);
//        std::cout << "scw = " << scW << std::endl;
        double scD = getScalarMul(normal, d);
        if (scD == 0)
        {
            if (scW < 0)
                return false;
            if (scW > 0)
                continue;
        }
        else
        {
            double t = - scW / scD;
//            std::cout << "t = " << t << std::endl;
//            if (t <= 1 && t >= 0)
//            {
//                Point p(p1.getX() + (p2.getX() - p1.getX()) * t,
//                        p1.getY() + (p2.getY() - p1.getY()) * t);
//                points.push_back(p);
//            }
            if (scD > 0)
            {
                if (t > tn)
                    tn = t;
            }
            else
            {
                if (t < tv)
                    tv = t;
            }
        }
//        std::cout << std::endl << std::endl;
    }
    if (tn > tv)
        return false;
    else
    {
        r[1].setX(p1.getX() + (p2.getX() - p1.getX()) * tn);
        r[1].setY(p1.getY() + (p2.getY() - p1.getY()) * tn);
        r[2].setX(p1.getX() + (p2.getX() - p1.getX()) * tv);
        r[2].setY(p1.getY() + (p2.getY() - p1.getY()) * tv);
    }
    return true;
}
void Viewer::on_cut_but_clicked()
{
    std::vector<Point> c = cutter->getPoints();
    if (!checkCutter(c))
    {
        print_error("cutter must be convex");
    }
    for (auto &line : lines)
    {
        Point r[3] = {Point(0, 0), Point(0, 0), Point(0, 0)};
        if (cut(line->get_p1(), line->get_p2(), cutter->getPoints(), r))
        {
            res_lines.push_back(std::shared_ptr<Line>(new Line(r[1].getX(),
                                                      r[1].getY(),
                                                      r[2].getX(),
                                                      r[2].getY())));
        }

    }
    print_error("");
    repaint();
}

void Viewer::on_erase_lines_but_clicked()
{
    lines.clear();
    res_lines.clear();
    print_error("");
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

void Viewer::on_lines_mode_but_clicked()
{
    mode = 1;
}

void Viewer::on_add_cutter_line_but_clicked()
{
    bool ok;
    int x = ui->cutter_x_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading cutter x");
        return;
    }
    int y = ui->cutter_y_text->text().toInt(&ok);
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
    int delta = ui->line_delta_text->text().toInt(&ok);
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
    int delta = ui->line_delta_text->text().toInt(&ok);
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
        print_error("unable to finish polygone");
}


