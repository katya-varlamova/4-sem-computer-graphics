#include "graphics.h"
#include "ui_graphics.h"

#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stack>
#define POINTS_NUM 300
std::stack <transformation_t> tranformation_stack;
void graphics::move(double x, double y)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < polygones[i].points_num; j++)
        {
            polygones[i].points[j].setX(polygones[i].points[j].x() + x);
            polygones[i].points[j].setY(polygones[i].points[j].y() + y);
        }
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < POINTS_NUM; j++)
        {
            ellipses[i].points[j].setX(ellipses[i].points[j].x() + x);
            ellipses[i].points[j].setY(ellipses[i].points[j].y() + y);
        }
    for (int i =0; i < 3; i++)
    {
        points[i].setX(points[i].x() + x);
        points[i].setY(points[i].y() + y);
    }
}
void graphics::move_slot_handle()
{
    bool ok;
    double x = ui->x_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading move x");
        return;
    }
    double y = ui->y_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading move y");
        return;
    }
    ui->back_but->setEnabled(true);
    print_error("");
    move(x, y);
    transformation_t t;
    t.transf.move = { x, y };
    t.type = MOVE;
    tranformation_stack.push(t);
    repaint();
}
void graphics::scale(double scale_x, double scale_y, double scale_x_center, double scale_y_center)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < polygones[i].points_num; j++)
        {
            polygones[i].points[j].setX(scale_x * polygones[i].points[j].x() + scale_x_center * (1.0 - scale_x));
            polygones[i].points[j].setY(scale_y * polygones[i].points[j].y() + scale_y_center * (1.0 - scale_y));
        }
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < POINTS_NUM; j++)
        {
            ellipses[i].points[j].setX(scale_x * ellipses[i].points[j].x() + scale_x_center * (1.0 - scale_x));
            ellipses[i].points[j].setY(scale_y * ellipses[i].points[j].y() + scale_y_center * (1.0 - scale_y));
        }
    for (int j = 0; j < 3; j++)
    {
        points[j].setX(scale_x * points[j].x() + scale_x_center * (1.0 - scale_x));
        points[j].setY(scale_y * points[j].y() + scale_y_center * (1.0 - scale_y));
    }
}
void graphics::scale_slot_handle()
{
    bool ok;
    double scale_x = ui->scale_x_text->text().toDouble(&ok);
    if (!ok || scale_x == 0)
    {
        print_error("error reading scale coefficient x");
        return;
    }
    double scale_x_center =  ui->scale_center_x_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading scale center x");
        return;
    }
    double scale_y =  ui->scale_y_text->text().toDouble(&ok);
    if (!ok || scale_y == 0)
    {
        print_error("error reading scale coefficient y");
        return;
    }
    double scale_y_center =  ui->scale_center_y_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading scale center y");
        return;
    }
    print_error("");
    ui->back_but->setEnabled(true);
    scale(scale_x, scale_y, scale_x_center, scale_y_center);
    transformation_t t;
    t.transf.scale = { scale_x, scale_y, scale_x_center, scale_y_center };
    t.type = SCALE;
    tranformation_stack.push(t);
    repaint();
}
void graphics::slot_back()
{
    if (tranformation_stack.empty())
        return;
    transformation_t t = tranformation_stack.top();
    tranformation_stack.pop();
    if (t.type == MOVE)
    {
        move(-t.transf.move.move_x, -t.transf.move.move_y);
    }
    if (t.type == SCALE)
    {
        scale(1 / t.transf.scale.scale_x, 1 / t.transf.scale.scale_y,
             t.transf.scale.center_x, t.transf.scale.center_y);
    }
    if (t.type == ROTATE)
    {
        rotate(t.transf.rotate.rotate_x, t.transf.rotate.rotate_y,
            -t.transf.rotate.angle);
    }
    repaint();
    if (tranformation_stack.empty())
        ui->back_but->setEnabled(false);
}
void graphics::print_error(QString message)
{
    ui->textEdit->setTextColor(Qt::red);
    ui->textEdit->setText(message);
}
double to_rad(double ang)
{
    double ret = ang * (M_PI / 180.0);
    return ret;
}
void graphics::rotate(double rotate_x, double rotate_y, double angle )
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < polygones[i].points_num; j++)
        {
            double x = polygones[i].points[j].x(), y = polygones[i].points[j].y();
            polygones[i].points[j].setX(rotate_x +
                                            (x - rotate_x) * cos(to_rad(angle)) +
                                            (y - rotate_y) * sin(to_rad(angle))
                                            );
            polygones[i].points[j].setY(rotate_y -
                                            (x - rotate_x) * sin(to_rad(angle)) +
                                            (y - rotate_y) * cos(to_rad(angle))
                                            );
        }
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < POINTS_NUM; j++)
        {
            double x = ellipses[i].points[j].x(), y = ellipses[i].points[j].y();
            ellipses[i].points[j].setX(rotate_x +
                                                (x - rotate_x) * cos(to_rad(angle)) +
                                                (y - rotate_y) * sin(to_rad(angle))
                                                );
            ellipses[i].points[j].setY(rotate_y -
                                                (x - rotate_x) * sin(to_rad(angle)) +
                                                (y - rotate_y) * cos(to_rad(angle))
                                                );
        }
    for (int j = 0; j < 3; j++)
    {
        double x = points[j].x(), y = points[j].y();
        points[j].setX(rotate_x +
                       (x - rotate_x) * cos(to_rad(angle)) +
                       (y - rotate_y) * sin(to_rad(angle))
                       );
        points[j].setY(rotate_y -
                       (x - rotate_x) * sin(to_rad(angle)) +
                       (y - rotate_y) * cos(to_rad(angle))
                       );
    }
}
void graphics::rotate_slot_handle()
{
    bool ok;
    double rotate_x = ui->scale_center_x_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading rotation center x");
        return;
    }
    double rotate_y =  ui->scale_center_y_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading rotation center y");
        return;
    }
    double angle =  ui->angle_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading rotation angle");
        return;
    }
    print_error("");
    ui->back_but->setEnabled(true);
    rotate(rotate_x, rotate_y, angle);
    transformation_t t;
    t.transf.rotate = {rotate_x, rotate_y, angle };
    t.type = ROTATE;
    tranformation_stack.push(t);
    repaint();
}
graphics::graphics(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::graphics)
{
    ui->setupUi(this);

    graph_height = ui->groupBox->height();
    graph_width = ui->groupBox->width();
    graph_x_pos = ui->groupBox->x();
    graph_y_pos = ui->groupBox->y();

    build_image();

    connect(ui->move_but, SIGNAL (clicked()), this, SLOT (move_slot_handle()));

    connect(ui->scale_but, SIGNAL (clicked()), this, SLOT (scale_slot_handle()));

    connect(ui->rotate_but, SIGNAL (clicked()), this, SLOT (rotate_slot_handle()));

    connect(ui->back_but, SIGNAL (clicked()), this, SLOT (slot_back()));

    ui->back_but->setEnabled(false);

}
graphics::~graphics()
{
    delete ui;
    free_image();
}
void graphics::build_image()
{
    double x = graph_x_pos + graph_width / 2, y = graph_y_pos + graph_height / 2, r = 50;
    polygones = (polygone_t *) malloc(3 * sizeof (polygone_t));

    polygones[0].points = (QPointF *) malloc(3 * sizeof(QPointF));
    polygones[0].points_num = 3;

    polygones[1].points = (QPointF *) malloc(4 * sizeof(QPointF));
    polygones[1].points_num = 4;

    polygones[2].points = (QPointF *) malloc(4 * sizeof(QPointF));
    polygones[2].points_num = 4;

    polygones[0].points[2].setX(x + r);                        polygones[0].points[2].setY(y - r); // top
    polygones[0].points[0].setX(x - sqrt(3) * r  + r);         polygones[0].points[0].setY(y + 2 * r); // left
    polygones[0].points[1].setX(x + sqrt(3) * r  + r);         polygones[0].points[1].setY(y + 2 * r); // right

    polygones[1].points[0].setX(x + r - 3 * r);                polygones[1].points[0].setY(y - r - sqrt(3) * r); // top top
    polygones[1].points[1].setX(x + r);                        polygones[1].points[1].setY(y - r); // top
    polygones[1].points[2].setX(x - sqrt(3) * r  + r);         polygones[1].points[2].setY(y + 2 * r); // left
    polygones[1].points[3].setX(x - sqrt(3) * r  + r - 3 * r); polygones[1].points[3].setY(y + 2 * r - sqrt(3) * r); // left top

    polygones[2].points[0].setX(x + r + 3 * r);                polygones[2].points[0].setY(y - r - sqrt(3) * r); // top top
    polygones[2].points[1].setX(x + r);                        polygones[2].points[1].setY(y - r); // top
    polygones[2].points[2].setX(x + sqrt(3) * r  + r);         polygones[2].points[2].setY(y + 2 * r); // left
    polygones[2].points[3].setX(x + sqrt(3) * r  + r + 3 * r); polygones[2].points[3].setY(y + 2 * r - sqrt(3) * r); // left top


    ellipses = (ellipse_t *) malloc(2 * sizeof (ellipse_t));

    ellipses[0].points = (QPointF *) malloc(POINTS_NUM * sizeof(QPointF));
    ellipses[1].points = (QPointF *) malloc(POINTS_NUM * sizeof(QPointF));
    double h = (polygones[0].points[1].x() - polygones[0].points[0].x()) / 2, w = (polygones[0].points[1].x() - polygones[0].points[0].x());
    double x_center = polygones[0].points[0].x() + w / 2;
    double y_center = polygones[0].points[0].y();
    double step = w / (POINTS_NUM / 2);
    double a = w / 2;
    double b = h / 2;
    for (int i = 0; i < POINTS_NUM / 2; i++)
    {
        double x_p = -a + i * step;
        double y_p = sqrt(b * b * (1 - x_p * x_p / a / a));
        ellipses[0].points[i].setX(x_center + x_p);
        ellipses[0].points[i].setY(y_center + y_p);
        ellipses[0].points[POINTS_NUM - i - 1].setX(x_center + x_p);
        ellipses[0].points[POINTS_NUM - i - 1].setY(y_center - y_p);
    }
    h = 2 * r;
    w = 2 * r;
    x_center = x + r;
    y_center = y + r;
    step = w / (POINTS_NUM / 2);
    a = w / 2;
    b = h / 2;

    for (int i = 0; i < POINTS_NUM / 2; i++)
    {
        double x_p = -a + i * step;
        double y_p = sqrt(b * b * (1 - x_p * x_p / a / a));
        ellipses[1].points[i].setX(x_center + x_p);
        ellipses[1].points[i].setY(y_center + y_p);
        ellipses[1].points[POINTS_NUM - i - 1].setX(x_center + x_p);
        ellipses[1].points[POINTS_NUM - i - 1].setY(y_center - y_p);
    }
    points = (QPointF *) malloc(sizeof(QPointF) * 3);
    points[0].setX(x + r);
    points[0].setY(y - r - sqrt(3) * r + (y + 2 * r + sqrt(3) * r / 2 - (y - r - sqrt(3) * r)) / 2);
    points[1].setX(x + r);
    points[1].setY(y + sqrt(3) * r / 2 + 2 * r);
    points[2].setX(x + r);
    points[2].setY(y + r);

}
void graphics::free_image()
{
    for (int i = 0; i < 3; i++)
    {
        free(polygones[i].points);
    }
    free(polygones);
    for (int i = 0; i < 2; i++)
    {
        free(ellipses[i].points);
    }
    free(ellipses);
    free(points);
}
void graphics::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter ( this );
    QColor color_1 = QColor(238, 128, 177), color_2 = QColor(41, 42, 47);

    painter.setPen(QPen(color_1, 2, Qt::SolidLine));
    painter.setBrush(QBrush(color_2));
    painter.drawPolygon(ellipses[0].points, POINTS_NUM);

    painter.setPen(QPen(color_2, 2, Qt::SolidLine));
    painter.setBrush(QBrush(color_1));
    painter.drawPolygon(polygones[0].points, polygones[0].points_num);

    painter.setPen(QPen(color_1, 2, Qt::SolidLine));
    painter.setBrush(QBrush(color_2));

    painter.drawPolygon(polygones[1].points, polygones[1].points_num);
    painter.drawPolygon(polygones[2].points, polygones[2].points_num);

    painter.setPen(QPen(color_2, 2, Qt::SolidLine, Qt::FlatCap));
    painter.setBrush(QBrush(color_2));

    painter.drawPolygon(ellipses[1].points, POINTS_NUM);

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < polygones[i].points_num; j++)
        {
            char *str;
            asprintf(&str, "(%.1lf, %.1lf)", polygones[i].points[j].x(), polygones[i].points[j].y());
            painter.setPen(QPen(Qt::white, 5, Qt::SolidLine));
            painter.drawText(QPointF(polygones[i].points[j].x(), polygones[i].points[j].y()), str);
            painter.drawPoint(polygones[i].points[j].x(), polygones[i].points[j].y());
            free(str);
        }
    for (int i = 0; i < 3; i++)
    {
        char *str;
        asprintf(&str, "(%.1lf, %.1lf)", points[i].x(), points[i].y());
        painter.setPen(QPen(Qt::white, 5, Qt::SolidLine));
        painter.drawText(points[i], str);
        painter.drawPoint(points[i]);
        free(str);
    }

}

















