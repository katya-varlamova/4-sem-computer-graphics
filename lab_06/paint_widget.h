#ifndef PAINT_WIDGET_H
#define PAINT_WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <unistd.h>
#include <sys/time.h>
#include <stack>
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include <QThread>
#include <stack>
typedef struct
{
    int x, y;
} point;
typedef struct
{
    point p1, p2;
} edge_t;

typedef struct
{
    std::vector <edge_t> edges;
    bool finished;
} polygone_t;

class Paint_widget : public QWidget
{
    Q_OBJECT
public:
    explicit Paint_widget(int x, int y, int width, int height, QWidget *parent = nullptr);
    void redrawImage();
    void sleepFeature();
    void finish_figure(std::vector <int> &time);
    void set_brush_color(const QColor &color);
    void set_delay(bool delay, int time = 0);
    void set_pen_color(const QColor &color);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void add_point(point &p);
    void erase();
    void draw_point(QPainter *painter, int x, int y);
    point get_last_point();
    int sign(double c);
    void bresenham_int(QPainter *painter, const point &p_b, const point &p_e);
    void draw_borders(QPainter *painter, const std::vector <edge_t> &edges);
    void fill(const polygone_t &polygone, point &point, int &time);
    void add_hole();
    void change_seed(point p);
    std::vector <polygone_t> polygones;
    point seed;
    int mode;
    QImage *image;
    bool delay;
    QColor bg_color, pen_color, brush_color;
    int holes;
    int sleep_time;

signals:

};

#endif // PAINT_WIDGET_H
