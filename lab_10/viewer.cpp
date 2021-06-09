#include "viewer.h"
#include "ui_viewer.h"
double func_1(double x, double z)
{
    return sin(x) * sin(x) + cos(z) * cos(z);
}
double func_2(double x, double z)
{
    return sin(x) * sin(z);
}
double func_3(double x, double z)
{
    return sin(x) + sin(x) * cos(z) + cos(z);
}
double func_4(double x, double z)
{
    return sin(x) - cos(z);
}
double func_5(double x, double z)
{
    return sqrt(sin(z) + cos(x) * cos(z) + 2);
}
double to_rad(double ang)
{
    return ang * (M_PI / 180.0);
}
void viewer::print_error(QString message)
{
    ui->textEdit->setTextColor(Qt::red);
    ui->textEdit->setText(message);
}
viewer::viewer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::viewer)
{
    ui->setupUi(this);
    scene = std::shared_ptr<QGraphicsScene>(new QGraphicsScene());
    width = ui->graphicsView->width();
    height = ui->graphicsView->height();
    scene->setSceneRect(0, 0, width, height);
    func = NULL;
    ui->graphicsView->setScene(scene.get());
    x_angle = 0;
    y_angle = 0;
    z_angle = 0;
}

viewer::~viewer()
{
    delete ui;
}
void sleepFeature(int sleep_time)
{
    QTime end = QTime::currentTime().addMSecs(sleep_time);
    while (QTime::currentTime() < end)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }
    return;
}
void viewer::draw_point(point2d p, QColor color)
{
    scene->addRect(p.x, p.y, 1, 1, QPen(color));
    //sleepFeature(10);
}
void viewer::get_scale(double &scale_x, double &scale_y, double min_y, double max_y)
{
    scale_x = width / (max_x - min_x);
    scale_y = height / (max_y - min_y);
//    if (max_x * min_x < 0.0)
//        scale_x = width / (max_x - min_x);
//    else
//        scale_x = max_x > 0 ? width / max_x : width / min_x;

//    if (max_y * min_y < 0.0)
//        scale_y = height / (max_y - min_y);
//    else
//        scale_y = max_y > 0 ? height / max_y : height / min_y;
}
void viewer::draw_line(point2d p1, point2d p2, QColor color)
{

//    p1.rotate_z(z_angle);
//    p2.rotate_z(z_angle);
    scene->addLine(p1.x, p1.y,
                   p2.x, p2.y, QPen(color));

}
void viewer::define_visibility(point2d &p, int &lowhor, int &uphor, int &vis)
{
    if (p.y <= lowhor)
    {
        vis = -1;
    }
    else if (p.y >= uphor)
    {
        vis = 1;
    }
    else
        vis = 0;
}
void horizon(point2d f, point2d s, std::vector<int> &lowhor, std::vector<int> &uphor)
{
    if (s.x - f.x == 0)
    {
        uphor[s.x] = std::max(uphor[s.x], s.y);
        lowhor[s.x] = std::min(lowhor[s.x], s.y);
    }
    else
    {
        double m = (s.y - f.y) / 1.0 / (s.x - f.x);
        for (int x = f.x; x <= s.x; x++)
        {
            int y = m * (x - f.x) + f.y;
            uphor[x] = std::max(uphor[x], y);
            lowhor[x] = std::min(lowhor[x], y);
        }

    }

}
int sign(double val)
{
    if (val < 0)
        return -1;
    if (val > 0)
        return 1;
    return 0;
}
void viewer::intersection(point2d &intersection, point2d f, point2d s, std::vector<int> &hor)
{

    if (f.x == s.x)
    {
        intersection.x = f.x;
        intersection.y = hor[f.x];
        return;
    }
    double m = (s.y - f.y) / 1.0 / (s.x - f.x);
    int ys = sign(f.y + m - hor[f.x + 1]);
    int cs = ys;
    int xi = f.x + 1;
    double yi = f.y + m;
    while (cs == ys && xi <= s.x + 1)
    {
        yi += m;
        xi++;
        cs = sign(yi - hor[xi]);
    }
    if (abs(yi - m - hor[xi - 1]) <= abs(yi - hor[xi]))
    {
        yi -= m;
        xi--;
    }
    if (xi <= s.x && xi >= 0 && yi >= 0 && yi < height && xi < width)
        intersection = point2d(xi, yi);
    else
    {
//        if (hor[f.x] == height || hor[f.x] == 0)
        intersection = point2d(-1, -1);
//        else
//            intersection = point2d(f.x, hor[f.x]);

    }

}
void viewer::handle(point2d p, point2d &rib, std::vector<int> &lowhor, std::vector<int> &uphor)
{
    if (rib.x != -1)
    {
        horizon(rib, p, lowhor, uphor);
        draw_line(p, rib, Qt::red);
    }
    rib = p;
}
void viewer::fhorizon()
{
    if (func == NULL)
        return;
    if (min_z == max_z)
    {
        print_error("z must differ");
        return;
    }
    if (min_x == max_x)
    {
        print_error("x must differ");
        return;
    }


    point min = point(min_x, func(min_x, max_z), max_z),
          max = point(min_x, func(min_x, max_z), max_z);
    min.rotate(x_angle, y_angle, z_angle);
    max.rotate(x_angle, y_angle, z_angle);

    double z_step = (max_z - min_z) / 90;
    double x_step = (max_x - min_x) / 100;


    for (double z = max_z; z > min_z; z -= z_step)
        for (double x = min_x; x < max_x; x += x_step)
        {
            point tmp = point(x, func(x, z), z);
            tmp.rotate(x_angle, y_angle, z_angle);

            if (tmp.get_y() < min.get_y())
                min = tmp;
            if (tmp.get_y() > max.get_y())
                max = tmp;
        }

    for (int i = 0; i <= width ; i++)
    {
        lowhor.push_back(height);
        uphor.push_back(0);
    }
    double offset_x = -min_x;
    double offset_y = -min.get_y();

    double scale_x, scale_y;
    get_scale(scale_x, scale_y, min.get_y(), max.get_y());

    int cur_vis, prev_vis;
    point cur, prev;
    point2d left(-1, -1), right(-1, -1);
    for (double z = max_z; z > min_z; z -= z_step)
    {
        prev = point(min_x, func(min_x, z), z);
        prev.rotate(x_angle, y_angle, z_angle);

        point2d prev2d = prev.project(offset_x, offset_y, scale_x, scale_y);
        handle(prev2d, left, lowhor, uphor);
        define_visibility(prev2d, lowhor[prev2d.x], uphor[prev2d.x], prev_vis);
        for (double x = min_x + x_step; x < max_x; x += x_step)
        {
            cur = point(x, func(x, z), z);
            cur.rotate(x_angle, y_angle, z_angle);
            point2d cur2d = cur.project(offset_x, offset_y, scale_x, scale_y);
            if (cur2d.x > width || cur2d.x < 0)
                continue;
            define_visibility(cur2d, lowhor[cur2d.x], uphor[cur2d.x], cur_vis);
            if (cur_vis == prev_vis)
            {
                if (cur_vis == 1 || cur_vis == -1)
                {
                    draw_line(cur2d, prev2d, Qt::red);
                    horizon(prev2d, cur2d, lowhor, uphor);
                }
//                else
//                    draw_line(cur, prev, min.get_y(), max.get_y(), Qt::white);
            }
            else
            {
                if (prev_vis == -1)
                {
                    point2d low_intersection2d;
                    intersection(low_intersection2d, prev2d, cur2d, lowhor);
                    if (low_intersection2d.x != -1 && low_intersection2d.y != -1)
                    {
                        draw_line(prev2d, low_intersection2d, Qt::red);
                        horizon(prev2d, low_intersection2d, lowhor, uphor);
                    }
                    if (cur_vis == 1)
                    {
                        point2d up_intersection2d;
                        intersection(up_intersection2d, prev2d, cur2d, uphor);
                        if (up_intersection2d.x != -1 && up_intersection2d.y != -1)
                        {
                        draw_line(cur2d, up_intersection2d, Qt::red);
                        horizon(up_intersection2d, cur2d, lowhor, uphor);
                        }
                    }
                }
                if (prev_vis == 1)
                {
                    point2d up_intersection2d;
                    intersection(up_intersection2d, prev2d, cur2d, uphor);
                    if (up_intersection2d.x != -1 && up_intersection2d.y != -1)
                    {
                    draw_line(prev2d, up_intersection2d, Qt::red);
                    horizon(prev2d, up_intersection2d, lowhor, uphor);
                    }
                    if (cur_vis == -1)
                    {
                        point2d low_intersection2d;
                        intersection(low_intersection2d, prev2d, cur2d, lowhor);
                        if (low_intersection2d.x != -1 && low_intersection2d.y != -1)
                        {
                        draw_line(cur2d, low_intersection2d, Qt::red);
                        horizon(low_intersection2d, cur2d, lowhor, uphor);
                        }
                    }
                }
                if (prev_vis == 0)
                {
                    if (cur_vis == -1)
                    {
                        point2d low_intersection2d;
                        intersection(low_intersection2d, prev2d, cur2d, lowhor);
                        if (low_intersection2d.x != -1 && low_intersection2d.y != -1)
                        {
                        draw_line(cur2d, low_intersection2d, Qt::red);
                        horizon(low_intersection2d, cur2d, lowhor, uphor);
                        }
                    }
                    if (cur_vis == 1)
                    {
                        point2d up_intersection2d;
                        intersection(up_intersection2d, prev2d, cur2d, uphor);
                        if (up_intersection2d.x != -1 && up_intersection2d.y != -1)
                        {
                        draw_line(cur2d, up_intersection2d, Qt::red);
                        horizon(up_intersection2d, cur2d, lowhor, uphor);
                        }
                    }
                }
            }
            prev_vis = cur_vis;
            prev = cur;
            prev2d = cur2d;
        }
        handle(prev2d, right, lowhor, uphor);
    }
    lowhor.clear();
    uphor.clear();
}
void viewer::rotate(double ax, double ay, double az)
{
    x_angle += ax;
    y_angle += ay;
    z_angle += az;
}
void viewer::on_x_rotation_but_clicked()
{
    bool ok;
    double x_angle = ui->x_angle_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading x angle");
        return;
    }
    scene->clear();
    rotate(to_rad(x_angle), 0, 0);
    fhorizon();
}

