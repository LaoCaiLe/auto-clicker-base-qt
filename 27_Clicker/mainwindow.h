#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void time_out();    //定时槽函数

private:
    unsigned int Counter_num;
    unsigned int Timer_num;
    unsigned int interval_time;

    QTimer *time_to_click;  //点击定时器
    QTimer *time_to_timer;  //定时器
    QTimer *time_to_clock;
    QLabel *label_time;

//    void keyPressEvent(QKeyEvent *e);

    Ui::MainWindow *ui;

signals:
    void start_click();
    void stop_click();
};

class Img :public QMainWindow
{
    Q_OBJECT
public:
    Img();
    ~Img();

private slots:
    void mouseMoveEvent(QMouseEvent *event);

private:

};

#endif // MAINWINDOW_H
