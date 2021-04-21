#include "paint_widget.h"
#include <iostream>
Paint_widget::Paint_widget(int x, int y, int width, int height, QWidget *parent) : QWidget(parent)
{
    int widget_width = width;
    int widget_height = height;
    pen_color = Qt::red;
    brush_color = Qt::green;
    bg_color = QColor(50, 50, 50);
    delay = true;
    holes = 0;
    sleep_time = 15;

    image = new QImage(widget_width, widget_height, QImage::Format_RGB32);
    image->fill(bg_color);
    setGeometry(x, y, widget_width, widget_height);

    seed = {500, 300};
    mode = 0;
}
void Paint_widget::redrawImage()
{
    image->fill(bg_color);
    QPainter painter(image);
    painter.setPen(pen_color);
    painter.drawPoint(seed.x, seed.y);
    for (size_t i = 0; i < polygones.size(); i++)
    {
        draw_borders(&painter, polygones[i].edges);
    }
    update();
}
void Paint_widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(0, 0, *image);
    Q_UNUSED(event);
}
void Paint_widget::change_seed(point p)
{
    seed = p;
    redrawImage();
}
point Paint_widget::get_last_point()
{
    if (polygones.empty() || polygones.back().edges.empty())
        return {-1, -1};
    return polygones.back().edges.back().p2;
}
void Paint_widget::mousePressEvent(QMouseEvent *event)
{
    point p = {(int)(event->position().x()), (int)(event->position().y())};
    if (mode == 1)
        add_point(p);
    else
        change_seed(p);
}
void Paint_widget::finish_figure(std::vector <int> &time)
{
    if (polygones.empty())
        return;
    if (polygones.back().edges.empty())
    {
        holes--;
        polygones.pop_back();
    }
    else
        add_point(polygones.back().edges.front().p1);
    std::stack <edge_t> holes_stack;
    for (int i = 0; i < holes; i++)
    {
        for (size_t k = 0; k < polygones.back().edges.size(); k++)
        {
            holes_stack.push(polygones.back().edges[k]);
        }
        polygones.pop_back();
    }
    holes = 0;
    size_t stk_size = holes_stack.size();;
    for (size_t i = 0; i < stk_size; i++)
    {
        polygones.back().edges.push_back(holes_stack.top());
        holes_stack.pop();
    }

    polygones.back().finished = true;

    time.clear();
    int polygine_time;
    for (size_t i = 0; i < polygones.size(); i++)
    {
        fill(polygones[i], seed, polygine_time);
        time.push_back(polygine_time);
    }

    polygone_t polygone;
    polygones.push_back(polygone);
    polygones.back().finished = false;
}
void Paint_widget::add_hole()
{
    if (polygones.size() == 0)
        return;
    if (polygones.back().edges.empty())
        return;
    add_point(polygones.back().edges.front().p1);
    holes++;
    polygone_t polygone;
    polygones.push_back(polygone);
    polygones.back().finished = false;
}
void Paint_widget::add_point(point &p)
{
    if (polygones.empty())
    {
        polygone_t polygone;
        polygones.push_back(polygone);
        polygones.back().finished = false;
    }
    if (polygones.back().edges.empty())
    {
        edge_t edge;
        polygones.back().edges.push_back(edge);
        polygones.back().edges.back().p1 = p;
        polygones.back().edges.back().p2 = p;
        redrawImage();
        return;
    }
    if (polygones.back().edges.back().p1.x == polygones.back().edges.back().p2.x &&
         polygones.back().edges.back().p1.y == polygones.back().edges.back().p2.y)
    {
        polygones.back().edges.back().p2 = p;
        redrawImage();
        return;
    }
    edge_t edge = {polygones.back().edges.back().p2, p};
    polygones.back().edges.push_back(edge);
    redrawImage();
}
void Paint_widget::draw_borders(QPainter *painter, const std::vector <edge_t> &edges)
{
    if (painter == NULL)
        return;
    for (size_t i = 0; i < edges.size(); i++)
    {
        bresenham_int(painter, edges[i].p1, edges[i].p2);
    }
}
void Paint_widget::draw_point(QPainter *painter, int x, int y)
{
    if (painter == NULL)
        return;
    painter->drawPoint(x, y);
}
int Paint_widget::sign(double c)
{
    if (c < -1e-6)
        return -1;
    else
        return 1;
}
void Paint_widget::bresenham_int(QPainter *painter, const point &p_b, const point &p_e)
{
    if (p_b.x == p_e.x && p_b.y == p_e.y)
    {
        draw_point(painter, round(p_b.x), round(p_b.y));
        return;
    }
    point cur;
    int dx, dy;
    bool fl = 0;
    int sx, sy, f;
    dx = p_e.x - p_b.x;
    dy = p_e.y - p_b.y;
    sx = sign(dx);
    sy = sign(dy);
    dx = abs(dx);
    dy = abs(dy);
    if (dy >= dx)
    {
        int tmp = dx;
        dx = dy;
        dy = tmp;
        fl = 1;
    }
    f = 2 * dy - dx;
    cur = p_b;
    int len = dx;
    for (int i = 0; i < len; i++)
    {
        draw_point(painter, cur.x, cur.y);
        if (f >= 0)
        {
            if (fl == 1)
                cur.x += sx;
            else
                cur.y += sy;
            f -= 2 * dx;
        }
        if (f < 0)
        {
            if (fl == 1)
                cur.y += sy;
            else
                cur.x += sx;
        }
        f += 2 * dy;
    }
}
void Paint_widget::fill(const polygone_t &polygone, point &p, int &time)
{
    time = 0;
    if (polygone.edges.empty() || polygone.finished == false)
        return;

    QPainter painter(image);
    painter.setPen(brush_color);

    std::stack<point> stack;
    stack.push(p);
    while (!stack.empty())
    {
        point p = stack.top();
        stack.pop();
        draw_point(&painter, p.x, p.y);
        point tmp = p;
        p.x++;
        int left_border, right_border;
        while (image->pixelColor(p.x, p.y) != pen_color)
        {
            draw_point(&painter, p.x, p.y);
            p.x++;
        }
        right_border = p.x - 1;
        p = tmp;
        p.x--;
        while (image->pixelColor(p.x, p.y) != pen_color)
        {
            draw_point(&painter, p.x, p.y);
            p.x--;
        }
        left_border = p.x + 1;


        p.y++;
        for (size_t i = 0; i < 2; i++)
        {
            p.x = left_border;
            bool found;
            while (p.x <= right_border)
            {
                found = false;
                while (image->pixelColor(p.x, p.y) != brush_color &&
                       image->pixelColor(p.x, p.y) != pen_color &&
                       p.x <= right_border)
                {
                    found = true;
                    p.x++;
                }
                if (found)
                {
                    p.x--;
                    stack.push(p);
                    p.x++;
                }
                while ((image->pixelColor(p.x, p.y) == brush_color ||
                       image->pixelColor(p.x, p.y) == pen_color) &&
                       p.x <= right_border)
                    p.x++;
            }
            p.y -= 2;
            if (delay)
            {
                sleepFeature();
                update();
            }
        }

    }

    update();
}
void Paint_widget::sleepFeature()
{
    QTime end = QTime::currentTime().addMSecs(sleep_time);
    while (QTime::currentTime() < end)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }
    return;
}
void Paint_widget::set_brush_color(const QColor &color)
{
    brush_color = color;
    redrawImage();
}
void Paint_widget::set_pen_color(const QColor &color)
{
    pen_color = color;
    redrawImage();
}
void Paint_widget::set_delay(bool delay, int time)
{
    this->delay = delay;
    if (delay)
        sleep_time = time;
}
void Paint_widget::erase()
{
    polygones.clear();
    redrawImage();
}
