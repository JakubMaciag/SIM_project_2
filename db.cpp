#include "db.h"

using namespace std;
Db::Db()
{
}
//połączenie się z bazą
QString Db::connectAndExecuteSelect(QString select){
    SQLHENV env;
    char driver[256];
    char attr[256];
    SQLSMALLINT driver_ret;
    SQLSMALLINT attr_ret;
    SQLUSMALLINT direction;
    SQLRETURN ret;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLCHAR outstr[1024];
    SQLSMALLINT outstrlen;
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
    SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);
    char dsn[256];
    char desc[256];
    SQLSMALLINT dsn_ret;
    SQLSMALLINT desc_ret;
    QString data;
    QString wynik;
   //SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
    //SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);

    direction = SQL_FETCH_FIRST;
    while(SQL_SUCCEEDED(ret = SQLDataSources(env, direction,
                                             (SQLCHAR*)dsn, sizeof(dsn), &dsn_ret,
                                             (SQLCHAR*)desc, sizeof(desc), &desc_ret))) {
        direction = SQL_FETCH_NEXT;
        printf("%s - %s\n", dsn, desc);
        if (ret == SQL_SUCCESS_WITH_INFO) printf("\tdata truncation\n");
    }
    SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);
    ret = SQLDriverConnect(dbc, NULL, (SQLCHAR*)"DSN=HyperApp;", SQL_NTS,
                           outstr, sizeof(outstr), &outstrlen,
                           SQL_DRIVER_COMPLETE);
    if (SQL_SUCCEEDED(ret)) {
        printf("Connected\n");

        SQLRETURN rc;

        if ((rc = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt)) != SQL_SUCCESS) {
            cout << "Alloc failed";
        }
        ;
        if ((rc = SQLExecDirect(stmt, (SQLCHAR*) select.toStdString().c_str(), SQL_NTS)) != SQL_SUCCESS) {
            cout << "ExecDirect failed";
        }

        while( SQLFetch(stmt) == SQL_SUCCESS) {
            int i = 1;
            // get the data for each column and add it to
            if(wynik.endsWith("#")) {
                wynik.remove(wynik.length()-1,wynik.length());
            }
            while (true) {
                SQLCHAR buf[255] = {0};
                if ((ret = SQLGetData(stmt, i, SQL_CHAR, buf, sizeof(buf), NULL)) == SQL_SUCCESS) {
                    data = reinterpret_cast<char*>(buf);
                    wynik.append(data);
                    wynik.append("#");
                    cout<<data.toStdString().c_str();
                    i++;
                } else {
                    break;
                }
            }
            wynik.append("@");
        }
        SQLDisconnect(stmt);
        printf("3result:\n");
        SQLRETURN result = SQLExecute(stmt);
        printf("result: " + result);
        if (ret == SQL_SUCCESS_WITH_INFO) {
            printf("Driver reported the following diagnostics\n");

        }
        SQLDisconnect(dbc);               /* disconnect from driver */
    } else {
        fprintf(stderr, "Failed to connect\n");

    }
    /* free up allocated handles */
    SQLFreeHandle(SQL_HANDLE_DBC, dbc);
    SQLFreeHandle(SQL_HANDLE_ENV, env);
    SQLDisconnect(dbc);
    SQLCancel(stmt);
    cout<<wynik.toStdString()<<endl;
    return wynik;
}



//rozłączenie się z bazą - opcjonalnie
void Db::disconnect(SQLHDBC dbc)
{
    SQLDisconnect(dbc);
}

//pobranie listy wszystkich pacjentów wraz z ich danymi
/*
     * dane pierwszego pacjenta @ dane drugiego pacjenta @... separator danych "@"
     *
     * id pacjenta # nazwisko pacjenta # ... separator "#"
     *
     * np. id1#nazwisko1#...@id2#nazwisko2#...@...
     * */
QString Db::getListOfPatients(){
    return connectAndExecuteSelect("select * from pacjenci");

}

//pobranie listy badań jednego pacjenta separatory analogicznie do @ i #
QString Db::getListOfStudies(QString idPatient){
    return connectAndExecuteSelect("select * from badania where pesel_pacjenta='"+idPatient+"'");
}

