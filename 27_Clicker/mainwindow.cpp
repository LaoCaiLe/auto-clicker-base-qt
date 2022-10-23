#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QString>
#include <QDateTime>
#include <QFont>
#include <QTimer>
#include <QMouseEvent>
#include <QEvent>

#include "Windows.h"//这是引入Windows操作系统的API
#include "WinUser.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QPainter>

int clickpoint_x;
int clickpoint_y;
QLabel *aim;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/img/icon.png"));
    setWindowTitle(QString("Clicker"));

    //定时发送信号给time_out函数显示时间
    time_to_click = new QTimer(this);
    time_to_timer = new QTimer(this);
    time_to_clock = new QTimer(this);

    //sign
    QLabel *label_sign = new QLabel("Laocaile",this);
    QFont font;
    font.setPointSize(12);
    label_sign->setFont(font);
    label_sign->move(this->width()-label_sign->width(),this->height()-label_sign->height());

    //时间显示
    time_to_clock->start(500);
    label_time = new QLabel(this);
    label_time->setFont(font);
    label_time->move(0,this->height()-label_time->height());
    connect(time_to_clock,&QTimer::timeout,this,&MainWindow::time_out);

    //鼠标点击定时
    connect(time_to_click,&QTimer::timeout,[=](){      
        qDebug()<<"click";
        mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTDOWN,clickpoint_x, clickpoint_y, 0, 0);//按下
        mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTUP,clickpoint_x, clickpoint_y, 0, 0);//松开
        SetCursorPos(clickpoint_x,clickpoint_y);
    });

    //停止时长定时
    connect(time_to_timer,&QTimer::timeout,[=](){
        time_to_timer->stop();
        emit this->stop_click();
    });

    //停止按钮默认关闭
    ui->Button_Stop->setEnabled(false);

    //输入框默认数值
    ui->lineEdit_time->setText(QString("100"));
    //输入框过滤掉非数字字符
    QRegExp regx("[0-9]+$");
    QValidator* validator = new QRegExpValidator(regx, ui->lineEdit_time);
    ui->lineEdit_time->setValidator(validator);

    //下拉框单位值
    ui->comboBox_unit->addItem("ms");
    ui->comboBox_unit->addItem("s");
    ui->comboBox_unit->addItem("min");

    //连接开始按键槽函数
    connect(ui->Button_Start,&QPushButton::clicked,[=](){
       emit start_click();
    });

    //连接停止按键槽函数
    connect(ui->Button_Stop,&QPushButton::clicked,[=](){
       emit stop_click();
    });

    //停止信号
    connect(this, &MainWindow::stop_click,this,[=](){
        ui->Button_Start->setEnabled(true);
        ui->Button_Stop->setEnabled(false);
        time_to_click->stop();
        time_to_timer->stop();
        qDebug()<<"this->stop_click()";
    });
    //开始信号
    connect(this, &MainWindow::start_click,this,[=](){
        ui->Button_Start->setEnabled(false);
        ui->Button_Stop->setEnabled(true);

        //获取坐标
        clickpoint_x = aim->x()+aim->width()/2-5;
        clickpoint_y = aim->y()+aim->height()/2-5;

        //窗口最小化
        this->minimumSize();
        //获取间隔时间
        interval_time = ( ui->lineEdit_time->text()).toInt(NULL,10);
        switch(ui->comboBox_unit->currentIndex())
        {
            case 0: break;
            case 1: interval_time*=1000; break;
            case 2: interval_time*=1000*60; break;
        }
        time_to_click->start(interval_time);

        //获取点击模式
        if(ui->bt_timer->isChecked())
        {
            //定时模式
            qDebug("timer");
            Timer_num = ui->spinBox_timer->value();
            time_to_timer->start(Timer_num*1000*60);
        }
        else if(ui->bt_counter->isChecked())
        {
            //计次模式
            qDebug("counter");
            Counter_num = ui->spinBox_counter->value();
            time_to_timer->start(Counter_num*interval_time);
        }
        else
        {
            //无限模式
            qDebug("infinte time");
        }
    });

    connect(ui->spinBox_timer,&QSpinBox::setValue,[=]{
        ui->bt_timer->setChecked(true);
    });
    connect(ui->spinBox_counter,&QSpinBox::setValue,[=]{
        ui->bt_counter->setChecked(true);
    });


}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_F9)
    {
        emit stop_click();
    }
    else if(e->key()==Qt::Key_F10)
    {
        emit start_click();
    }
}

void MainWindow::time_out()
{
    QDateTime t_time = QDateTime::currentDateTime();
    QString t_str = t_time.toString("hh:mm:ss");
    label_time->setText(t_str);
}

Img::Img()
{
    setWindowFlags(Qt::FramelessWindowHint);        //设置无边框
    setAttribute(Qt::WA_TranslucentBackground);//设置背景透明
    setWindowFlags(Qt::CoverWindow|Qt::FramelessWindowHint);
    setFixedSize(1920,1080);
    setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    QPixmap img;
    img.load(":/img/aim.png");

    aim = new QLabel(this);
    aim->setFixedSize(img.width(),img.height());
    aim->move(this->width()/2, this->height()/2);
    aim->setFocusPolicy(Qt::TabFocus);
    aim->setCursor(Qt::ClosedHandCursor);
    aim->setPixmap(img);

}

Img::~Img()
{

}

void Img::mouseMoveEvent(QMouseEvent *event)
{
    if((event->x()>=aim->x() && event->x()<=(aim->x()+width()))&&
         (event->y()>=aim->y() && event->y()<=(aim->y()+height())))
    {
        aim->move(event->x()-aim->width()/2,event->y()-aim->height()/2);
        clickpoint_x = aim->x()+aim->width()/2-5;
        clickpoint_y = aim->y()+aim->height()/2-5;
    }
}
