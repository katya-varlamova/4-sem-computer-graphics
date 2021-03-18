#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>
#include <QPainter>
#include <math.h>
#include <vector>
#include <stack>
#include "graph.h"
#include "logic_domain/task_manager.h"

typedef struct
{
    size_t x_min, y_min, x_max, y_max;
} window_sizes_t;

QT_BEGIN_NAMESPACE
namespace Ui { class viewer; }
QT_END_NAMESPACE

class viewer : public QMainWindow
{
    Q_OBJECT

public:
    color_t color;
    algorithm_t algorithm;
    window_sizes_t sizes;
    std::vector <line_t> lines;
    std::stack <int> lines_num_stack;
    viewer(QWidget *parent = nullptr);
    ~viewer();
    void print_error(QString message);
    void paintEvent(QPaintEvent *event);
    graph graph;
private slots:
    void on_dda_but_clicked();

    void on_bres_float_but_clicked();

    void on_bres_int_but_clicked();

    void on_bres_aliased_but_clicked();

    void on_vu_but_clicked();

    void on_autodraw_but_clicked();

    void on_red_but_clicked();

    void on_green_but_clicked();

    void on_white_but_clicked();

    void on_bacground_but_clicked();

    void on_draw_line_but_clicked();

    void on_draw_spectrum_but_clicked();

    void on_time_stat_but_clicked();

    void on_stepwise_comp_but_clicked();

    void on_back_but_clicked();

    void on_blue_but_clicked();

    void on_yellow_but_clicked();

    void on_magenta_but_clicked();

    void on_cyan_but_clicked();

signals:
    void send_time_stat(double *, int, char **);

    void send_graph_params(double *y, int *x, int n, char *);

private:
    Ui::viewer *ui;
};
#endif // VIEWER_H