//pobranie listy serii konkretnego pacjenta dla konkretnego badania  separatory analogicznie do @ i #
QString Db::getListOfSeries(QString idPatient, QString idStudy){
    return connectAndExecuteSelect("select * from seria where id_badania_glowne ='"+idStudy+"' and pesel_pacjenta='"+idPatient+"'");
}

//pobranie listy obrazow dla konkretnego pacjenta konkretnego badania i konkretnej serii separatory analogicznie do @ i #
QString Db::getListOfPictures(QString idPatient, QString idStudy, QString idSeries){
    return connectAndExecuteSelect("select * from obrazy where pesel_pacjenta='"+idPatient+"' and id_badania_glowne='"+idStudy+"' and id_serii='"+idSeries+"'");

}

//dodanie pacjenta
void Db::setNewPatient(QString patientData){
    QStringList list= patientData.split("#");
    connectAndExecuteSelect("INSERT INTO PACJENCI (PESEL, NAZWISKO, PLEC, DATA_URODZENIA, WIEK) VALUES ('"+list.takeAt(0)+"','"+list.takeAt(1)+"','"+list.takeAt(2)+"','"+list.takeAt(3)+"','"+list.takeAt(4)+"');");

}

//dodanie nowego badania
void Db::setNewStudy(QString patientID, QString studyData){
    QStringList list= studyData.split("#");
    connectAndExecuteSelect("INSERT INTO BADANIA (ID_BADANIA_GLOWNE, PESEL_PACJENTA, ID_DICOM, TYP_DICOM,DATA, CZAS, MODALNOSC, OPIS, ID_BADANIA_OD_LEKARZA)VALUES ('"+list.takeAt(0)+"','"+patientID+"','"+list.takeAt(1)+"','"+list.takeAt(2)+"','"+list.takeAt(3)+"','"+list.takeAt(4)+"','"+list.takeAt(5)+"','"+list.takeAt(6)+"','"+list.takeAt(7)+"');");
}

//dodanie nowej serii patinetID jest nieużywane w tej tabeli, usuwam.
void Db::setNewSeries(QString patinetID,QString studyID, QString seriesData){
    QStringList list= seriesData.split("#");
    connectAndExecuteSelect("INSERT INTO SERIA (ID_SERII, ID_BADANIA_GLOWNE, DATA, CZAS, ILOSC_OBRAZOW, OPIS, PESEL_PACJENTA) VALUES ('"+list.takeAt(0)+"','"+studyID+"','"+list.takeAt(1)+"','"+list.takeAt(2)+"','"+list.takeAt(3)+"','"+list.takeAt(4)+"','"+patinetID+"');");
}

//dodanie nowego obrazu patientID i studyID są nieużywane w tej tabeli usuwam
void Db::setNewPicture(QString patinetID, QString studyID,QString seriesID, QString pictureData){
    QStringList list= pictureData.split("#");
    connectAndExecuteSelect("INSERT INTO OBRAZY (ID_OBRAZU, ID_SERII, NUMER_W_KOLEJNOSCI, SCIEZKA, PESEL_PACJENTA , ID_BADANIA_GLOWNE) VALUES ('"+list.takeAt(0)+"','"+seriesID+"','"+list.takeAt(1)+"','"+list.takeAt(2)+"','"+patinetID+"','"+studyID+"');");
}

//usuniecie pacjenta
void Db::delNewPatient(QString patientID){
    connectAndExecuteSelect("DELETE FROM PACJENCI a WHERE a.PESEL = '"+patientID+"'");

}

//usuniecie badania
void Db::delNewStudy(QString patientID, QString studyID){
    connectAndExecuteSelect("DELETE FROM BADANIA a WHERE a.ID_BADANIA_GLOWNE = '"+studyID+"' AND a.PESEL_PACJENTA = '"+patientID+"'");

}

//usuniecie serii
void Db::delNewSeries(QString patinetID, QString studyID, QString seriesID){
    connectAndExecuteSelect("DELETE FROM SERIA a WHERE a.ID_SERII = '"+seriesID+"' AND a.ID_BADANIA_GLOWNE = '"+studyID+"' AND a.PESEL_PACJENTA = '"+patinetID+"'");

}

