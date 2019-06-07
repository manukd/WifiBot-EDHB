#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "MYROBOT.h"
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
    setBatteryBar(100);
    ui->progressBar->setValue(0);

    // --- Set interactive component ---
    // Slider
    ui->speedSlider->setMinimum(0);
    ui->speedSlider->setMaximum(100);

    // Connect Button Connection to appropriate slot
    connect(ui->connectionButton,SIGNAL(clicked()),SLOT(startConnection()));
    connect(ui->speedSlider,SIGNAL(valueChanged(int)),SLOT(handleSlider()));
    QShortcut *forward = new QShortcut(QKeySequence("z"), ui->centralWidget);
    QShortcut *back = new QShortcut(QKeySequence("s"), ui->centralWidget);
    QShortcut *left = new QShortcut(QKeySequence("q"), ui->centralWidget);
    QShortcut *right = new QShortcut(QKeySequence("d"), ui->centralWidget);
    QShortcut *forward_left = new QShortcut(QKeySequence("a"), ui->centralWidget);
    QShortcut *forward_right = new QShortcut(QKeySequence("e"), ui->centralWidget);
    QShortcut *forward_f = new QShortcut(QKeySequence("Up"), ui->centralWidget);
    QShortcut *back_f = new QShortcut(QKeySequence("Down"), ui->centralWidget);
    QShortcut *left_f = new QShortcut(QKeySequence("Left"), ui->centralWidget);
    QShortcut *right_f = new QShortcut(QKeySequence("Right"), ui->centralWidget);
    QObject::connect(forward, SIGNAL(activated()), ui->front, SLOT(click()));
    QObject::connect(forward_left, SIGNAL(activated()), ui->frontLeft, SLOT(click()));
    QObject::connect(forward_right, SIGNAL(activated()), ui->frontRight, SLOT(click()));
    QObject::connect(back, SIGNAL(activated()), ui->back, SLOT(click()));
    QObject::connect(left, SIGNAL(activated()), ui->left, SLOT(click()));
    QObject::connect(right, SIGNAL(activated()), ui->right, SLOT(click()));
    QObject::connect(forward_f, SIGNAL(activated()), ui->front, SLOT(click()));
    QObject::connect(back_f, SIGNAL(activated()), ui->back, SLOT(click()));
    QObject::connect(left_f, SIGNAL(activated()), ui->left, SLOT(click()));
    QObject::connect(right_f, SIGNAL(activated()), ui->right, SLOT(click()));

    // Default speed
    //ui->speedSlider->setValue(50);
    // --- Init WifiBotObject ---
    wifiBot = new MyRobot();
    rbController = new RobotController(wifiBot);
    qDebug() << "valeur batterie init : " << wifiBot->getBat();
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
        view->setContentsMargins(10,10,0,0);
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
            setBatteryBar(wifiBot->getBat());
            break;
        case 2:
            rbController->goBack();
            setBatteryBar(wifiBot->getBat());
            break;
        case 3:
            rbController->turnLeft(false);
            setBatteryBar(wifiBot->getBat());
            break;
        case 4:
            rbController->turnRight(false);
            setBatteryBar(wifiBot->getBat());
            break;
        case 5:
            rbController->turnLeft(true);
            setBatteryBar(wifiBot->getBat());
            break;
        case 6:
            rbController->turnRight(true);
            setBatteryBar(wifiBot->getBat());
            break;
        default:
           qDebug() << "Error - Switch - go() - Not suppose to be here... ";
           rbController->stop();
    }

}

void MainWindow::handleSlider(){
    // Set new Speed Value
    rbController->setSpeed((ui->speedSlider->value()*240)/100);

    ui->speedValue->display(ui->speedSlider->value());
    ui->progressBar->setValue(ui->speedSlider->value());
}

void MainWindow::setBatteryBar(int value){
    ui->batteryBar->setValue(value);
    qDebug() << "valeur batterie set bar : " << value;
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
