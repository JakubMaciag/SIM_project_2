#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;

#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    //konstruktor
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //czyszczenie comboboxow - paski do wyboru danych z bazy danych
    ui->comboBox->clear();
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
    ui->comboBox_4->clear();
    // poczatkowa dezaktywacja przyciskow do usuwania
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
    ui->pushButton_7->setEnabled(false);
    ui->pushButton_8->setEnabled(false);
    // poczatkowa dezaktywacja pol do edycji danych
    ui->lineEdit->setEnabled(false);
    ui->lineEdit_2->setEnabled(false);
    ui->lineEdit_3->setEnabled(false);
    ui->lineEdit_4->setEnabled(false);
    ui->lineEdit_5->setEnabled(false);
    ui->lineEdit_6->setEnabled(false);
    ui->lineEdit_7->setEnabled(false);
    ui->lineEdit_8->setEnabled(false);
    ui->lineEdit_9->setEnabled(false);
    ui->lineEdit_10->setEnabled(false);
    ui->lineEdit_11->setEnabled(false);
    ui->lineEdit_12->setEnabled(false);
    ui->lineEdit_13->setEnabled(false);
    ui->lineEdit_14->setEnabled(false);
    ui->lineEdit_15->setEnabled(false);
    ui->lineEdit_16->setEnabled(false);
    ui->lineEdit_17->setEnabled(false);
    ui->lineEdit_18->setEnabled(false);
    ui->lineEdit_19->setEnabled(false);
    ui->lineEdit_20->setEnabled(false);
}
//destruktor
MainWindow::~MainWindow()
{
    delete ui;
}

// przycisk open
void MainWindow::on_pushButton_clicked()
{
    //pobranie ścieżki do pliku dicom
    filePath = QFileDialog::getOpenFileName(this, "Open DICOM", "/home/", "",0,0);
    ui->nameOfFileLabel->setText(filePath);
    //załadowanie pliku do dataset i image - pobranie podstawowych danych do tworzenia ścieżki
    //weryfikacja poprawnosci pliku dicom
    string filePathStr = filePath.toStdString();
    DcmFileFormat file_format;
    OFCondition status = file_format.loadFile(filePathStr.c_str());
    if (status.bad()) {
        loadingWindow.showMessage("Error with opening the DICOM file!");
    }else{
        dataSet = loadingWindow.lodingFileFromPath(filePath);// wczytanie danych
        image = loadingWindow.loadImageFromDCM(filePath);//wczytanie obrazu
        ui->nameOfFileLabel->setText(filePath);//wyswietlenie sciezki
        //wyświetlenie zdjęcia
        QPixmap scaledImage = image.scaled(ui->label->size());//maly obraz
        ui->label->setPixmap(scaledImage);
        scene = new QGraphicsScene(this);//duzy obraz
        scene->addPixmap(image);
        ui->graphicsView->setScene(scene);
        //konwersja do xml
        loadingWindow.convert(filePath);
        //wyświetlenie danych
        QString filePath2 = "tmpXml.xml";//plik tymczaoswy
        QByteArray data;
        data.reserve(5000000);//bufor danych z pliku - rezerwacja pamieci
        QFile readFile(filePath2);
        if(!readFile.open(QFile::ReadOnly | QFile::Text))
        {
            loadingWindow.showMessage("Error with loading data!");
        }
        else
        {
            //wczytanie pliku xml
            data = readFile.readAll();
            data = loadingWindow.replaceDataText(data);//usuwanie elementow xml
            ui->textBrowser->setText(data);//wyswietlenie
        }
        //usuwanie pliku tymczasowego
        readFile.close();
        loadingWindow.deleteConvertedFiles();
    }
    //utworzenie sciezki docelowej (do bazy)
    pathSvr = loadingWindow.getPathForServer(filePath);
    ypathpic = pathSvr;
    //pobranie danych z dicomu
    xallDataforPatient =  loadingWindow.getData(filePath, pathSvr);
    //przypisanie danych do zmiennych w programie i wyswietlenie w polach do edycji
    getDatafromLoadedDCMintovariabbles();
    fillEditData();
}

