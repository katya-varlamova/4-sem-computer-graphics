#include "viewer.h"
#include "ui_viewer.h"
#include <iostream>
void viewer::print_error(QString message)
{
    ui->textEdit->setTextColor(Qt::red);
    ui->textEdit->setText(message);
}
viewer::viewer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::viewer)
{
    ui->setupUi(this);

    algorithm = CANONICAL_EQ;
    color = RED;
    connect(this, SIGNAL(send_time_stat(std::vector<std::vector<std::pair<double,double> > >,char**)), &graph,
            SLOT(recieve_time_stat(std::vector<std::vector<std::pair<double,double> > >,char**)));

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
    for (size_t i = 0; i < ellipses.size(); i++)
    {
        draw_ellipse(params, ellipses[i]);
    }
}
void viewer::on_draw_circle_but_clicked()
{
    bool ok;
    int center_x = ui->center_x_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading spectrum center x");
        return;
    }
    int center_y = ui->center_y_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading spectrum center y");
        return;
    }
    int radius = ui->circle_radius_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading radius");
        return;
    }
    ellipse_t ellipse;
    ellipse.algo = algorithm;
    ellipse.color = color;
    ellipse.center = { center_x, center_y };
    ellipse.radius_hor = radius;
    ellipse.radius_ver = radius;
    ellipses.push_back(ellipse);
    print_error("");
    repaint();
}

void viewer::on_draw_ellipse_but_clicked()
{
    bool ok;
    int center_x = ui->center_x_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading center x");
        return;
    }
    int center_y = ui->center_y_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading center y");
        return;
    }
    int radius_hor = ui->ellipse_radius_hor_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading radius hor");
        return;
    }
    int radius_ver = ui->ellipse_radius_ver_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading radius ver");
        return;
    }
    ellipse_t ellipse;
    ellipse.algo = algorithm;
    ellipse.color = color;
    ellipse.center = { center_x, center_y };
    ellipse.radius_hor = radius_hor;
    ellipse.radius_ver = radius_ver;
    ellipses.push_back(ellipse);
    print_error("");
    repaint();
}

void viewer::on_draw_circle_spectrum_but_clicked()
{
    bool ok;
    int center_x = ui->spectrum_center_x_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading spectrum center x");
        return;
    }
    int center_y = ui->spectrum_center_y_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading spectrum center y");
        return;
    }
    int num = ui->obj_num_text->text().toInt(&ok);
    if (!ok || num < 1)
    {
        print_error("error reading objects number");
        return;
    }
    int radius = ui->circle_radius_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading radius");
        return;
    }
    int step = ui->step_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading step");
        return;
    }
    ellipse_t ellipse;
    for (int i = 1; i < num + 1; i++)
    {
        ellipse.algo = algorithm;
        ellipse.color = color;
        ellipse.center = { center_x, center_y };
        ellipse.radius_hor = radius;
        ellipse.radius_ver = radius;
        ellipses.push_back(ellipse);
        radius += step;
    }
    print_error("");
    repaint();
}

