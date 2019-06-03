#include "robotcontroller.h"

RobotController::RobotController(MyRobot *robot)
{
    _myRobot = robot;
    _speed = 0;
}

void RobotController::setSpeed(int value){
    _speed = value;
}
bool RobotController::getState()
{
    return this->connected;
}
void RobotController::setState(bool _state)
{
    this->connected = _state;
}
// --- Direction ---
void RobotController::goFront(){
    _myRobot->speed(_speed,_speed,true,true);
}
void RobotController::goBack(){
    _myRobot->speed(_speed,_speed,false,false);
}
void RobotController::turnLeft(bool withFront){
    if (withFront) {
        _myRobot->speed(_speed,_speed-50,true,true);
    }else
        _myRobot->speed(_speed,_speed,true,false);
}
void RobotController::turnRight(bool withFront){
    if(withFront){
        _myRobot->speed(_speed-50,_speed,true,true);
    }else
        _myRobot->speed(_speed,_speed,false,true);
}

void RobotController::stop(){
    _myRobot->speed(0,0,true,true);
}


/// --- Sensor ---

void RobotController::getCamStream(){

}
