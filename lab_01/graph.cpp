#include "graph.h"
#include "ui_graph.h"
#include <iostream>
#include <stdlib.h>

static int graph_x_pos = 0, graph_y_pos = 0, graph_height = 0, graph_width = 0;
polygones_t polygones;
axis_t axis;
graph::~graph()
{
    delete ui;
    free(points);
    free(points_const);
    free(polygones.polygones);
}

bool graph::is_on_line(QPointF p1, QPointF p2, QPointF p)
{
    double x1 = p1.x(), y1 = p1.y();
    double x2 = p2.x(), y2= p2.y();
    double x = p.x(), y = p.y();
    return (y1 - y2) * x + (x2 - x1) * y + (x1 * y2 - x2 * y1) == 0.0 ?
                true
              :
                false;

}
bool graph::is_min_tan(double min_tan, double dx, double dy, bool is_set)
{
    if (is_set == false)
        return true;
    if (dx == 0)
        return min_tan < 0 ? true : false;
    if (dy / dx < 0 && min_tan >= 0)
        return false;
    if (dy / dx > 0 && min_tan <= 0)
        return true;
    return dy / dx < min_tan ? true : false;
}
void graph::solve()
{
    res_polygone = -1;
    double min_tan = 0;
    bool is_set = false;
    for (int i = 0; i < points_num; i++)
    {
        for (int j = i + 1; j < points_num; j++)
        {
            if (is_on_line(points[i],points[j], QPointF(x_circle, y_circle)))
            {
                for (int k = 0; k < points_num; k++)
                {
                    if (k == i || k == j)
                        continue;
                    if (!is_on_line(points[i], points[j], points[k]))
                    {
                        polygones.polygones[polygones.pol_num].points_num = 3;
                        polygones.polygones[polygones.pol_num].points[0] = i;
                        polygones.polygones[polygones.pol_num].points[1] = j;
                        polygones.polygones[polygones.pol_num].points[2] = k;
                        polygones.pol_num++;
                        break;
                    }
                }

                double dx = points[j].x() - points[i].x();
                double dy = points[j].y() - points[i].y();
                if (is_min_tan(min_tan, dx, dy, is_set))
                {
                    is_set = true;
                    if (dx == 0)
                        min_tan = 0;
                    else
                        min_tan = dy / dx;
                    if (polygones.pol_num)
                        res_polygone = polygones.pol_num - 1;
                }

            }
        }
    }
    if (res_polygone == -1)
    {
        print_error("it is impossible to draw at least one triangle corresponding to task");
        return;
    }
}
void graph::print_error(QString message)
{
    ui->textEdit->setText(message);
}
void graph::find_slot_handle()
{
    bool ok;
    double x, y;
    polygones.pol_num  = 0;
    points_num = 0;
    ui->result->setText("");
    for (int j = 0; j < 10; j++)
    {
        QString x_str = ui->tableWidget->item(j, 0)->text();
        QString y_str = ui->tableWidget->item(j, 1)->text();
        if (x_str.isEmpty() && y_str.isEmpty())
            continue;
        if (x_str.isEmpty() || y_str.isEmpty())
        {
            char *str;
            asprintf(&str, "error reading point: x or y of %d point is empty", j + 1);
            print_error(str);
            free(str);
            return;
        }

        x = x_str.toDouble(&ok);
        if (!ok)
        {
            char *str;
            asprintf(&str, "error reading point %d: x is incorrect", j + 1);
            print_error(str);
            free(str);
            return;
        }
        y = y_str.toDouble(&ok);
        if (!ok)
        {
            char *str;
            asprintf(&str, "error reading point %d: y is incorrect", j + 1);
            print_error(str);
            free(str);
            return;
        }
        points[points_num].setX(x);
        points[points_num].setY(y);
        points_const[points_num].setX(x);
        points_const[points_num].setY(y);
        points_num++;
    }
    x_circle = ui->x_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading circle center x");
        return;
    }
    y_circle = ui->y_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading circle center y");
        return;
    }
    radius = ui->r_text->text().toDouble(&ok);
    if (!ok)
    {
        print_error("error reading circle radius");
        return;
    }
    print_error("");
    solve();
    if (res_polygone != -1)
    {
        char *str;
        asprintf(&str,
                "the desired triangle has coordinates:\n1. (%.1lf, %.1lf), vertex %d\n2. (%.1lf, %.3lf), vertex %d\n3. (%.3lf, %.3lf), vertex %d",
                points[polygones.polygones[res_polygone].points[0]].x(),
                points[polygones.polygones[res_polygone].points[0]].y(), polygones.polygones[res_polygone].points[0] + 1,
                points[polygones.polygones[res_polygone].points[1]].x(),
                points[polygones.polygones[res_polygone].points[1]].y(), polygones.polygones[res_polygone].points[1] + 1,
                points[polygones.polygones[res_polygone].points[2]].x(),
                points[polygones.polygones[res_polygone].points[2]].y(), polygones.polygones[res_polygone].points[2] + 1
                );
        ui->result->setText(str);
        free(str);
    }
    scale();
    repaint();
}

