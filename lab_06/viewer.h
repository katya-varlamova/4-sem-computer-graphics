#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>
#include <QPainter>
#include <math.h>
#include <vector>
#include <stack>
#include <string>
#include <string.h>
#include <QPainter>
#include <QGraphicsScene>
#include "paint_widget.h"


QT_BEGIN_NAMESPACE
namespace Ui { class viewer; }
QT_END_NAMESPACE

class viewer : public QMainWindow
{
    Q_OBJECT

public:

    Paint_widget *paint_widget;

    int mode;

    viewer(QWidget *parent = nullptr);

    ~viewer();

    void print_error(QString message);

    void keyPressEvent(QKeyEvent *event);

private slots:

    void on_with_delay_but_clicked();

    void on_without_delay_but_clicked();

    void on_red_pen_but_clicked();

    void on_green_pen_but_clicked();

    void on_white_pen_but_clicked();

    void on_blue_pen_but_clicked();

    void on_yellow_pen_but_clicked();

    void on_magenta_pen_but_clicked();

    void on_cyan_pen_but_clicked();

    void on_red_brush_but_clicked();

    void on_green_brush_but_clicked();

    void on_white_brush_but_clicked();

    void on_bacground_brush_but_clicked();

    void on_blue_brush_but_clicked();

    void on_yellow_brush_but_clicked();

    void on_magenta_brush_but_clicked();

    void on_cyan_brush_but_clicked();

    void on_fill_but_clicked();

    void on_erase_but_clicked();

    void on_add_point_but_clicked();

    void on_add_hole_but_clicked();

    void on_delay_text_textChanged(const QString &arg1);

    void on_set_seed_but_clicked();

    void on_seed_mode_but_clicked();

    void on_draw_mode_but_clicked();

    void on_add_horisontal_but_clicked();

    void on_add_vertical_but_clicked();

private:
    Ui::viewer *ui;
};
#endif // VIEWER_H