void viewer::on_y_rotation_but_clicked()
{
    bool ok;
    double y_angle = ui->y_angle_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading y angle");
        return;
    }
    scene->clear();
    rotate(0, to_rad(y_angle), 0);
    fhorizon();
}

void viewer::on_z_rotation_but_clicked()
{
    bool ok;
    double z_angle = ui->z_angle_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading z angle");
        return;
    }
    scene->clear();
    rotate(0, 0, to_rad(z_angle));
    fhorizon();
}
void viewer::on_complex_rotation_but_clicked()
{
    bool ok;
    double x_angle = ui->x_angle_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading x angle");
        return;
    }
    double y_angle = ui->y_angle_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading y angle");
        return;
    }
    double z_angle = ui->z_angle_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading z angle");
        return;
    }
    scene->clear();
    rotate(to_rad(x_angle), to_rad(y_angle), to_rad(z_angle));
    fhorizon();
}

void viewer::on_func_1_but_clicked()
{
    bool ok;
    double min_x= ui->x_beg_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading x beg");
        return;
    }
    double max_x= ui->x_end_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading x end");
        return;
    }
    double min_z= ui->z_beg_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading z beg");
        return;
    }
    double max_z= ui->z_end_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading z end");
        return;
    }
    if (min_z > max_z)
    {
        double tmp = min_z;
        min_z = max_z;
        max_z = tmp;
    }
    if (min_x > max_x)
    {
        double tmp = min_x;
        min_x = max_x;
        max_x = tmp;
    }

    this->min_x = min_x;
    this->max_x = max_x;
    this->min_z = min_z;
    this->max_z = max_z;

    scene->clear();
    func = func_1;
    x_angle = 0;
    y_angle = 0;
    z_angle = 0;
    fhorizon();
}

