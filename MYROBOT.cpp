// myrobot.cpp

#include "MYROBOT.h"

MyRobot::MyRobot(QObject *parent) : QObject(parent) {
    DataToSend.resize(9);
    DataToSend[0] = 0xFF;
    DataToSend[1] = 0x07;
    DataToSend[2] = 0x0;
    DataToSend[3] = 0x0;
    DataToSend[4] = 0x0;
    DataToSend[5] = 0x0;
    DataToSend[6] = 0x0;
    DataToSend[7] = 0x0;
    DataToSend[8] = 0x0;
    DataReceived.resize(21);
    TimerEnvoi = new QTimer();
    // setup signal and slot
    connect(TimerEnvoi, SIGNAL(timeout()), this, SLOT(MyTimerSlot())); //Send data to wifibot timer
}


void MyRobot::doConnect() {
    socket = new QTcpSocket(this); // socket creation
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    qDebug() << "connecting..."; // this is not blocking call
    //socket->connectToHost("LOCALHOST", 15020);
    socket->connectToHost("192.168.1.11", 15020); // connection to wifibot 192.168.1.106
    // we need to wait...
    if(!socket->waitForConnected(5000)) {
        qDebug() << "Error: " << socket->errorString();
        return;
    }
    TimerEnvoi->start(75);

}

void MyRobot::disConnect() {
    TimerEnvoi->stop();
    socket->close();
}

void MyRobot::connected() {
    qDebug() << "connected..."; // Hey server, tell me about you.
}

void MyRobot::disconnected() {
    qDebug() << "disconnected...";
}

void MyRobot::bytesWritten(qint64 bytes) {
    qDebug() << bytes << " bytes written...";
}

void MyRobot::readyRead() {
    qDebug() << "reading..."; // read the data from the socket
    DataReceived = socket->readAll();
    emit updateUI(DataReceived);
    this->batterie = (float)DataReceived[2];
    qDebug() << (unsigned char)DataReceived[2];
    //this->getStats();
}

void MyRobot::MyTimerSlot() {
    qDebug() << "Timer...";
    while(Mutex.tryLock());
    socket->write(DataToSend);
    Mutex.unlock();
}

int MyRobot::getBat()
{
    return ((this->batterie/10)/18,2)*100;
}



void MyRobot::speed(char speed_right, char speed_left, bool dir_right, bool dir_left)
{
        DataToSend[2] = speed_left;
        DataToSend[3] = 0;
        DataToSend[4] = speed_right;
        DataToSend[5] = 0;
        DataToSend[6] = (0b0000000 | (dir_left << 6) | (dir_right << 4));
        qint16 crc = crc16(DataToSend,7);
        DataToSend[7] = (qint8)(crc);
        DataToSend[8] = (qint8)(crc >> 8);
}

qint16 MyRobot::crc16(QByteArray adresse_tab , unsigned char taille_max)
{
        unsigned int Crc = 0xFFFF;
        unsigned int Polynome = 0xA001;
        unsigned int CptOctet = 0;
        unsigned int CptBit = 0;
        unsigned int Parity= 0;

        Crc = 0xFFFF;
        Polynome = 0xA001; // Polynôme = 2^15 + 2^13 + 2^0 = 0xA001.

        for ( CptOctet= 1 ; CptOctet < taille_max ; CptOctet++)
        {
                Crc ^= (unsigned char)(adresse_tab[CptOctet]); //Ou exculsif entre octet message et CRC

                for ( CptBit = 0; CptBit <= 7 ; CptBit++) /* Mise a 0 du compteur nombre de bits */
                {
                        Parity= Crc;
                        Crc >>= 1; // Décalage a droite du crc
                        if (Parity%2 == 1) Crc ^= Polynome; // Test si nombre impair -> Apres decalage à droite il y aura une retenue
                } // "ou exclusif" entre le CRC et le polynome generateur.
        }
        return(Crc);
}

void MyRobot::getStats()
{
    float sensors[11];
    QByteArray ba = socket->readAll();

       if(ba.size() > 17)
       {
           quint32 odoL,odoR;
           qint16 speedL,speedR;
           quint8 battery,adc0,adc1,adc3,adc4,current,version;

           sensors[0] = ba.at(2);
           qDebug() << sensors[0];
           // Left
           /*speedL = ba.at(0)+(ba.at(1)<<8);
           adc4 = ba.at(3);
           adc3 = ba.at(4);
           odoL = ba.at(5)+(ba.at(6)<<8)+(ba.at(7)<<16)+(ba.at(8)<<24);

           // Right
           speedR = ba.at(9)+(ba.at(10)<<8);
           adc0 = ba.at(11);
           adc1 = ba.at(12);
           odoR = ba.at(13)+(ba.at(14)<<8)+(ba.at(15)<<16)+(ba.at(16)<<24);
           current = ba.at(17);
           version = ba.at(18);*/

           //On suppose que la batterie est déchargée à 11.3V et pleine à 12.8 et et si c'est au dessus, c'est parce qu'il recharge

       }
}
