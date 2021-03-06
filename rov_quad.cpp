#include "rov_quad.h"
#include <QtSql>
#include <QMessageBox>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QPushButton>
#include <QTime>
#include <QDateTime>
#include <iostream>
using namespace std;

rov_quad::rov_quad(QWidget *parent)
    : QMainWindow(parent)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");  //CHOIX DU DRIVER UTILISÉ POUR LA CONNECTION A LA BASE DE DONNÉES
    db.setHostName("localhost");    //IP OU EST LA BASE DE DONNÉES
    db.setDatabaseName("rov_quad");     //NOM DE LA BASE DE DONNÉES
    db.setUserName("root");     //NOM D'UTILISATEUR A UTILISER POUR LA CONNECTION A LA BASE DE DONNÉES
    db.setPassword("");     //MOT DE PASSE A UTILISER POUR LA CONNECTION A LA BASE DE DONNÉES
    db.open();      //OUVERTURE LA BASE DE DONNÉES


    if (db.open()) {
        qDebug() << "Connection à la bdd ok" << endl;
    }

    else {
        //DEBUG
        QMessageBox msgBox;
        msgBox.setText("Erreur : Problème de Base de données");
        msgBox.setWindowTitle("Erreur");
        msgBox.setDetailedText("L'application n'a pas réussi à accéder à la base de données.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowIcon(QIcon(":/icon.png"));
        msgBox.exec();
    }

    //TAILLE DE LA FENÊTRE
    setMinimumSize(1000,580);
    setMaximumSize(1000,580);

    //CHANGEMENT GRAPHIQUE DE LA FENËTRE
    setStyleSheet("QMainWindow { background-color: #ffffff }");
    setWindowTitle("Rover Quadrupède");
    setStyle(QStyleFactory::create("Fusion"));
    setWindowIcon(QIcon(":/icon.png"));

    //CRÉATION ÉCRAN CAMERA
    QCamera *camera=new QCamera;
    QCameraViewfinder *viewfinder=new QCameraViewfinder(this);
    viewfinder->resize(716,480);
    viewfinder->move(300,30);

    camera->setViewfinder(viewfinder);

    imageCapture=new QCameraImageCapture(camera);

    camera->setCaptureMode(QCamera::CaptureStillImage);
    camera->start();

    //BOUTON POUR PRENDRE UNE PHOTO
    bouton_capture=new QPushButton("Capture",this);
    bouton_capture->move(830,535);
    bouton_capture->resize(120,30);
    bouton_capture->setEnabled(false);
    bouton_capture->setStyleSheet("fusion");

    //BOUTON POUR CHOISIR LE LIEU DE DESTINATION
    bouton_sauv=new QPushButton ("Lieu de destination",this);
    bouton_sauv->move(670,535);
    bouton_sauv->resize(120,30);

    //HUMIDITÉ
    HUM=new QLineEdit(this);
    HUM->resize(50,20);
    HUM->move(557,540);

    humidite1=new QLabel(this);
    humidite1->setText("Humidité :");
    humidite1->resize(150,20);
    humidite1->move(500,540);

    humidite2=new QLabel(this);
    humidite2->setText("%");
    humidite2->resize(20,20);
    humidite2->move(610,540);

    //TEMPERATURE
    TEMP=new QLineEdit(this);
    TEMP->resize(50,20);
    TEMP->move(395,540);

    temperature1=new QLabel(this);
    temperature1->setText("Température :");
    temperature1->resize(200,20);
    temperature1->move(320,540);

    temperature2=new QLabel(this);
    temperature2->setText("°C");
    temperature2->resize(20,20);
    temperature2->move(450,540);

    //PIXMAP
    rec = new QLabel(this);
    rec->move(20,20);
    rec->setFixedSize(1,1);
    pixini = new QPixmap(320,180);
    pixini->fill();
    rec->setPixmap(*pixini);

    rec2 = new QLabel(this);
    rec2->move(20,20);
    rec2->setFixedSize(240,160);
    pixini2 = new QPixmap(320,180);
    pixini2->fill();
    rec2->setPixmap(*pixini2);

    rec3 = new QLabel(this);
    rec3->move(20,200);
    rec3->setFixedSize(240,160);
    pixini3 = new QPixmap(320,180);
    pixini3->fill();
    rec3->setPixmap(*pixini3);

    rec4 = new QLabel(this);
    rec4->move(20,380);
    rec4->setFixedSize(240,160);
    pixini4 = new QPixmap(320,180);
    pixini4->fill();
    rec4->setPixmap(*pixini4);

    //SIGNAUX ET SLOTS
    QObject::connect(bouton_capture, SIGNAL(clicked()), this, SLOT(capture()));
    QObject::connect(bouton_sauv, SIGNAL(clicked()), this, SLOT(lieu_enregistrement()));

}
rov_quad::~rov_quad(){

}

