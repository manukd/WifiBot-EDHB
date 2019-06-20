#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include <QWebEngineView>
#include <QKeyEvent>

#include "MyRobot.h"
#include "robotcontroller.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();
    static void writeConsole(char* text);
    void camControl(int i);

private slots:
    void startConnection();
    void handleSlider();
    void setBatteryBar(const QByteArray Data);
    void go(int way);

    void on_front_pressed();
    void on_frontRight_pressed();
    void on_frontLeft_pressed();
    void on_left_pressed();
    void on_right_pressed();
    void on_back_pressed();

    void on_front_released();
    void on_right_released();
    void on_frontLeft_released();
    void on_frontRight_released();
    void on_left_released();
    void on_back_released();

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void on_cam_up_clicked();

    void on_cam_down_clicked();

    //void on_cam_left_clicked();

    void on_cam_right_clicked();

    void on_cam_left_pressed();

private:
    Ui::MainWindow *ui;

    /// --- Working variables ---
    MyRobot *wifiBot;
    RobotController *rbController;
    QNetworkAccessManager *manager;
};

#endif // MAINWINDOW_H
