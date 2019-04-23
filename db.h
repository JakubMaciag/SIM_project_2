#ifndef DB_H
#define DB_H
#include <iostream>
#include <stdio.h>
#include <sql.h>
#include <sqlext.h>
#include <QString>
#include <QStringList>
#include "dcmtklib.h"

class Db
{
public:
    Db();
    //połączenie się z bazą

    QString connectAndExecuteSelect(QString select);
    //rozłączenie się z bazą - opcjonalnie
    void disconnect(SQLHDBC dbc);
    //pobranie listy wszystkich pacjentów wraz z ich danymi

    QString getListOfPatients();
    //pobranie listy badań jednego pacjenta separatory analogicznie do @ i #
    QString getListOfStudies(QString idPatient);
    //pobranie listy serii konkretnego pacjenta dla konkretnego badania  separatory analogicznie do @ i #
    QString getListOfSeries(QString idPatient, QString idStudy);
    //pobranie listy obrazow dla konkretnego pacjenta konkretnego badania i konkretnej serii separatory analogicznie do @ i #
    QString getListOfPictures(QString idPatient, QString idStudy, QString idSeries);

    //dodanie pacjenta
    void setNewPatient(QString patientData);
    //dodanie nowego badania
    void setNewStudy(QString patientID, QString studyData);
    //dodanie nowej serii
    void setNewSeries(QString studyID,QString patinetID, QString seriesData);
    //dodanie nowego obrazu
    void setNewPicture(QString patientID, QString studyID, QString seriesID, QString pictureData);

    //usuniecie pacjenta
    void delNewPatient(QString patientID);
    //usuniecie badania
    void delNewStudy(QString patientID, QString studyID);
    //usuniecie serii
    void delNewSeries(QString patinetID, QString studyID, QString seriesID);
    //usuniecie obrazu
    void delNewPicture(QString patientID, QString studyID, QString seriesID, QString pictureID);

    //usuniecie badania
    void delNewStudy2(QString patientID);
    //usuniecie serii
    void delNewSeries2(QString patinetID);
    //usuniecie obrazow
    void delNewPicture2(QString patientID);

    void delNewPicture4(QString patientID, QString studyID, QString seriesID);
    //usuniecie serii
    void delNewSeries3(QString patinetID, QString studyID);
    //usuniecie obrazu
    void delNewPicture3(QString patientID, QString studyID);


    //weryfikacja
       bool ifPatientExist(QString patientID);
       bool ifStudyExist(QString patientID, QString studyID);
       bool ifSeriesExist(QString patinetID, QString studyID, QString seriesID);
       bool ifPictureExist(QString patientID, QString studyID, QString seriesID, QString pictureID);
};

#endif // DB_H