void viewer::on_func_2_but_clicked()
{
    bool ok;
    double min_x= ui->x_beg_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading x beg");
        return;
    }
    double max_x= ui->x_end_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading x end");
        return;
    }
    double min_z= ui->z_beg_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading z beg");
        return;
    }
    double max_z= ui->z_end_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading z end");
        return;
    }
    if (min_z > max_z)
    {
        double tmp = min_z;
        min_z = max_z;
        max_z = tmp;
    }
    if (min_x > max_x)
    {
        double tmp = min_x;
        min_x = max_x;
        max_x = tmp;
    }

    this->min_x = min_x;
    this->max_x = max_x;
    this->min_z = min_z;
    this->max_z = max_z;

    scene->clear();
    func = func_2;
    x_angle = 0;
    y_angle = 0;
    z_angle = 0;
    fhorizon();
}


void viewer::on_func_3_but_clicked()
{
    bool ok;
    double min_x= ui->x_beg_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading x beg");
        return;
    }
    double max_x= ui->x_end_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading x end");
        return;
    }
    double min_z= ui->z_beg_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading z beg");
        return;
    }
    double max_z= ui->z_end_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading z end");
        return;
    }
    if (min_z > max_z)
    {
        double tmp = min_z;
        min_z = max_z;
        max_z = tmp;
    }
    if (min_x > max_x)
    {
        double tmp = min_x;
        min_x = max_x;
        max_x = tmp;
    }

    this->min_x = min_x;
    this->max_x = max_x;
    this->min_z = min_z;
    this->max_z = max_z;

    scene->clear();
    func = func_3;
    x_angle = 0;
    y_angle = 0;
    z_angle = 0;
    fhorizon();
}