//pobranie danych do zapisu
void MainWindow::getDataFromEditLines()
{   //sumowanie danych jako paczki do wrzucenia do bazy danych
    //pacjent
    ypatID = ui->lineEdit->text();
    ysurname = ui->lineEdit_2->text();
    ysex = ui->lineEdit_3->text();
    ydate = ui->lineEdit_4->text();
    yage = ui->lineEdit_5->text( );
    patData = ypatID+"#"+ysurname+"#"+ysex+"#"+ydate+"#"+yage;
    //badanie
    ystudy_id = ui->lineEdit_7->text();
    ytag = ui->lineEdit_8->text( );
    ystudyDate = ui->lineEdit_9->text( );
    ystudyTime = ui->lineEdit_10->text();
    ymodality = ui->lineEdit_11->text( );
    ydescrStudy = ui->lineEdit_12->text();
    ynrtag = ui->lineEdit_13->text( );
    studData = ystudy_id+"#"+ytag+"#"+"0"+"#"+ystudyDate+"#"+ystudyTime+"#"+ymodality+"#"+ydescrStudy+"#"+ynrtag;
    //seria
    yseriesID = ui->lineEdit_14->text();
    yserDate = ui->lineEdit_15->text();
    yseTime = ui->lineEdit_6->text();
    ynumberOfSlices = ui->lineEdit_16->text();
    yserDesc = ui->lineEdit_17->text();
    serData = yseriesID+"#"+yserDate+"#"+yseTime+"#"+ynumberOfSlices+"#"+yserDesc;
    //obrazy
    ypicId = ui->lineEdit_18->text();
    ypicNumber = ui->lineEdit_19->text();
    pathSvr = ui->lineEdit_20->text();
    picData = ypicId+"#"+ypicNumber+"#"+ypathpic;
}

//save in db
void MainWindow::on_pushButton_2_clicked()
{   //sprawdzenie wypelnienia pol tekstowych danymi
   // ui->lineEdit_5->setText(ui->lineEdit_5->text().replace("Y",""));
    bool logicVariable = true;
    if (ui->lineEdit->text().isEmpty())
    {
        loadingWindow.showMessage("No patient id");
        logicVariable = false;
    }
    if(ui->lineEdit_7->text().isEmpty())
    {
        loadingWindow.showMessage("No study id");
        logicVariable = false;
    }
    if(ui->lineEdit_14->text().isEmpty())
    {
        loadingWindow.showMessage("No series id");
        logicVariable = false;
    }
    if(ui->lineEdit_20->text().isEmpty())
    {
        loadingWindow.showMessage("No picture id");
        logicVariable = false;
    }
    cout<<filePath.toStdString()<<endl;

    if (logicVariable){
        //wczytanie danych z pol edytowalnych
       getDataFromEditLines();
        //weryfikacja istnienia danych w bazie
        if (dbset->ifPatientExist(ypatID))
        {
            sendWindow.showMessage("Patient exists in DB");
            if (dbset->ifStudyExist(ypatID, ystudy_id))
            {
                sendWindow.showMessage("Study exists in DB");
                if(dbset->ifSeriesExist(ypatID, ystudy_id, yseriesID)){
                    sendWindow.showMessage("Series exists in DB");
                    if(dbset->ifPictureExist(ypatID, ystudy_id, yseriesID, ypatID))
                    {
                        sendWindow.showMessage("Picture exists in DB");
                    }
                    else{
                        //wrzucenie danych do bazy - obraz
                        dbset->setNewPicture(ypatID,ystudy_id,yseriesID,picData);
                        //stworzenie sciezki
                        QString directory = "/home/";
                        QString fianlPath = directory + ui->lineEdit_20->text();
                        //zapisanie pliku
                        sendWindow.sendIntoFile(filePath, fianlPath);
                        sendWindow.showMessage("Done");
                    }
                }else {
                    //wrzucenie danych do bazy - seria i obraz
                    dbset->setNewSeries(ypatID,ystudy_id,serData);
                    dbset->setNewPicture(ypatID,ystudy_id,yseriesID,picData);
                    //stworzenie sciezki
                    QString directory1 = "/home/";
                    QString fianlPath1 = directory1 + ui->lineEdit_20->text();
                    //zapisanie pliku
                    sendWindow.sendIntoFile(filePath, fianlPath1);
                    sendWindow.showMessage("Done");
                }
            }else{
                //wrzucenie danych do bazy - badanie seria i obraz
                dbset->setNewStudy(ypatID,studData);
                dbset->setNewSeries(ypatID,ystudy_id,serData);
                dbset->setNewPicture(ypatID,ystudy_id,yseriesID,picData);
                //stworzenie sciezki
                QString directory2 = "/home/";
                QString fianlPath2 = directory2 + ui->lineEdit_20->text();
                cout<<fianlPath2.toStdString()<<endl;
                //zapisanie pliku
                sendWindow.sendIntoFile(filePath, fianlPath2);
                sendWindow.showMessage("Done");
            }
        }else{
            //wrzucenie danych do bazy - pacjent badanie seria obraz
            dbset->setNewPatient(patData);
            dbset->setNewStudy(ypatID,studData);
            dbset->setNewSeries(ypatID,ystudy_id,serData);
            dbset->setNewPicture(ypatID,ystudy_id,yseriesID,picData);
            //stworzenie sciezki
            QString directory3 = "/home/";
            QString fianlPath3= directory3 + ui->lineEdit_20->text();
            //zapisanie pliku
            sendWindow.sendIntoFile(filePath, fianlPath3);
            sendWindow.showMessage("Done");
        }
    }else
    {
        sendWindow.showMessage("Error with saving data. Bad data or data is in DB!");
    }
}


