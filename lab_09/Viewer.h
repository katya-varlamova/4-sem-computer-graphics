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
    std::shared_ptr<Polygone> cutter;
    std::shared_ptr<Polygone> polygone;
    std::shared_ptr<Polygone> res_polygone;
    std::vector<Line> lumber;
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);
    void print_error(QString message);
    bool cut(std::vector<Point> &polygone, std::vector<Point> &cutter);
private slots:
    void on_cut_but_clicked();

    void on_erase_cutter_but_clicked();

    void on_cutter_mode_but_clicked();

    void on_add_cutter_line_but_clicked();

    void on_add_hor_cutter_line_but_clicked();

    void on_finish_cutter_line_but_clicked();

    void on_add_ver_cutter_line_but_clicked();

    void on_add_polygone_line_but_clicked();

    void on_add_hor_polygone_line_but_clicked();

    void on_add_ver_polygone_line_but_clicked();

    void on_finish_polygone_but_clicked();

    void on_erase_polygone_but_clicked();

    void on_lines_mode_but_clicked();

private:
    Ui::Viewer *ui;
};
#endif // VIEWER_H
