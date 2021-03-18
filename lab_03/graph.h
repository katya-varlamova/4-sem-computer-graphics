#ifndef GRAPH_H
#define GRAPH_H
#include <QtCharts.h>
#include <QMainWindow>

namespace Ui {
class graph;
}

class graph : public QMainWindow
{
    Q_OBJECT

public:
    explicit graph(QWidget *parent = nullptr);
    ~graph();
    QBarSeries *bar_series;
    QLineSeries *line_series;
    QChart *chart;
    QChartView *chart_view;
public slots:
    void recieve_time_stat(double *time_stat, int n, char **strs);

    void recieve_graph_params(double *y, int *x, int n, char *);
private:
    Ui::graph *ui;
};

#endif // GRAPH_H