void viewer::on_func_4_but_clicked()
{
    bool ok;
    double min_x= ui->x_beg_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading x beg");
        return;
    }
    double max_x= ui->x_end_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading x end");
        return;
    }
    double min_z= ui->z_beg_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading z beg");
        return;
    }
    double max_z= ui->z_end_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading z end");
        return;
    }
    if (min_z > max_z)
    {
        double tmp = min_z;
        min_z = max_z;
        max_z = tmp;
    }
    if (min_x > max_x)
    {
        double tmp = min_x;
        min_x = max_x;
        max_x = tmp;
    }

    this->min_x = min_x;
    this->max_x = max_x;
    this->min_z = min_z;
    this->max_z = max_z;

    scene->clear();
    func = func_4;
    x_angle = 0;
    y_angle = 0;
    z_angle = 0;
    fhorizon();
}

void viewer::on_func_5_but_clicked()
{
    bool ok;
    double min_x= ui->x_beg_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading x beg");
        return;
    }
    double max_x= ui->x_end_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading x end");
        return;
    }
    double min_z= ui->z_beg_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading z beg");
        return;
    }
    double max_z= ui->z_end_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading z end");
        return;
    }
    if (min_z > max_z)
    {
        double tmp = min_z;
        min_z = max_z;
        max_z = tmp;
    }
    if (min_x > max_x)
    {
        double tmp = min_x;
        min_x = max_x;
        max_x = tmp;
    }

    this->min_x = min_x;
    this->max_x = max_x;
    this->min_z = min_z;
    this->max_z = max_z;

    scene->clear();
    func = func_5;
    x_angle = 0;
    y_angle = 0;
    z_angle = 0;
    fhorizon();
}