//usuniecie obrazu
void Db::delNewPicture(QString patientID, QString studyID, QString seriesID, QString pictureID){
    connectAndExecuteSelect("DELETE FROM OBRAZY WHERE ID_OBRAZU = '"+pictureID+"' AND ID_SERII = '"+seriesID+"' AND PESEL_PACJENTA = '"+patientID+"' AND ID_BADANIA_GLOWNE = '"+studyID+"'");
}

//usuniecie badania
void Db::delNewStudy2(QString patientID){

     connectAndExecuteSelect("DELETE FROM BADANIA a WHERE a.PESEL_PACJENTA = '"+patientID+"'");
}
//usuniecie serii
void Db::delNewSeries2(QString patinetID){ connectAndExecuteSelect("DELETE FROM SERIA a WHERE a.PESEL_PACJENTA = '"+patinetID+"'");
                                         }
//usuniecie obrazow
void Db::delNewPicture2(QString patientID){connectAndExecuteSelect("DELETE FROM OBRAZY WHERE  PESEL_PACJENTA = '"+patientID+"'");

                                     }

  //usuniecie serii
 void Db::delNewSeries3(QString patinetID, QString studyID){connectAndExecuteSelect("DELETE FROM SERIA a WHERE a.ID_BADANIA_GLOWNE = '"+studyID+"' AND a.PESEL_PACJENTA = '"+patinetID+"'");
                                                           }
 //usuniecie obrazu
void Db::delNewPicture3(QString patientID, QString studyID){ connectAndExecuteSelect("DELETE FROM OBRAZY WHERE PESEL_PACJENTA = '"+patientID+"' AND ID_BADANIA_GLOWNE = '"+studyID+"'");

                                                           }
//usuniecie obrazu
void Db::delNewPicture4(QString patientID, QString studyID, QString seriesID){ connectAndExecuteSelect("DELETE FROM OBRAZY WHERE PESEL_PACJENTA = '"+patientID+"' AND ID_SERII = '"+seriesID+"' AND ID_BADANIA_GLOWNE = '"+studyID+"'");

                                                          }
//weryfikacja
bool Db::ifPatientExist(QString patientID){
    bool wynik=true;
    if(connectAndExecuteSelect("select * from PACJENCI where pesel='"+patientID+"'").isEmpty()) {
        cout<<"wynik: "<<wynik<<endl;
        wynik=false;}else{
        wynik = true;
    }
    return wynik;
}
bool Db::ifStudyExist(QString patientID, QString studyID){
    bool wynik=true;
    if(connectAndExecuteSelect("select * from BADANIA WHERE ID_BADANIA_GLOWNE = '"+studyID+"' AND PESEL_PACJENTA = '"+patientID+"'").isEmpty())  {
        cout<<"wynik: "<<wynik<<endl;
        wynik=false;}else{
        wynik = true;
    }
    return wynik;
}
bool Db::ifSeriesExist(QString patinetID, QString studyID, QString seriesID){
    bool wynik=true;
    if(connectAndExecuteSelect("select * from SERIA WHERE ID_SERII = '"+seriesID+"' AND ID_BADANIA_GLOWNE = '"+studyID+"' AND PESEL_PACJENTA = '"+patinetID+"'").isEmpty()){
        cout<<"wynik: "<<wynik<<endl;
        wynik=false;}else{
        wynik = true;
    }
    return wynik;
}
bool Db::ifPictureExist(QString patientID, QString studyID, QString seriesID, QString pictureID){
    bool wynik=true;
    if(connectAndExecuteSelect("select * from OBRAZY WHERE ID_OBRAZU = '"+pictureID+"' AND ID_SERII = '"+seriesID+"' AND PESEL_PACJENTA = '"+patientID+"' AND ID_BADANIA_GLOWNE = '"+studyID+"'").isEmpty()) {
        cout<<"wynik: "<<wynik<<endl;
        wynik=false;}else{
        wynik = true;
    }
    return wynik;
}
