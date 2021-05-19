#include "Viewer.h"
#include "ui_Viewer.h"

Viewer::Viewer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Viewer)
{
    ui->setupUi(this);
    mode = 0;
    cutter = std::shared_ptr<Cutter>(new Cutter);
    line_direction = 0;
}

Viewer::~Viewer()
{
    delete ui;
}
void Viewer::keyPressEvent(QKeyEvent *event)
{
    if (mode == 1 && (event->key() == Qt::Key_Shift))
    {
        line_direction = 1;
    }
    if (mode == 1 && (event->key() == Qt::Key_Return))
    {
        line_direction = 2;
    }
}
void Viewer::mousePressEvent(QMouseEvent *event)
{
    if (mode == 0)
    {
        if (!cutter->is_left_built() && !cutter->is_right_built())
        {
            cutter->init_left_top(event->position().x(), event->position().y());
            print_error("");
        }
        else if (cutter->is_left_built() && !cutter->is_right_built())
        {
            cutter->init_right_bottom(event->position().x(), event->position().y());
            print_error("");
        }
        else
           print_error("cutter already exists, please, erase it and try again");
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
void Viewer::draw_point(QPainter *painter, int x, int y)
{
    if (painter == NULL)
        return;
    painter->drawPoint(x, y);
}
int Viewer::sign(double c)
{
    if (c < -1e-6)
        return -1;
    else
        return 1;
}
void Viewer::bresenham_int(QPainter *painter, Point p_b, Point p_e)
{
    if (p_b.getX() == p_e.getX() && p_b.getY() == p_e.getY())
    {
        draw_point(painter, round(p_b.getX()), round(p_b.getY()));
        return;
    }
    Point cur(0, 0);
    int dx, dy;
    bool fl = 0;
    int sx, sy, f;
    dx = p_e.getX() - p_b.getX();
    dy = p_e.getY() - p_b.getY();
    sx = sign(dx);
    sy = sign(dy);
    dx = abs(dx);
    dy = abs(dy);
    if (dy >= dx)
    {
        int tmp = dx;
        dx = dy;
        dy = tmp;
        fl = 1;
    }
    f = 2 * dy - dx;
    cur = p_b;
    int len = dx;
    for (int i = 0; i < len; i++)
    {
        draw_point(painter, cur.getX(), cur.getY());
        if (f >= 0)
        {
            if (fl == 1)
                cur.setX(cur.getX() + sx);
            else
                cur.setY(cur.getY() + sy);
            f -= 2 * dx;
        }
        if (f < 0)
        {
            if (fl == 1)
                cur.setY(cur.getY() + sy);
            else
                cur.setX(cur.getX() + sx);
        }
        f += 2 * dy;
    }
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
    if (cutter->is_left_built() && cutter->is_right_built())
    {
        painter.drawLine(cutter->get_left_top().getX(), cutter->get_left_top().getY(),
                         cutter->get_right_bottom().getX(), cutter->get_left_top().getY());

        painter.drawLine(cutter->get_right_bottom().getX(), cutter->get_left_top().getY(),
                         cutter->get_right_bottom().getX(), cutter->get_right_bottom().getY());

        painter.drawLine(cutter->get_right_bottom().getX(), cutter->get_right_bottom().getY(),
                         cutter->get_left_top().getX(), cutter->get_right_bottom().getY());

        painter.drawLine(cutter->get_left_top().getX(), cutter->get_right_bottom().getY(),
                         cutter->get_left_top().getX(), cutter->get_left_top().getY());
        char *str;
        asprintf(&str, "(%d, %d)", cutter->get_left_top().getX(), cutter->get_left_top().getY());
        painter.setPen(QPen(Qt::white, 3, Qt::SolidLine));
        painter.drawText(QPointF(cutter->get_left_top().getX(), cutter->get_left_top().getY()), str);
        free(str);

        asprintf(&str, "(%d, %d)", cutter->get_right_bottom().getX(), cutter->get_left_top().getY());
        painter.setPen(QPen(Qt::white, 3, Qt::SolidLine));
        painter.drawText(QPointF(cutter->get_right_bottom().getX(), cutter->get_left_top().getY()), str);
        free(str);

        asprintf(&str, "(%d, %d)", cutter->get_right_bottom().getX(), cutter->get_right_bottom().getY());
        painter.setPen(QPen(Qt::white, 3, Qt::SolidLine));
        painter.drawText(QPointF(cutter->get_right_bottom().getX(), cutter->get_right_bottom().getY()), str);
        free(str);

        asprintf(&str, "(%d, %d)", cutter->get_left_top().getX(), cutter->get_right_bottom().getY());
        painter.setPen(QPen(Qt::white, 3, Qt::SolidLine));
        painter.drawText(QPointF(cutter->get_left_top().getX(), cutter->get_right_bottom().getY()), str);
        free(str);
    }
    if (cutter->is_left_built() && !cutter->is_right_built())
        painter.drawRect(cutter->get_left_top().getX(),
                         cutter->get_left_top().getY(), 1, 1);

    painter.setPen(Qt::red);
    for (auto &line : lines)
    {
        if (line->is_p1_built() && line->is_p2_built())
        {
//            bresenham_int(&painter, line->get_p1(), line->get_p2());
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
//            bresenham_int(&painter, line->get_p1(), line->get_p2());
            painter.drawLine(line->get_p1().getX(),
                             line->get_p1().getY(),
                             line->get_p2().getX(),
                             line->get_p2().getY());
        }
        if (line->is_p1_built() && !line->is_p2_built())
            painter.drawPoint(line->get_p1().getX(),
                             line->get_p1().getY());
    }
}
void Viewer::on_draw_cutter_but_clicked()
{
    bool ok;
    int xleft = ui->cutter_xleft_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading xleft");
        return;
    }
    int xright = ui->cutter_xright_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading xright");
        return;
    }
    int ybottom = ui->cutter_ybottom_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading ybottom");
        return;
    }
    int ytop = ui->cutter_ytop_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading ytop");
        return;
    }
    print_error("");
    cutter = std::unique_ptr<Cutter>(new Cutter(xleft, ytop, xright, ybottom));
    repaint();
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
#include <iostream>
bool Viewer::cut(Point p1, Point p2, int xl, int yt, int xr, int yb, Point *r)
{
    Point q(0, 0);
    int t1[4], t2[4];
    fill_code(p1, t1, xl, yt, xr, yb);
    fill_code(p2, t2, xl, yt, xr, yb);

    double m = MAXFLOAT;
    if (sum_code(t1) == 0 && sum_code(t2) == 0)
    {
        r[1] = p1;
        r[2] = p2;
        return true;
    }

    if (mul_codes(t1, t2) != 0)
    {
        return false;
    }
    int step = 1;
    q = p1;
    if (sum_code(t1) == 0)
    {
        r[1] = p1;
        q = p2;
        step = 2;
    }
    if (sum_code(t2) == 0)
    {
        r[1] = p2;
        q = p1;
        step = 2;
    }
    for (; step < 3; step++)
    {
        if (p1.getX() != p2.getX())
        {
            m = (1.0 * p2.getY() - p1.getY()) / ((p2.getX() - p1.getX()));
            if (q.getX() <= xl)
            {
                int yp = round(m * (xl - q.getX())) + q.getY();
                if (yp <= yt && yp >= yb)
                {
                    r[step].setX(xl);
                    r[step].setY(yp);
                    q = p2;
                    continue;
                }
            }
            if (q.getX() >= xr)
            {
                int yp = round(m * (xr - q.getX()) + q.getY());
                if (yp <= yt && yp >= yb)
                {
                    r[step].setX(xr);
                    r[step].setY(yp);
                    q = p2;
                    continue;
                }
            }

        }
        if (p1.getY() != p2.getY())
        {
            if (p2.getX() != p1.getX())
                m = (1.0 * p2.getY() - p1.getY()) / ((p2.getX() - p1.getX()));
            if (q.getY() >= yt)
            {
                int xp;
                if (p2.getX() != p1.getX())
                    xp = round((yt - q.getY()) / m + q.getX());
                else
                    xp = q.getX();
                if (xp <= xr && xp >= xl)
                {
                    r[step].setX(xp);
                    r[step].setY(yt);
                    q = p2;
                    continue;
                }
            }
            if (q.getY() <= yb)
            {
                int xp;
                if (p2.getX() != p1.getX())
                    xp = round((yb - q.getY()) / m + q.getX());
                else
                    xp = q.getX();
                if (xp <= xr && xp >= xl)
                {
                    r[step].setX(xp);
                    r[step].setY(yb);
                    q = p2;
                    continue;
                }
            }
        }
        return false;
    }
    return true;
}
void Viewer::on_cut_but_clicked()
{
    for (auto &line : lines)
    {
        Point r[3] = {Point(0, 0), Point(0, 0), Point(0, 0)};
        if (cut(line->get_p1(), line->get_p2(),
            cutter->get_left_top().getX(),
            cutter->get_right_bottom().getY(),
            cutter->get_right_bottom().getX(),
            cutter->get_left_top().getY(), r))
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
    cutter->reset();
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
