#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QMainWindow>
#define _USE_MATH_DEFINES
#include <math.h>
#include <QWidget>
#include <QtWidgets>
#include <QPushButton>
#include <QPointF>

QT_BEGIN_NAMESPACE
namespace Ui { class graphics; }
QT_END_NAMESPACE
static int graph_x_pos = 0, graph_y_pos = 0, graph_height = 0, graph_width = 0;
typedef enum
{
    MOVE, SCALE, ROTATE
} transformation_type;
typedef struct
{
    double move_x, move_y;
} move_t;
typedef struct
{
    double scale_x, scale_y, center_x, center_y;
} scale_t;
typedef struct
{
    double rotate_x, rotate_y;
    double angle;
} rotate_t;
typedef union
{
    move_t move;
    scale_t scale;
    rotate_t rotate;
} transformation_u;
typedef struct
{
    transformation_u transf;
    transformation_type type;
} transformation_t;
typedef  struct
{
    QPointF *points;
    int points_num;
} polygone_t;
typedef  struct
{
    QPointF *points;
} ellipse_t;

class graphics : public QMainWindow
{
    Q_OBJECT
public:
    polygone_t *polygones;
    ellipse_t *ellipses;
    QPointF * points;
    explicit graphics(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    ~graphics();
    void build_image();
    void free_image();
    void print_error(QString message);
    void rotate(double, double, double);
    void move(double, double);
    void scale(double, double, double, double);
    void rotate_ellipses(double rotate_x, double rotate_y, double angle);
    double define_angle(QPointF p1, QPointF p2);
public slots:
    void move_slot_handle();
    void slot_back();
    void scale_slot_handle();
    void rotate_slot_handle();

private:
    Ui::graphics *ui;
signals:

};
#endif // GRAPHICS_H
