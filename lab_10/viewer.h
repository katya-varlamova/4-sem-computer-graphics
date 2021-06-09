#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "point.h"
#include <unistd.h>
#include <QEventLoop>
#include <QThread>
#include <QTime>
#include <iostream>
QT_BEGIN_NAMESPACE
namespace Ui { class viewer; }
QT_END_NAMESPACE

class viewer : public QMainWindow
{
    Q_OBJECT

public:
    viewer(QWidget *parent = nullptr);
    ~viewer();

private slots:
    void on_x_rotation_but_clicked();

    void on_y_rotation_but_clicked();

    void on_z_rotation_but_clicked();

    void on_complex_rotation_but_clicked();

    void on_func_1_but_clicked();

    void on_func_2_but_clicked();

    void on_func_3_but_clicked();

    void on_func_4_but_clicked();

    void on_func_5_but_clicked();

private:
    double x_angle, y_angle, z_angle;
    double min_x, max_x, min_z, max_z;
    std::vector<int>uphor, lowhor;
    int width, height;
    double (*func)(double, double);
    void rotate(double, double, double);
    void draw_point(point2d p, QColor color);
    void get_scale(double &scale_x, double &scale_y, double min_y, double max_y);
    void draw_line(point2d p1, point2d p2, QColor color);
    void define_visibility(point2d &p, int &lowhor, int &uphor, int &vis);
    void handle(point2d p, point2d &rib, std::vector<int> &lowhor, std::vector<int> &uphor);
    void intersection(point2d &intersection, point2d f, point2d s, std::vector<int> &hor);
    void fhorizon();
    std::shared_ptr<QGraphicsScene> scene;
    void print_error(QString message);
    Ui::viewer *ui;
};
#endif // VIEWER_H
