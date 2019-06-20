#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "MyRobot.h"
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init(){
    //setBatteryBar(100);
    ui->progressBar->setValue(0);

    // --- Set interactive component ---
    // Slider
    ui->speedSlider->setMinimum(0);
    ui->speedSlider->setMaximum(100);
    // Connect Button Connection to appropriate slot
    connect(ui->connectionButton,SIGNAL(clicked()),SLOT(startConnection()));
    connect(ui->speedSlider,SIGNAL(valueChanged(int)),SLOT(handleSlider()));
    /*QShortcut *forward = new QShortcut(QKeySequence("z"), ui->centralWidget);
    QShortcut *back = new QShortcut(QKeySequence("s"), ui->centralWidget);
    QShortcut *left = new QShortcut(QKeySequence("q"), ui->centralWidget);
    QShortcut *right = new QShortcut(QKeySequence("d"), ui->centralWidget);
    QShortcut *forward_left = new QShortcut(QKeySequence("a"), ui->centralWidget);
    QShortcut *forward_right = new QShortcut(QKeySequence("e"), ui->centralWidget);
    QShortcut *forward_f = new QShortcut(QKeySequence("Up"), ui->centralWidget);
    QShortcut *back_f = new QShortcut(QKeySequence("Down"), ui->centralWidget);
    QShortcut *left_f = new QShortcut(QKeySequence("Left"), ui->centralWidget);
    QShortcut *right_f = new QShortcut(QKeySequence("Right"), ui->centralWidget);
    //QObject::connect(forward, SIGNAL(activated()), ui->front, SLOT(click()));
    QObject::connect(forward_left, SIGNAL(activated()), ui->frontLeft, SLOT(click()));
    QObject::connect(forward_right, SIGNAL(activated()), ui->frontRight, SLOT(click()));
    QObject::connect(back, SIGNAL(activated()), ui->back, SLOT(click()));
    QObject::connect(left, SIGNAL(activated()), ui->left, SLOT(click()));
    QObject::connect(right, SIGNAL(activated()), ui->right, SLOT(click()));
    QObject::connect(forward_f, SIGNAL(activated()), ui->front, SLOT(click()));
    QObject::connect(back_f, SIGNAL(activated()), ui->back, SLOT(click()));
    QObject::connect(left_f, SIGNAL(activated()), ui->left, SLOT(click()));
    QObject::connect(right_f, SIGNAL(activated()), ui->right, SLOT(click()));*/

    // Default speed
    //ui->speedSlider->setValue(50);
    // --- Init WifiBotObject ---
    wifiBot = new MyRobot();
    rbController = new RobotController(wifiBot);
    //connect(wifiBot,SIGNAL(updateUI(sensors)),this,SLOT(setBatteryBar(sensors)));
    connect(wifiBot,&MyRobot::updateUI,this,&MainWindow::setBatteryBar);

    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
        //1 - Directions
        case Qt::Key_Z:
            rbController->goFront();
            break;
        case Qt::Key_S:
            rbController->goBack();
            break;
        case Qt::Key_D:
            rbController->turnRight(false);
            break;
        case Qt::Key_Q:
            rbController->turnLeft(false);
            break;
        case Qt::Key_E:
            rbController->turnRight(true);
            break;
        case Qt::Key_A:
            rbController->turnLeft(true);
            break;
        case Qt::Key_Up:
            rbController->goFront();
            break;
        case Qt::Key_Down:
            rbController->goBack();
            break;
        case Qt::Key_Right:
            rbController->turnRight(false);
            break;
        case Qt::Key_Left:
            rbController->turnLeft(false);
        case Qt::Key_8:
            camControl(1);
            break;
        case Qt::Key_2:
            camControl(2);
            break;
        case Qt::Key_4:
            camControl(3);
            break;
        case Qt::Key_6:
            camControl(4);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key()){
        case Qt::Key_Z:
            rbController->stop();
            break;
        case Qt::Key_S:
            rbController->stop();
            break;
        case Qt::Key_D:
            rbController->stop();
            break;
        case Qt::Key_Q:
            rbController->stop();
            break;
        case Qt::Key_E:
            rbController->stop();
            break;
        case Qt::Key_A:
            rbController->stop();
            break;
        case Qt::Key_Up:
            rbController->stop();
            break;
        case Qt::Key_Down:
            rbController->stop();
            break;
        case Qt::Key_Right:
            rbController->stop();
            break;
        case Qt::Key_Left:
            rbController->stop();
    }
}

void MainWindow::startConnection(){
    QWebEngineView *view = new QWebEngineView(this);
    if(!this->rbController->getState())
    {
        ui->console->append("Connexion...");
        ui->connectionButton->setText("Se déconnecter");
        wifiBot->doConnect();
        wifiBot->readyRead();
        ui->console->append("Robot correctement connecté.");
        this->rbController->setState(true);
        view->load(QUrl("http://192.168.1.11:8080/?action=stream"));
        view->resize(ui->cam->size());
        view->move(10,10);
        view->setZoomFactor(1.58);
        view->show();
    }
    else
    {
        ui->console->append("Déconnexion...");
        ui->connectionButton->setText("Connexion");
        wifiBot->disConnect();
        ui->console->append("Robot correctement déconnecté.");
        this->rbController->setState(false);
    }

}