//przycisk load from DB
void MainWindow::on_pushButton_3_clicked()
{
    //wyczyszczenie comboboxow
    ui->comboBox->clear();
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
    ui->comboBox_4->clear();
    //pobranie danych - pacjenci
    QString listP = dbset->getListOfPatients();
    QStringList listOfPatients = listP.split("@");
    ui->comboBox->addItems(listOfPatients);
}


//aktywowanie i dezaktywowanie przyciskow do usuwania
void MainWindow::on_checkBox_2_clicked()
{
    if(ui->checkBox_2->isChecked())
    {
        ui->pushButton_5->setEnabled(true);
        ui->pushButton_6->setEnabled(true);
        ui->pushButton_7->setEnabled(true);
        ui->pushButton_8->setEnabled(true);

    }else
    {
        ui->pushButton_5->setEnabled(false);
        ui->pushButton_6->setEnabled(false);
        ui->pushButton_7->setEnabled(false);
        ui->pushButton_8->setEnabled(false);
    }
}

//aktywowanie/dezaktywowanie pol do edycji danych wczytywanych do bazy
void MainWindow::on_checkBox_clicked()
{
    ui->lineEdit->setEnabled(ui->checkBox->isChecked());
    ui->lineEdit_2->setEnabled(ui->checkBox->isChecked());
    ui->lineEdit_3->setEnabled(ui->checkBox->isChecked());
    ui->lineEdit_4->setEnabled(ui->checkBox->isChecked());
    ui->lineEdit_5->setEnabled(ui->checkBox->isChecked());
    ui->lineEdit_6->setEnabled(ui->checkBox->isChecked());
    ui->lineEdit_7->setEnabled(ui->checkBox->isChecked());
    ui->lineEdit_8->setEnabled(ui->checkBox->isChecked());
    ui->lineEdit_9->setEnabled(ui->checkBox->isChecked());
    ui->lineEdit_10->setEnabled(ui->checkBox->isChecked());
    ui->lineEdit_11->setEnabled(ui->checkBox->isChecked());
    ui->lineEdit_12->setEnabled(ui->checkBox->isChecked());
    ui->lineEdit_13->setEnabled(ui->checkBox->isChecked());
    ui->lineEdit_14->setEnabled(ui->checkBox->isChecked());
    ui->lineEdit_15->setEnabled(ui->checkBox->isChecked());
    ui->lineEdit_16->setEnabled(ui->checkBox->isChecked());
    ui->lineEdit_17->setEnabled(ui->checkBox->isChecked());
    ui->lineEdit_18->setEnabled(ui->checkBox->isChecked());
    ui->lineEdit_19->setEnabled(ui->checkBox->isChecked());
    ui->lineEdit_20->setEnabled(ui->checkBox->isChecked());
}

