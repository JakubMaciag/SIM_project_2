#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "dcmtklib.h"
#include <QDialog>
#include <QMessageBox>
#include "loadfile.h"
#include "sendfile.h"
#include "db.h"

using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    LoadFile loadingWindow;
    SendFile sendWindow;
    DcmDataset* dataSet;
    QString pathSvr;
    QString filePath;
    QString pathTmp;
    Db* dbset = new Db;
    QString pathPat;
    QString pathStud;
    QString pathSer;
    QString pathPict;

    QString xpatID;
    QString xstudID;
    QString xserID;
    QString xpicID;

    QString patData;
    QString studData;
    QString serData;
    QString picData;
    QString xallDataforPatient;
    QStringList allDataFromFileInList;

    QString ypatID;
    QString ysurname;
    QString ysex;
    QString ydate;
    QString yage;
    QString ystudy_id;
    QString ytag;
    QString ynrtag;
    QString ystudyDate;
    QString ystudyTime;
    QString ymodality;
    QString ydescrStudy;
    QString yanyStudId;
    QString yseriesID;
    QString yserDate;
    QString yseTime;
    QString yserDesc;
    QString ynumberOfSlices;
    QString ypicId;
    QString ypicNumber;
    QString ypathpic;

    QPixmap image;
    QString patient_id, study_id, series_id, file_name;
    QGraphicsScene *scene;
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_checkBox_2_clicked();
    void on_checkBox_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_10_clicked();
    void on_pushButton_11_clicked();
    void on_pushButton_12_clicked();

private:
    Ui::MainWindow *ui;
    void fillEditData();
    void getDatafromLoadedDCMintovariabbles();
    void getDataFromEditLines();
};

#endif // MAINWINDOW_H
