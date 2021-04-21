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
    mode = 0;
    paint_widget = new Paint_widget(270,
                                    0,
                                    this->geometry().width(),
                                    this->geometry().height(), this);

}

viewer::~viewer()
{
    delete ui;
}
void viewer::keyPressEvent(QKeyEvent *event)
{
    if (mode == 0)
        return;
    if (event->key() == Qt::Key_Return)
    {
        std::vector <int> time;
        paint_widget->finish_figure(time);
        char str[100];
        char buf[20];
        for (size_t i = 0; i < time.size(); i++)
        {
            sprintf(buf, "%zu. %d\n", i + 1, time[i]);
            strncat(&str[0], &buf[0], 100);
        }
        print_error(str);
    }
    if (event->key() == Qt::Key_Shift)
    {
        paint_widget->add_hole();
    }

}
void viewer::on_add_point_but_clicked()
{
    if (mode == 0)
        return;
    bool ok;
    int x = ui->x_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading next point x");
        return;
    }
    int y = ui->y_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading next point y");
        return;
    }
    point p = {x, y};
    paint_widget->add_point(p);
}
void viewer::on_add_hole_but_clicked()
{
    if (mode == 0)
        return;
    paint_widget->add_hole();
}
void viewer::on_fill_but_clicked()
{
    if (mode == 0)
        return;
    std::vector <int> time;
    paint_widget->finish_figure(time);
    char str[100];
    char buf[20];
    for (size_t i = 0; i < time.size(); i++)
    {
        sprintf(buf, "%zu. %d\n", i + 1, time[i]);
        strncat(&str[0], &buf[0], 100);
    }
    print_error(str);
}
void viewer::on_erase_but_clicked()
{
    paint_widget->erase();
}

void viewer::on_red_pen_but_clicked()
{
    paint_widget->set_pen_color(Qt::red);
}
void viewer::on_green_pen_but_clicked()
{
    paint_widget->set_pen_color(Qt::green);
}
void viewer::on_white_pen_but_clicked()
{
    paint_widget->set_pen_color(Qt::white);
}
void viewer::on_blue_pen_but_clicked()
{
    paint_widget->set_pen_color(Qt::gray);
}
void viewer::on_yellow_pen_but_clicked()
{
    paint_widget->set_pen_color(Qt::yellow);
}
void viewer::on_magenta_pen_but_clicked()
{
    paint_widget->set_pen_color(Qt::magenta);
}
void viewer::on_cyan_pen_but_clicked()
{
    paint_widget->set_pen_color(Qt::cyan);
}
void viewer::on_red_brush_but_clicked()
{
    paint_widget->set_brush_color(Qt::red);
}
void viewer::on_green_brush_but_clicked()
{
    paint_widget->set_brush_color(Qt::green);
}
void viewer::on_white_brush_but_clicked()
{
    paint_widget->set_brush_color(Qt::white);
}
void viewer::on_bacground_brush_but_clicked()
{
    paint_widget->set_brush_color(QColor(50, 50, 50));
}
void viewer::on_blue_brush_but_clicked()
{
    paint_widget->set_brush_color(Qt::gray);
}
void viewer::on_yellow_brush_but_clicked()
{
    paint_widget->set_brush_color(Qt::yellow);
}
void viewer::on_magenta_brush_but_clicked()
{
    paint_widget->set_brush_color(Qt::magenta);
}
void viewer::on_cyan_brush_but_clicked()
{
    paint_widget->set_brush_color(Qt::cyan);
}
void viewer::on_with_delay_but_clicked()
{
    bool ok;
    int time = ui->delay_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading delay time");
        return;
    }
    paint_widget->set_delay(true, time);
}

void viewer::on_without_delay_but_clicked()
{
    paint_widget->set_delay(false);
}

void viewer::on_delay_text_textChanged(const QString &arg1)
{
    int time;
    if (ui->with_delay_but->isChecked())
    {
        bool ok;
        time = arg1.toInt(&ok);
        if (!ok)
        {
            print_error("error reading delay time");
            return;
        }
        paint_widget->set_delay(true, time);
        print_error("");
    }
}

void viewer::on_set_seed_but_clicked()
{
    bool ok;
    int seed_x = ui->x_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading seed x");
        return;
    }
    int seed_y = ui->y_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading seed x");
        return;
    }
    point p = {seed_x, seed_y};
    paint_widget->change_seed(p);
}

void viewer::on_seed_mode_but_clicked()
{
    mode = 0;
    paint_widget->mode = 0;
}

void viewer::on_draw_mode_but_clicked()
{
    mode = 1;
    paint_widget->mode = 1;
}

void viewer::on_add_horisontal_but_clicked()
{
    bool ok;
    int delta = ui->delta_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading horisontal delta");
        return;
    }
    if (delta == 0)
    {
        print_error("delta must be != 0");
        return;
    }
    point prev = paint_widget->get_last_point();
    if (prev.x == -1 && prev.y == -1)
    {
        print_error("there is no first point");
        return;
    }
    point p = {prev.x + delta, prev.y};
    paint_widget->add_point(p);
}

void viewer::on_add_vertical_but_clicked()
{
    bool ok;
    int delta = ui->delta_text->text().toInt(&ok);
    if (!ok)
    {
        print_error("error reading horisontal delta");
        return;
    }
    if (delta == 0)
    {
        print_error("delta must be != 0");
        return;
    }
    point prev = paint_widget->get_last_point();
    if (prev.x == -1 && prev.y == -1)
    {
        print_error("there is no first point");
        return;
    }
    point p = {prev.x, prev.y + delta};
    paint_widget->add_point(p);
}