void MainWindow::go(int way){
    switch(way){
        case 1:
            rbController->goFront();

            break;
        case 2:
            rbController->goBack();
            break;
        case 3:
            rbController->turnLeft(false);
            break;
        case 4:
            rbController->turnRight(false);
            break;
        case 5:
            rbController->turnLeft(true);
            break;
        case 6:
            rbController->turnRight(true);
            break;
        default:
           qDebug() << "Error go ";
           rbController->stop();
    }

}

void MainWindow::handleSlider(){
    // Set new Speed Value
    rbController->setSpeed((ui->speedSlider->value()*240)/100);

    ui->speedValue->display(ui->speedSlider->value());
    ui->progressBar->setValue(ui->speedSlider->value());
}

void MainWindow::setBatteryBar(const QByteArray Data){
    qDebug() << "Data[2] : ";
    qDebug() << Data[2];
    quint32 odoL,odoR;
            qint16 speedL,speedR;
            quint8 battery,adc0,adc1,adc3,adc4,current,version;
            battery = Data.at(2);
            // Left
            speedL = Data.at(0)+(Data.at(1)<<8);
            adc4 = Data.at(3);
            adc3 = Data.at(4);
            odoL = Data.at(5)+(Data.at(6)<<8)+(Data.at(7)<<16)+(Data.at(8)<<24);

            // Right
            speedR = Data.at(9)+(Data.at(10)<<8);
            adc0 = Data.at(11);
            adc1 = Data.at(12);
            odoR = Data.at(13)+(Data.at(14)<<8)+(Data.at(15)<<16)+(Data.at(16)<<24);

            current = Data.at(17);
            version = Data.at(18);

            //Valeur final capteurs
            float battery_1 = ((((float)battery)*0.1f)-11.3f/12.6f)*100;
            current = ((float)current)*0.1f;
            float odoL_1 = ((float)odoL)/5564.0f;
            float odoR_1 = ((float)odoR)/5564.0f;

            float speedL1 = ((float)speedL)*100.0f/5564.0f;
            float speedR1 = ((float)speedR)*100.0f/5564.0f;

            float adc4_1 = ((float)adc4)*(3.3f/255);
            float adc3_1 = ((float)adc3)*(3.3f/255);
            float adc0_1 = ((float)adc0)*(3.3f/255);
            float adc1_1 = ((float)adc1)*(3.3f/255);


            ui->batteryBar->setValue(battery_1);
            ui->odo_gauche->display((float)odoL_1);
            ui->odo_droit->display((float)odoR_1);

            ui->vitesse_gauche->display((float)speedL1);
            ui->vitesse_droite->display((float)speedR1);

            ui->adc_avant_d->display((float)adc4_1);
            ui->adc_arriere_d->display((float)adc3_1);
            ui->adc_avant_g->display((float)adc0_1);
            ui->adc_arriere_g->display((float)adc1_1);

            qDebug() << (((float)speedL)*100.0f)/5564.0f;
            qDebug() << ((float)adc4)*(3.3f/255);


}


void MainWindow::camControl(int i)
{
    switch(i){
        case 1:
        manager->get(QNetworkRequest(QUrl("http://192.168.1.11:8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=-250")));
            break;
        case 2:
        manager->get(QNetworkRequest(QUrl("http://192.168.1.11:8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=250")));
            break;
        case 3:
        manager->get(QNetworkRequest(QUrl("http://192.168.1.11:8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=250")));
            break;
        case 4:
        manager->get(QNetworkRequest(QUrl("http://192.168.1.11:8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=-250")));
            break;
        default:
           qDebug() << "Error camControl ";
           rbController->stop();
    }
}

/// --- Direction Button ---
void MainWindow::on_front_pressed(){    go(1);  }
void MainWindow::on_back_pressed(){    go(2);  }
void MainWindow::on_left_pressed(){    go(3);  }
void MainWindow::on_right_pressed(){    go(4);  }
void MainWindow::on_frontLeft_pressed(){    go(5);  }
void MainWindow::on_frontRight_pressed(){    go(6);  }

void MainWindow::on_front_released(){   rbController->stop();  }
void MainWindow::on_left_released(){    rbController->stop();  }
void MainWindow::on_back_released(){    rbController->stop();  }
void MainWindow::on_right_released(){    rbController->stop();  }
void MainWindow::on_frontLeft_released(){  rbController->stop();  }
void MainWindow::on_frontRight_released(){   rbController->stop();  }

void MainWindow::on_cam_up_clicked()
{
    camControl(1);
}

void MainWindow::on_cam_down_clicked()
{
    camControl(2);
}

/*void MainWindow::on_cam_left_clicked()
{
    camControl(3);
}*/

void MainWindow::on_cam_right_clicked()
{
    camControl(4);
}

void MainWindow::on_cam_left_pressed()
{
    camControl(3);
}
 //déchargée à 11.3V et pleine à 12.8