//    for (int i = 1; i < 2; i++)
//    {
//        for (int j = 0; j < 5; j++)
//        {
//        char *str;
//        asprintf(&str, "(%.1lf, %.1lf)", ellipses[i].points[2].x(), ellipses[i].points[2].y());
//        painter.setPen(QPen(Qt::white, 5, Qt::SolidLine));
//        //painter.drawText(QPointF(ellipses[i].points[j].x(), ellipses[i].points[j].y()), str);
//        painter.drawPoint(ellipses[i].points[j].x(), ellipses[i].points[j].y());
//        free(str);
//        }
//    }

//    painter.save();
//    double ang = define_angle(ellipses[0].points[3], ellipses[0].points[4]);
//    std::cout << ang << std::endl;
//    painter.translate(ellipses[0].rotation.rotate_x, ellipses[0].rotation.rotate_y);
//    rotate_ellipses(ellipses[0].rotation.rotate_x, ellipses[0].rotation.rotate_y, ang);
//    painter.rotate(ang);
//    painter.translate(-ellipses[0].rotation.rotate_x, -ellipses[0].rotation.rotate_y);
//    painter.drawEllipse(QRectF(ellipses[0].points[0], ellipses[0].points[1]));
//    rotate_ellipses(ellipses[0].rotation.rotate_x, ellipses[0].rotation.rotate_y, -ang);
//    painter.restore();
//void graphics::rotate_ellipses(double rotate_x, double rotate_y, double angle)
//{
//    for (int i = 0; i < 2; i++)
//        for (int j = 0; j < 2; j++)
//        {
//            double x = ellipses[i].points[j].x(), y = ellipses[i].points[j].y();
//            ellipses[i].points[j].setX(rotate_x +
//                                                (x - rotate_x) * cos(to_rad(angle)) +
//                                                (y - rotate_y) * sin(to_rad(angle))
//                                                );
//            ellipses[i].points[j].setY(rotate_y -
//                                                (x - rotate_x) * sin(to_rad(angle)) +
//                                                (y - rotate_y) * cos(to_rad(angle))
//                                                );
//        }
//}
//double to_deg(double ang)
//{
//    double ret = ang / M_PI * 180.0;
//    return ret;
//}
//double graphics::define_angle(QPointF p1, QPointF p2)
//{
//    double y = p2.y() - p1.y();
//    double x = p2.x() - p1.x();
//    double r = std::atan2(y, x);
//    return to_deg(r);
//}
//    ellipses[0].points[0].setX(polygones[0].points[0].x());         ellipses[0].points[0].setY(polygones[0].points[0].y() - h / 2);
//    ellipses[0].points[1].setX(polygones[0].points[0].x() + w);     ellipses[0].points[1].setY(polygones[0].points[0].y() + h / 2);
//    ellipses[0].points[2].setX(polygones[0].points[0].x() + w / 2); ellipses[0].points[2].setY(polygones[0].points[0].y() + h / 2);
//    ellipses[0].points[3].setX(polygones[0].points[0].x());         ellipses[0].points[3].setY(polygones[0].points[0].y());
//    ellipses[0].points[4].setX(polygones[0].points[0].x() + w);     ellipses[0].points[4].setY(polygones[0].points[0].y());
//    ellipses[0].rotation.angle = 0;
//    ellipses[0].rotation.rotate_x = 0;
//    ellipses[0].rotation.rotate_y = 0;

//    ellipses[1].points[0].setX(x);           ellipses[1].points[0].setY(y);
//    ellipses[1].points[1].setX(x + 2 * r);   ellipses[1].points[1].setY(y + 2 * r);
//    ellipses[1].points[2].setX(x + r);       ellipses[1].points[2].setY(y + r);
//    ellipses[1].points[3].setX(x);           ellipses[1].points[3].setY(y + r);
//    ellipses[1].points[4].setX(x + 2 * r);   ellipses[1].points[4].setY(y + r);
//    std::cout<< ellipses[1].points[0].x() << " " << ellipses[1].points[0].y() << std::endl;
//    std::cout<< ellipses[1].points[1].x() << " " << ellipses[1].points[1].y() << std::endl << std::endl;


//    ellipses[1].rotation.angle = 0;
//    ellipses[1].rotation.rotate_x = 0;
//    ellipses[1].rotation.rotate_y = 0;
