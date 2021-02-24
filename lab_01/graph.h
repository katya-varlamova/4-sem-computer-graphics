#ifndef GRAPH_H
#define GRAPH_H

#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QPoint>
#include <form.h>
#include <structs.h>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui { class graph; }
QT_END_NAMESPACE

class graph : public QMainWindow
{
    Q_OBJECT
public:
    ~graph();
    void paintEvent(QPaintEvent *event);
    QPointF *points, center, *points_const;
    int points_num;
    int res_polygone;
    double x_circle, y_circle, radius;
    graph(QWidget *parent = nullptr);
    void solve();

private:
    bool is_on_line(QPointF p1, QPointF p2, QPointF p);
    bool is_min_tan(double, double, double, bool);
    void scale();
    void print_error(QString message);
public slots:
    void find_slot_handle();

private:
    Ui::graph *ui;
};
#endif // GRAPH_H