int rov_quad::randInt(int min, int max)
{
//GÉNÈRE UN CHIFFRE ALÉATOIRE ENTRE MIN ET MAX
return qrand() % ((max + 1) - min) + min;
}

void rov_quad::capture()   //CAPTURE ET ENREGISTRE L'IMAGE DANS LE LIEU SELECTIONNÉ
{
    if (imageCapture->isReadyForCapture() == true) {

        i = i + 1;
        //QString s = QString::number(i);              DAY/MONTH/YEAR/HOURS/MINUTES/SECONDS
        QString s = QDateTime::currentDateTime().toString("dd_MM_yyyy_h'H'_mm'm'_ss's'");

       imageCapture->capture(emplacement->absoluteFilePath(QString(s)));

       delai();

       if (photob.load(emplacement->absoluteFilePath(QString(s))) == true && i >= 4 )
       {
           photob4 = photob3.scaled(QSize(320,180),Qt::KeepAspectRatio);
           rec4->setPixmap(photob4);
       }

       if (photob.load(emplacement->absoluteFilePath(QString(s))) == true && i >= 3 )
       {
           photob3 = photob2.scaled(QSize(320,180),Qt::KeepAspectRatio);
           rec3->setPixmap(photob3);
       }

       if (photob.load(emplacement->absoluteFilePath(QString(s))) == true && i >= 2)
       {
           photob2 = photob.scaled(QSize(320,180),Qt::KeepAspectRatio);
           rec2->setPixmap(photob2);
       }

       if (photob.load(emplacement->absoluteFilePath(QString(s))) == true)
       {
           photob.scaled(QSize(320,180),Qt::KeepAspectRatio);
           rec->setPixmap(photob);
       }



       //GÉNÉRATEUR DE CHIFFRE ALÉATOIRE
       QTime time1 = QTime::currentTime();
       qsrand((uint)time1.msec());

       // VALEUR ALÉATOIRE ENTRE 0 ET 50
       double tempAleatoire = randInt(0,500);
       tempAleatoire = tempAleatoire /10;

       //cout << tempAleatoire << endl;

       delai();

       //GÉNÉRATEUR DE CHIFFRE ALÉATOIRE
       QTime time2 = QTime::currentTime();
       qsrand((uint)time2.msec());

       // VALEUR ALÉATOIRE ENTRE 0 ET 100
       double humAleatoire = randInt(0,1000);
       humAleatoire = humAleatoire /10;

       //cout << humAleatoire << endl;

       //ENVOIE DU TEMPS ET DES CHIFFRES ALÉATOIRE A LA BDD
       QSqlQuery query;
       query.prepare("INSERT INTO mesures (date,temperature,humidite,chemin_image) "
                     "VALUES (CURRENT_TIMESTAMP,:temp,:humidite,:chemin_image)");
       query.bindValue(":temp", tempAleatoire);
       query.bindValue(":humidite", humAleatoire);
       query.bindValue(":chemin_image", chemin_image_absolu);
       query.exec();

        QString tempAl = QString::number(tempAleatoire);
        QString humAl = QString::number(humAleatoire);
        TEMP->setText(tempAl);
        HUM->setText(humAl);
    }

    else {

        QMessageBox msgBox;
        msgBox.setText("Erreur : Probleme de Caméra");
        msgBox.setWindowTitle("Erreur");
        msgBox.setDetailedText("L'application n'a pas réussi à accéder à la caméra.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowIcon(QIcon(":/icon.png"));
        msgBox.exec();
    }
}

void rov_quad::lieu_enregistrement()   //ENREGISTRE LE LIEU DANS LA VARIABLE "emplacement"
{
    emplacement = new QDir(choixemplacement.getExistingDirectory(this));
    bouton_capture->setEnabled(true);

    chemin_image_absolu = emplacement->absoluteFilePath(QString(""));
}

void rov_quad::delai()
{
    QTime dieTime= QTime::currentTime().addMSecs(500);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
