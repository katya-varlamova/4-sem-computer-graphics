#include "graph.h"
#include "ui_graph.h"
#include <iostream>
graph::graph(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::graph)
{
    ui->setupUi(this);
    chart = new QChart();
    bar_series = new QBarSeries(chart);
    line_series = new QLineSeries(chart);
    chart_view = new QChartView(chart);
}
graph::~graph()
{
    delete ui;
    delete chart;
    delete chart_view;
}
void graph::recieve_time_stat(double *time_stat, int n, char **strs)
{
    if (n == 0)
        return;
    chart->removeSeries(bar_series);
    chart->removeSeries(line_series);
    bar_series->clear();
    double max = time_stat[0];
    for (int i = 0; i < n; i++)
    {
        if (max < time_stat[i])
            max = time_stat[i];
        QBarSet *set = new QBarSet(strs[i], bar_series);
        *set << time_stat[i];
        bar_series->append(set);
    }
    chart->addSeries(bar_series);
    chart_view->chart()->createDefaultAxes();
    chart_view->chart()->setTitle("algorithm execution time (in ms)");
    setCentralWidget(chart_view);
}
void graph::recieve_graph_params(double *y, int *x, int n, char *name)
{
    if (n == 0)
        return;
    chart->removeSeries(bar_series);
    chart->removeSeries(line_series);
    line_series->clear();
    double max = y[0];
    for (int i = 0; i < n; i++)
    {
        if (max < y[i])
            max = y[i];
        line_series->append(x[i], y[i]);
    }
    line_series->setName(name);
    chart->addSeries(line_series);
    chart_view->chart()->createDefaultAxes();
    chart_view->chart()->setTitle("dependence of the number of steps on the angle of inclination of the line");
    setCentralWidget(chart_view);
}
