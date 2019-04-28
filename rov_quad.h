#ifndef rov_quad_H
#define rov_quad_H

#include <QMainWindow>
#include <QtWidgets>
#include <QCameraImageCapture>
#include <QCameraViewfinder>
#include <QCamera>

class rov_quad : public QMainWindow
{
    Q_OBJECT

public:
   rov_quad(QWidget *parent = 0);
    ~rov_quad();

public slots:
        void capture();
        void lieu_enregistrement();
        void delai();
        int randInt(int low, int high);

private :
        int i = 0;
        //TOUT FONCTIONNE
        QPushButton *bouton_capture;
        QPushButton *bouton_sauv;
        QCameraImageCapture *imageCapture;
        QCamera *Camera;
        QCameraViewfinder *mCameraViewfinder;
        QCameraImageCapture *mCameraImageCapture;
        QFileDialog choixemplacement;
        QDir *emplacement;
        QLineEdit *chemin;
        QLineEdit *chemin_image;
        QString chemin_image_absolu;

        //TEMPÉRATURE ET HUMIDITÉ
        QLineEdit *HUM;
        QLineEdit *TEMP;
        QLabel *humidite1;
        QLabel *humidite2;
        QLabel *temperature1;
        QLabel *temperature2;

        //PIXMAP (AFFICHE LES DERNIERE IMAGE)
        QLabel *rec;
        QLabel *rec2;
        QLabel *rec3;
        QLabel *rec4;
        QPixmap photob, *pixini;
        QPixmap photob2, *pixini2;
        QPixmap photob3, *pixini3;
        QPixmap photob4, *pixini4;
        //TEST
};

#endif // rov_quad_H
