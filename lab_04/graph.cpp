#include "graph.h"
#include "ui_graph.h"
#include <iostream>
graph::graph(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::graph)
{
    ui->setupUi(this);
    chart = new QChart();
    chart_view = new QChartView(chart);
}
graph::~graph()
{
    delete ui;
    delete chart;
    delete chart_view;
}
void graph::recieve_time_stat(std::vector <std::vector<std::pair<double, double>>> time,char** strs)
{
    chart->removeAllSeries();

    for (size_t i = 0; i < time.size(); i++)
    {
        QLineSeries *line_series = new QLineSeries(chart);
        for (size_t j = 0; j < time[i].size(); j++)
        {
            line_series->append(time[i][j].first, time[i][j].second);
        }
        line_series->setName(strs[i]);
        chart->addSeries(line_series);
    }
    chart_view->chart()->createDefaultAxes();
    chart_view->chart()->setTitle("dependence of time on radius");
    setCentralWidget(chart_view);
}
