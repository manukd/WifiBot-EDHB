#ifndef ROBOTCONTROLLER_H
#define ROBOTCONTROLLER_H

#include "MyRobot.h"

class RobotController
{
public:
    RobotController(MyRobot *myRobot);
    ~RobotController();

    void setSpeed(int value);
    bool getState();
    void setState(bool _state);

    // --- Direction ---
    void goFront();
    void goBack();
    void turnLeft(bool withFront);
    void turnRight(bool withFront);
    void stop();

    // --- Sensor ---
    void getCamStream();


private:
    MyRobot * _myRobot;
    char _speed;
    bool connected = false;

};

#endif // ROBOTCONTROLLER_H
