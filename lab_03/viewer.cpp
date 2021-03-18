#include "viewer.h"
#include "ui_viewer.h"
#include <iostream>
void viewer::print_error(QString message)
{
    ui->textEdit->setTextColor(Qt::red);
    ui->textEdit->setText(message);
}
double to_rad(double ang)
{
    double ret = ang * (M_PI / 180.0);
    return ret;
}
viewer::viewer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::viewer)
{
    ui->setupUi(this);

    algorithm = DDA;
    color = RED;
    connect(this, SIGNAL(send_time_stat(double*,int,char**)), &graph,
            SLOT(recieve_time_stat(double*,int,char**)));
    connect(this, SIGNAL(send_graph_params(double*,int*,int,char*)), &graph,
            SLOT(recieve_graph_params(double*,int*,int,char*)));


}

viewer::~viewer()
{
    delete ui;
}
void viewer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    draw_params_t params;
    params.draw = true;
    params.painter = &painter;
    for (size_t i = 0; i < lines.size(); i++)
    {
        draw_line(params, lines[i]);
    }
}
void viewer::on_draw_line_but_clicked()
{
    bool ok;
    int start_x = ui->start_x_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading start x");
        return;
    }
    int start_y = ui->start_y_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading start y");
        return;
    }
    int end_x = ui->end_x_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading end x");
        return;
    }
    int end_y = ui->end_y_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading end y");
        return;
    }
    print_error("");
    line_t line;
    line.algo = algorithm;
    line.color = color;
    line.p1 = { start_x, start_y };
    line.p2 = { end_x, end_y };
    lines.push_back(line);
    lines_num_stack.push(1);
    repaint();
}
void viewer::on_draw_spectrum_but_clicked()
{
    bool ok;
    int center_x = ui->spectrum_center_x_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading center x");
        return;
    }
    int center_y = ui->spectrum_center_y_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading center y");
        return;
    }
    int lines_num = ui->spectrum_lines_num_text->text().toInt(&ok);
    if (!ok || lines_num < 1)
    {
        print_error("error reading lines number");
        return;
    }
    int radius = ui->radius_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading radius");
        return;
    }
    line_t line;
    double angle = 360.0 / lines_num;
    for (int i = 0; i < lines_num; i++)
    {
        line.algo = algorithm;
        line.color = color;
        line.p1 = { center_x, center_y };
        line.p2 = { center_x + int(round(radius * cos(to_rad(angle * i)))),
                    center_y + int(round(radius * sin(to_rad(angle * i)))) };
        lines.push_back(line);
    }
    lines_num_stack.push(lines_num);
    print_error("");
    repaint();
}
void viewer::on_time_stat_but_clicked()
{
    bool ok;
    int start_x = ui->start_x_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading start x");
        return;
    }
    int start_y = ui->start_y_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading start y");
        return;
    }
    int end_x = ui->end_x_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading end x");
        return;
    }
    int end_y = ui->end_y_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading end y");
        return;
    }
    print_error("");
    line_t line;
    line.color = color;
    line.p1 = { start_x, start_y };
    line.p2 = { end_x, end_y };
    draw_params_t params;
    params.draw = false;
    params.painter = NULL;

    double *time_stat = (double *)malloc(6 * sizeof (double));
    int n = 6;
#define N 1000

    for (int i = 0; i < n; i++)
        time_stat[i] = 0;

    line.algo = DDA;
    for (int i = 0; i < N; i++)
    {
        draw_line(params, line);
        time_stat[0] += params.time;
    }
    time_stat[0] /= N;

    line.algo = BRES_FLOAT;
    for (int i = 0; i < N; i++)
    {
        draw_line(params, line);
        time_stat[1] += params.time;
    }
    time_stat[1] /= N;

    line.algo = BRES_INT;
    for (int i = 0; i < N; i++)
    {
        draw_line(params, line);
        time_stat[2] += params.time;
    }
    time_stat[2] /= N;

    line.algo = BRES_ALIASED;
    for (int i = 0; i < N; i++)
    {
        draw_line(params, line);
        time_stat[3] += params.time;
    }
    time_stat[3] /= N;

    line.algo = AUTO;
    draw_line(params, line);
    time_stat[4] = params.time;

    line.algo = VU;
    for (int i = 0; i < N; i++)
    {
        draw_line(params, line);
        time_stat[5] += params.time;
    }
    time_stat[5] /= N;

    const char chrs[6][15] = {"DDA", "BRES FLOAT", "BRES INT", "BRES ALIASED", "AUTO", "VU"};
    char **strs = (char **)malloc(sizeof(char *) * 6);
    for (int i = 0; i < 6; i++)
        strs[i] = (char *)chrs[i];
    emit send_time_stat(time_stat, n, strs);
    free(strs);
    graph.show();

    free(time_stat);
}
void viewer::on_stepwise_comp_but_clicked()
{
    bool ok;
    int radius = ui->radius_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading radius");
        return;
    }
    line_t line;
    line.color = color;
    line.algo = algorithm;
    draw_params_t params;
    params.draw = false;
    params.painter = NULL;
    int center_x = 500, center_y = 500;
    double angle = 1;
    int *x = (int *) malloc(360 * sizeof (int));
    double *y = (double *) malloc(360 * sizeof (double));
    for (int i = 0; i < 361; i++)
    {
        line.p1 = { center_x, center_y };
        line.p2 = { center_x + int(round(radius * cos(to_rad(angle * i)))),
                    center_y + int(round(radius * sin(to_rad(angle * i)))) };
        draw_line(params, line);
        x[i] = i;
        y[i] = params.num_of_grad;
    }
    const char chrs[6][15] = {"DDA", "BRES FLOAT", "BRES INT", "BRES ALIASED", "VU", "AUTO"};
    char *name = (char *) malloc(60);
    snprintf(name, 60, "%s algo; length of string: %d", chrs[int(algorithm)], radius);
    emit send_graph_params(y, x, 361, name);
    graph.show();
    free(x);
    free(y);
    free(name);
}
void viewer::on_dda_but_clicked()
{
    algorithm = DDA;
}
void viewer::on_bres_float_but_clicked()
{
    algorithm = BRES_FLOAT;
}
void viewer::on_bres_int_but_clicked()
{
    algorithm = BRES_INT;
}
void viewer::on_bres_aliased_but_clicked()
{
    algorithm = BRES_ALIASED;
}
void viewer::on_vu_but_clicked()
{
    algorithm = VU;
}
void viewer::on_autodraw_but_clicked()
{
    algorithm = AUTO;
}
void viewer::on_red_but_clicked()
{
    color = RED;
}
void viewer::on_green_but_clicked()
{
    color = GREEN;
}
void viewer::on_white_but_clicked()
{
    color = WHITE;
}
void viewer::on_bacground_but_clicked()
{
    color = BACKGROUND;
}
void viewer::on_blue_but_clicked()
{
    color = GRAY;
}
void viewer::on_yellow_but_clicked()
{
    color = YELLOW;
}
void viewer::on_magenta_but_clicked()
{
    color = MAGENTA;
}
void viewer::on_cyan_but_clicked()
{
    color = CYAN;
}
void viewer::on_back_but_clicked()
{
    if (lines_num_stack.empty())
        return;
    int n = lines_num_stack.top();
    lines_num_stack.pop();
    for (int i = 0; i < n; i++)
        lines.pop_back();
    repaint();
}
