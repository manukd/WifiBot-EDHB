#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include <QWebEngineView>

#include "MYROBOT.h"
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

private slots:
    void startConnection();
    void handleSlider();
    void setBatteryBar(int value);
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
private:
    Ui::MainWindow *ui;

    /// --- Working variables ---
    MyRobot *wifiBot;
    RobotController *rbController;
};

#endif // MAINWINDOW_H