void viewer::on_draw_ellipse_spectrum_but_clicked()
{
    bool ok;
    int center_x = ui->spectrum_center_x_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading spectrum center x");
        return;
    }
    int center_y = ui->spectrum_center_y_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading spectrum center y");
        return;
    }
    int num = ui->obj_num_text->text().toInt(&ok);
    if (!ok || num < 1)
    {
        print_error("error reading objects number");
        return;
    }
    int radius_hor = ui->ellipse_radius_hor_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading radius hor");
        return;
    }
    int radius_ver = ui->ellipse_radius_ver_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading radius ver");
        return;
    }
    int step = ui->step_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading step");
        return;
    }
    ellipse_t ellipse;
    double exc = sqrt(1.0 - radius_ver * radius_ver / double(radius_hor) / double(radius_hor));
    for (int i = 1; i < num + 1; i++)
    {
        ellipse.algo = algorithm;
        ellipse.color = color;
        ellipse.center = { center_x, center_y };
        ellipse.radius_hor = radius_hor;
        ellipse.radius_ver = radius_ver;
        ellipses.push_back(ellipse);
        radius_hor += step;
        radius_ver = radius_hor * sqrt(1 - exc * exc);
    }
    print_error("");
    repaint();
}
void measure_time_for_algo(algorithm_t algo,
                           std::vector<std::pair<double, double>> &time_vec,
                           int radius_hor, int radius_ver)
{
    struct timeval tv_start, tv_stop;
    double N = 100;
    int center_x = 0;
    int center_y = 0;
    int num = 100;
    draw_params_t params;
    params.draw = false;
    params.painter = NULL;
    ellipse_t ellipse;
    ellipse.algo = algo;
    ellipse.color = RED;
    ellipse.center = { center_x, center_y };
    double exc = sqrt(1.0 - radius_ver * radius_ver / double(radius_hor) / double(radius_hor));
    for (int i = 1; i < num + 1; i++)
    {
        ellipse.radius_hor = radius_hor;
        ellipse.radius_ver = radius_ver;
        int time = 0;
        for (int j = 0; j < N; j++)
        {
            gettimeofday(&tv_start, NULL);
            draw_ellipse(params, ellipse);
            gettimeofday(&tv_stop, NULL);
            time += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
        }
        time /= N;
        time_vec.push_back(std::pair(radius_hor, time));
        radius_hor += 1;
        radius_ver = radius_hor * sqrt(1 - exc * exc);
    }
}
void viewer::on_circle_time_stat_but_clicked()
{
    bool ok;
    int radius = ui->circle_radius_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading radius");
        return;
    }
    std::vector<std::pair<double, double>> tmp;
    std::vector <std::vector<std::pair<double, double>>> time;
    measure_time_for_algo(CANONICAL_EQ, tmp, radius, radius);
    time.push_back(tmp);
    tmp.clear();

    measure_time_for_algo(PARAMETRIC_EQ, tmp, radius, radius);
    time.push_back(tmp);
    tmp.clear();

    measure_time_for_algo(BRES, tmp, radius, radius);
    time.push_back(tmp);
    tmp.clear();

    measure_time_for_algo(MIDPOINT, tmp, radius, radius);
    time.push_back(tmp);
    tmp.clear();

    const char chrs[4][15] = {"CANONICAL", "PARAMETRIC", "BRES", "MIDPOINT"};
    char **strs = (char **)malloc(sizeof(char *) * 4);
    for (int i = 0; i < 4; i++)
        strs[i] = (char *)chrs[i];
    emit send_time_stat(time, strs);
    free(strs);
    graph.show();
}
void viewer::on_ellipse_time_stat_but_clicked()
{
    bool ok;
    int radius_hor = ui->ellipse_radius_hor_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading radius hor");
        return;
    }
    int radius_ver = ui->ellipse_radius_ver_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading radius ver");
        return;
    }
    std::vector<std::pair<double, double>> tmp;
    std::vector <std::vector<std::pair<double, double>>> time;
    measure_time_for_algo(CANONICAL_EQ, tmp, radius_hor, radius_ver);
    time.push_back(tmp);
    tmp.clear();

    measure_time_for_algo(PARAMETRIC_EQ, tmp, radius_hor, radius_ver);
    time.push_back(tmp);
    tmp.clear();

    measure_time_for_algo(BRES, tmp, radius_hor, radius_ver);
    time.push_back(tmp);
    tmp.clear();

    measure_time_for_algo(MIDPOINT, tmp, radius_hor, radius_ver);
    time.push_back(tmp);
    tmp.clear();

    const char chrs[4][15] = {"CANONICAL", "PARAMETRIC", "BRES", "MIDPOINT"};
    char **strs = (char **)malloc(sizeof(char *) * 4);
    for (int i = 0; i < 4; i++)
        strs[i] = (char *)chrs[i];
    emit send_time_stat(time, strs);
    free(strs);
    graph.show();
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
void viewer::on_can_eq_but_clicked()
{
    algorithm = CANONICAL_EQ;
}
void viewer::on_par_eq_but_clicked()
{
    algorithm = PARAMETRIC_EQ;
}
void viewer::on_bres_but_clicked()
{
    algorithm = BRES;
}
void viewer::on_midpoint_but_clicked()
{
    algorithm = MIDPOINT;
}
void viewer::on_auto_but_clicked()
{
    algorithm = AUTO;
}

void viewer::on_erase_but_clicked()
{
    ellipses.clear();
    repaint();
}