void graph::scale()
{
    std::cout << "gr h: " << graph_height << " gr w: " << graph_width << std::endl;

    if (polygones.pol_num == 0)
        return;
    double min_x = points[0].x(), max_x = points[0].x();
    double min_y = points[0].y(), max_y = points[0].y();
    for (int i = 0; i < polygones.polygones->points_num; i++)
        for (int j = 0; j < polygones.polygones[i].points_num; j++)
        {
            if (points[polygones.polygones[i].points[j]].x() < min_x)
                min_x = points[polygones.polygones[i].points[j]].x();
            if (points[polygones.polygones[i].points[j]].x() > max_x)
                max_x = points[polygones.polygones[i].points[j]].x();

            if (points[polygones.polygones[i].points[j]].y() < min_y)
                min_y = points[polygones.polygones[i].points[j]].y();
            if (points[polygones.polygones[i].points[j]].y() > max_y)
                max_y = points[polygones.polygones[i].points[j]].y();
        }
    if (x_circle - radius < min_x)
        min_x = x_circle - radius;
    if (x_circle + radius > max_x)
        max_x = x_circle + radius;
    if (y_circle - radius < min_y)
        min_y = y_circle - radius;
    if (y_circle + radius > max_y)
        max_y = y_circle + radius;
    if (min_x > 0)
        min_x = 0;
    if (max_x < 0)
        max_x = 0;
    if (min_y > 0)
        min_y = 0;
    if (max_y < 0)
        max_y = 0;
    max_x -= min_x;
    max_y -= min_y;
    double kx = graph_width / max_x;
    double ky = graph_height / max_y;
    double k = std::min(kx, ky);
    if (min_x < 0)
    {
        min_x *= k;
        min_x = abs(min_x);
    }
    else
        min_x *= k;
    if (min_y < 0)
    {
        min_y *= k;
        min_y = abs(min_y);
    }
    else
        min_y = 0;
    center.setX(min_x + graph_x_pos);
    center.setY(graph_height - min_y + graph_y_pos);
    for (int i = 0; i < points_num; i++)
    {
        points[i].setX(k * points[i].x() + center.x());
        points[i].setY(center.y() - k * points[i].y());
    }
    x_circle = k * x_circle + min_x + graph_x_pos;
    y_circle = graph_height - k * y_circle - min_y + graph_y_pos;
    radius *= k;
}
graph::graph(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::graph)
{
    ui->setupUi(this);
    res_polygone = -1;
    graph_height = ui->groupBox->height();
    graph_width = ui->groupBox->width();
    graph_x_pos = ui->groupBox->x();
    graph_y_pos = ui->groupBox->y();
    ui->textEdit->setTextColor(Qt::red);
    ui->result->setTextColor(Qt::green);
    center.setX(graph_x_pos);
    center.setY(graph_y_pos + graph_height);
    points = (QPointF *) malloc(10 * sizeof(QPointF));
    points_const = (QPointF *) malloc(10 * sizeof(QPointF));
    points_num = 0;
    polygones.polygones = (polygone_t *) calloc(100, sizeof(polygone_t));
    polygones.pol_num = 0;
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 2; j++)
        {
            if (!ui->tableWidget->item(i, j))
                ui->tableWidget->setItem(i, j, new QTableWidgetItem(""));
        }
    connect(ui->pushButton, SIGNAL (clicked()), this, SLOT (find_slot_handle()));
}
void graph::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter ( this );
    painter.setPen(QPen(Qt::darkBlue, 2, Qt::SolidLine));
    painter.setBrush(QBrush(Qt::darkBlue));
    if (polygones.pol_num)
    {
        painter.drawEllipse(QPointF(x_circle, y_circle), radius, radius);
    }
    painter.setPen(QPen(Qt::black, 3, Qt::SolidLine));
    painter.drawLine(center.x(), graph_y_pos, center.x(), graph_y_pos + graph_height);
    painter.drawLine(graph_x_pos, center.y(), graph_x_pos + graph_width, center.y());

    painter.setPen(QPen(Qt::darkBlue, 2, Qt::SolidLine));
    painter.setBrush(QBrush(Qt::darkBlue));
    QPointF pol[3];
    for (int p = 0; p < polygones.pol_num; p++)
    {
        painter.setPen(QPen(Qt::darkBlue, 2, Qt::SolidLine));

        pol[0] = points[polygones.polygones[p].points[0]];
        pol[1] = points[polygones.polygones[p].points[1]];
        pol[2] = points[polygones.polygones[p].points[2]];
        if (p == res_polygone)
        {
            painter.setPen(QPen(Qt::darkGreen, 2, Qt::SolidLine));
            painter.setBrush(QBrush(Qt::darkGreen));
            painter.drawPolygon(pol, polygones.polygones[p].points_num);
            painter.setPen(QPen(Qt::darkBlue, 2, Qt::SolidLine));
            painter.setBrush(QBrush(Qt::darkBlue));
        }
        else
            painter.drawPolygon(pol, polygones.polygones[p].points_num);

        painter.setPen(QPen(Qt::black, 3, Qt::SolidLine));
        painter.drawLine(QPointF(points[polygones.polygones[p].points[0]]), QPointF(points[polygones.polygones[p].points[1]]));
        painter.drawLine(QPointF(points[polygones.polygones[p].points[0]]), QPointF(x_circle, y_circle));
        painter.setPen(QPen(Qt::darkBlue, 2, Qt::SolidLine));

        for (int k = 0; k < polygones.polygones[p].points_num; k++)
        {
            char *str;
            asprintf(&str, "%d: (%.1lf, %.1lf)",
                    polygones.polygones[p].points[k] + 1,
                    points_const[polygones.polygones[p].points[k]].x(),
                    points_const[polygones.polygones[p].points[k]].y());
            painter.setPen(QPen(Qt::white, 5, Qt::SolidLine));
            painter.drawText(points[polygones.polygones[p].points[k]], str);
            painter.setPen(QPen(Qt::darkBlue, 2, Qt::SolidLine));
        }
    }
    painter.setPen(QPen(Qt::black, 3, Qt::SolidLine));
    painter.drawLine(center.x(), graph_y_pos, center.x() - 5, graph_y_pos + 10);
    painter.drawLine(center.x(), graph_y_pos, center.x() + 5, graph_y_pos + 10);
    painter.drawLine(graph_x_pos + graph_width, center.y(), graph_x_pos + graph_width - 10, center.y() + 5);
    painter.drawLine(graph_x_pos + graph_width, center.y(), graph_x_pos + graph_width - 10, center.y() - 5);

    painter.setPen(QPen(Qt::darkBlue, 2, Qt::SolidLine));
    painter.setBrush(QBrush(Qt::darkBlue));
    if (polygones.pol_num)
    {
        char *str;
        asprintf(&str, "(%.1lf, %.1lf)", ui->x_text->text().toDouble(), ui->y_text->text().toDouble());
        painter.setPen(QPen(Qt::white, 5, Qt::SolidLine));
        painter.drawText(QPointF(x_circle, y_circle), str);
        painter.setPen(QPen(Qt::darkBlue, 2, Qt::SolidLine));
        free(str);
    }
    painter.drawPoint(QPointF(x_circle, y_circle));
}