//usuwanie pacjenta
void MainWindow::on_pushButton_5_clicked()
{
    xpatID = ui->comboBox->currentText().split("#").first();
    pathPat = "/home/"+xpatID;
    sendWindow.deleteFolder(pathPat);
    dbset->delNewPicture2(xpatID);
    dbset->delNewSeries2(xpatID);
    dbset->delNewStudy2(xpatID);
    dbset->delNewPatient(xpatID);
    ui->comboBox->clear();
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
    ui->comboBox_4->clear();
}
//usuwanie badania
void MainWindow::on_pushButton_6_clicked()
{
    xpatID = ui->comboBox->currentText().split("#").first();
    pathStud = "/home/"+xpatID+"/"+ui->comboBox_2->currentText().split("#").first();
    sendWindow.deleteFolder(pathStud);
    xstudID = ui->comboBox_2->currentText().split("#").first();
    dbset->delNewPicture3(xpatID,xstudID);
    dbset->delNewSeries3(xpatID,xstudID);
    dbset->delNewStudy(xpatID,xstudID);
    ui->comboBox->clear();
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
    ui->comboBox_4->clear();
}
//usuwanie serii
void MainWindow::on_pushButton_7_clicked()
{   xpatID = ui->comboBox->currentText().split("#").first();
    xstudID = ui->comboBox_2->currentText().split("#").first();
    xserID = ui->comboBox_3->currentText().split("#").first();
    pathSer = "/home/"+xpatID+"/"+xstudID+"/"+xserID;
    sendWindow.deleteFolder(pathSer);
    dbset->delNewPicture4(xpatID,xstudID,xserID);
    dbset->delNewSeries(xpatID,xstudID,xserID);
    ui->comboBox->clear();
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
    ui->comboBox_4->clear();
}
//usuwanie obrazu
void MainWindow::on_pushButton_8_clicked()
{
    sendWindow.deleteFileInPath(pathPict);
    xpatID = ui->comboBox->currentText().split("#").first();
    xstudID = ui->comboBox_2->currentText().split("#").first();
    xserID = ui->comboBox_3->currentText().split("#").first();
    xpicID = ui->comboBox_4->currentText().split("#").first();
    dbset->delNewPicture(xpatID,xstudID,xserID,xpicID);
    ui->comboBox->clear();
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
    ui->comboBox_4->clear();
}
//zapis pliku do nosnika
void MainWindow::on_pushButton_4_clicked()
{
    QString filePathToSave;
    filePathToSave = QFileDialog::getSaveFileName(this,"Save DICOM","","*.dcm",0,0);
    QString path2Tmp = "/home/"+ui->comboBox_4->currentText().split("#").at(3);
    sendWindow.saveOnAnyDisk(path2Tmp, filePathToSave);
}
//wypelnienie pol danych
void MainWindow::fillEditData(){
    //pacjent
    ui->lineEdit->setText(ypatID);
    ui->lineEdit_2->setText(ysurname);
    ui->lineEdit_3->setText(ysex);
    ui->lineEdit_4->setText(ydate);
    ui->lineEdit_5->setText( yage);
    //badanie
    ui->lineEdit_7->setText(ystudy_id);
    ui->lineEdit_8->setText( ytag);
    ui->lineEdit_9->setText( ystudyDate);
    ui->lineEdit_10->setText(ystudyTime);
    ui->lineEdit_11->setText( ymodality);
    ui->lineEdit_12->setText(ydescrStudy);
    ui->lineEdit_13->setText( ynrtag);
    //seria
    ui->lineEdit_14->setText(yseriesID);
    ui->lineEdit_15->setText(yserDate);
    ui->lineEdit_6->setText(yseTime);
    ui->lineEdit_16->setText(ynumberOfSlices);
    ui->lineEdit_17->setText(yserDesc);
    //obrazy
    ui->lineEdit_18->setText(ypicId);
    ui->lineEdit_19->setText(ypicNumber);
    ui->lineEdit_20->setText( pathSvr);
    //pobranie numeru obrazu

}

//pobranie danych z nowo otwartego pliku
void MainWindow::getDatafromLoadedDCMintovariabbles()
{   allDataFromFileInList = xallDataforPatient.split("#");
    ypatID = allDataFromFileInList.at(0);
    ysurname = allDataFromFileInList.at(1);
    ysex = allDataFromFileInList.at(2);
    ydate = allDataFromFileInList.at(3);
    yage = allDataFromFileInList.at(4);
    ystudy_id = allDataFromFileInList.at(5);
    ytag = allDataFromFileInList.at(6);
    ynrtag = allDataFromFileInList.at(7);
    ystudyDate = allDataFromFileInList.at(8);
    ystudyTime = allDataFromFileInList.at(9);
    ymodality = allDataFromFileInList.at(10);
    ydescrStudy = allDataFromFileInList.at(11);
    yanyStudId = allDataFromFileInList.at(12);
    yseriesID = allDataFromFileInList.at(13);
    yserDate = allDataFromFileInList.at(14);
    yseTime = allDataFromFileInList.at(15);
    yserDesc = allDataFromFileInList.at(16);
    ynumberOfSlices = allDataFromFileInList.at(17);
    ypicId = allDataFromFileInList.at(18);
    ypicNumber = allDataFromFileInList.at(19);
}


