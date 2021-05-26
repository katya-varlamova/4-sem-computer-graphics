#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <vector>
#include <objects/polygone.h>
#include <objects/line.h>
#include <limits.h>
QT_BEGIN_NAMESPACE
namespace Ui { class Viewer; }
QT_END_NAMESPACE

class Viewer : public QMainWindow
{
    Q_OBJECT

public:
    Viewer(QWidget *parent = nullptr);
    ~Viewer();
protected:
    int mode;
    int line_direction;
    std::vector<std::shared_ptr<Line>> lines;
    std::vector<std::shared_ptr<Line>> res_lines;
    std::shared_ptr<Polygone> cutter;
//    std::vector<Point> points;
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);
    void print_error(QString message);
    bool cut(Point p1, Point p2, std::vector<Point> cutter, Point *r);
private slots:
    void on_add_line_but_clicked();

    void on_add_hor_line_but_clicked();

    void on_add_ver_line_but_clicked();

    void on_cut_but_clicked();

    void on_erase_lines_but_clicked();

    void on_erase_cutter_but_clicked();

    void on_cutter_mode_but_clicked();

    void on_lines_mode_but_clicked();

    void on_add_cutter_line_but_clicked();

    void on_add_hor_cutter_line_but_clicked();

    void on_finish_cutter_line_but_clicked();

    void on_add_ver_cutter_line_but_clicked();

private:
    Ui::Viewer *ui;
};
#endif // VIEWER_H