//load studies from db
void MainWindow::on_pushButton_9_clicked()
{
    //wyczyszczenie comboboxow
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
    ui->comboBox_4->clear();
    //pobranie danych - badania
    QString stringData = ui->comboBox->currentText();
    stringData.replace("@","");
    QStringList strList = stringData.split("#");
    QString idPatient = strList.first();
    QString listP = dbset->getListOfStudies(idPatient);
    QStringList studies = listP.split("@");
    ui->comboBox_2->addItems(studies);
}
//load series from db
void MainWindow::on_pushButton_10_clicked()
{
    //wyczyszczenie comboboxow
        ui->comboBox_3->clear();
        ui->comboBox_4->clear();
        //pobranie danych - serie
        QString stringData = ui->comboBox->currentText();
        stringData.replace("@","");
        QStringList strList = stringData.split("#");
        QString idPatient = strList.first();

        QString stringData2 = ui->comboBox_2->currentText();
        stringData2.replace("@","");
        QStringList strList2 = stringData2.split("#");
        QString idStudy = strList2.first();

        QString listP = dbset->getListOfSeries(idPatient, idStudy);
        QStringList series = listP.split("@");
        ui->comboBox_3->addItems(series);
}
//load pictures from db
void MainWindow::on_pushButton_11_clicked()
{
    //wyczyszczenie comboboxa
    ui->comboBox_4->clear();
    //pobranie danych o obrazach
    QString stringData = ui->comboBox->currentText();
    stringData.replace("@","");
    QStringList strList = stringData.split("#");
    QString idPatient = strList.first();

    QString stringData2 = ui->comboBox_2->currentText();
    stringData2.replace("@","");
    QStringList strList2 = stringData2.split("#");
    QString idStudy = strList2.first();

    QString stringData3 = ui->comboBox_3->currentText();
    stringData3.replace("@","");
    QStringList strList3 = stringData3.split("#");
    QString idSeries = strList3.first();
    cout<<" "+idPatient.toStdString()+" "+idStudy.toStdString()+" "+ idSeries.toStdString()<<endl;

    QString listP = dbset->getListOfPictures(idPatient, idStudy, idSeries);
    cout<<"lista obr "+listP.toStdString()<<endl;
    QStringList pictures = listP.split("@");
    ui->comboBox_4->addItems(pictures);
}
//load picture from db
void MainWindow::on_pushButton_12_clicked()
{
    //pobranie sciezki do pliku z bazy
    QStringList str = ui->comboBox_4->currentText().split("#");
    pathTmp = str.takeAt(3);
    pathTmp = "/home/"+pathTmp;
    //zaladowanie pliku z bazy
    DcmDataset* dataSet2 = loadingWindow.lodingFileFromPath(pathTmp);
    QPixmap image2 = loadingWindow.loadImageFromDCM(pathTmp);
    //wyświetlenie zdjęcia
    QGraphicsScene *scene2 = new QGraphicsScene(this);
    scene2->addPixmap(image2);
    ui->graphicsView->setScene(scene2);
    //konwersja do xml
    loadingWindow.convert2(pathTmp);
    //wyświetlenie danychF
    QString filePath3 = "tmpXml2.xml";
    QByteArray data2;
    data2.reserve(5000000);
    QFile readFile1(filePath3);
    if(!readFile1.open(QFile::ReadOnly | QFile::Text))
    {
        loadingWindow.showMessage("Error with loading data!");
    }
    else
    {
        data2 = readFile1.readAll();
        data2 = loadingWindow.replaceDataText(data2);
        ui->textBrowser_2->setText(data2);
    }
    //usuwanie pliku tymczasowego
    readFile1.close();
    loadingWindow.deleteConvertedFiles2();
}
