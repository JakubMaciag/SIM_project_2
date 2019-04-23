#include "loadfile.h"
//konstruktor
LoadFile::LoadFile()
{

}
// załadowanie danych medycznych dicom do programu
DcmDataset* LoadFile::lodingFileFromPath(QString filePath)
{
    string filePathStr = filePath.toStdString();
    DcmFileFormat file_format;
    //sprawdzenie poprawnosci wczytanego pliku (jego status)
    OFCondition condition = file_format.loadFile(filePathStr.c_str());
    //wczytanie danych z dicom - dataset
    DcmDataset* dataset = file_format.getDataset();
    if (condition.good())  {
        showMessage("File is correct!");
    } else {
        showMessage("Error with dataset!");
    }
    return dataset;
}
//zaladowanie obrazu do programu
QPixmap LoadFile::loadImageFromDCM(QString filePath)
{
    QPixmap pixMap;
    DicomImage *imagex = new DicomImage(filePath.toStdString().c_str());
    if (imagex != NULL)
    {
        if (imagex->getStatus() == EIS_Normal)
        {
            //konwersja
            Uint8 *pixelData = (Uint8 *)(imagex->getOutputData(8 /* bits per sample */));
            if (pixelData != NULL)
            {
                QImage image2( pixelData, imagex->getWidth(), imagex->getHeight(), QImage::Format_Grayscale8);
                pixMap.convertFromImage( image2.scaled(imagex->getWidth(), imagex->getHeight()), Qt::AutoColor) ;
            }
        } else
            cerr << "Error: cannot load DICOM image (" << DicomImage::getString(imagex->getStatus()) << ")" << endl;
    }
    delete imagex;
    return pixMap;
}
//wyswietlanie wiadomosci
void LoadFile::showMessage(QString msg)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.exec();
}
//usuwanie z danych xml zbednych elementow
QByteArray LoadFile::replaceDataText(QByteArray data)
{
    data.replace("<","");
    data.replace(">","");
    data.replace("element tag=","");
    data.replace("/data-set","");
    data.replace("/file-format","");
    data.replace("/element","");
    data.replace("?xml version=\"1.0\"?","");
    data.replace('"'," ");
    return data;
}
//konwersja dcm do xml za pomoca programu z biblioteki DCMTK i konsoli linuxowej - plik otworzony
void LoadFile::convert(QString filePath)
{
    QProcess::execute("dcm2xml "+filePath+" "+"tmpXml.xml");
}
//konwersja dcm do xml za pomoca programu z biblioteki DCMTK i konsoli linuxowej - plik z bazy
void LoadFile::convert2(QString filePath)
{
    QProcess::execute("dcm2xml "+filePath+" "+"tmpXml2.xml");
}
//usuwanie pliku tymczasowego xml - plik otworzony
void LoadFile::deleteConvertedFiles()
{
    QProcess::execute("rm tmpXml.xml");
}
////usuwanie pliku tymczasowego xml - plik z bazy
void LoadFile::deleteConvertedFiles2()
{
    QProcess::execute("rm tmpXml2.xml");
}
//pobranie sciezki do zapisu w bazie danych - na dysku [sciezka konczy sie nazwa wczytywanego obrazu]
QString LoadFile::getPathForServer(QString filePath){
    string filePathStr = filePath.toStdString();
    //wczytanie dicom i sprawdzenie statusu
    DcmFileFormat file_format;
    OFCondition status = file_format.loadFile(filePathStr.c_str());
    if (status.bad()) {
        showMessage("Error with opening the file!");
    }
    //wczytanie dataset i sprawdzenie poprawnosci - status
    DcmDataset* dataset = file_format.getDataset();
    OFString patient_id, study_id, seriesID;
    OFCondition condition;
    //sprawdzenie czy wczytalo id pacjenta, badania i serii
    condition = dataset->findAndGetOFStringArray(DCM_PatientID, patient_id);
    dataset->findAndGetOFStringArray(DCM_StudyID, study_id);
    dataset->findAndGetOFStringArray(DCM_SeriesNumber, seriesID);
    if (!condition.good())  {
        showMessage("Error with file data!");
    }
//    tworzenie sciezki do pliku
    string st1 = patient_id.c_str();
    string st2 = study_id.c_str();
    string st3 = seriesID.c_str();
    string str0 = st1+"/"+st2+"/"+st3;
    QStringList query = filePath.split("/");
    QString nameOfFile = query.last();
    QString path = QString::fromStdString(str0);
    path = path+"/"+nameOfFile;
    return path;
}
//pobranie danych z pliku dicom
QString LoadFile::getData(QString filePath, QString pathInDB){
    //wczytanie danych z dicomu i przypisanie ich do zmiennych
    QString dataList;
    string filePathStr = filePath.toStdString();
    DcmFileFormat file_format;
    OFCondition status = file_format.loadFile(filePathStr.c_str());
    DcmDataset* dataset = file_format.getDataset();
    OFString patient_id, surname, sex, date,age, study_id, tag, nrtag, studyDate, studyTime, modality, descrStudy, anyStudId, seriesID,serDate, seTime,numberOfSlices, serDesc,picId, picNumber,path;
    dataset->findAndGetOFStringArray(DCM_PatientID, patient_id);
    dataset->findAndGetOFStringArray(DCM_PatientName, surname);
    dataset->findAndGetOFStringArray(DCM_PatientSex, sex);
    dataset->findAndGetOFStringArray(DCM_PatientBirthDate, date);
    dataset->findAndGetOFStringArray(DCM_PatientAge, age);
    dataset->findAndGetOFStringArray(DCM_StudyID, study_id);
    dataset->findAndGetOFStringArray(DCM_StudyID, tag);
    dataset->findAndGetOFStringArray(DCM_StudyID, nrtag);
    dataset->findAndGetOFStringArray(DCM_StudyDate, studyDate);
    dataset->findAndGetOFStringArray(DCM_StudyTime, studyTime);
    dataset->findAndGetOFStringArray(DCM_Modality, modality);
    dataset->findAndGetOFStringArray(DCM_StudyDescription, descrStudy);
    dataset->findAndGetOFStringArray(DCM_StudyInstanceUID, anyStudId);
    dataset->findAndGetOFStringArray(DCM_SeriesNumber, seriesID);
    dataset->findAndGetOFStringArray(DCM_SeriesDate, serDate);
    dataset->findAndGetOFStringArray(DCM_SeriesTime, seTime);
    dataset->findAndGetOFStringArray(DCM_SeriesDescription, serDesc);
    dataset->findAndGetOFStringArray(DCM_NumberOfSlices, numberOfSlices);
    dataset->findAndGetOFStringArray(DCM_ImageID, picId);
    dataset->findAndGetOFStringArray(DCM_ImageIndex , picNumber);
   //path = "/home/"+patient_id+"/"+study_id+"/"+seriesID+"/"+picId;
    //konwersja ze stringa do QStringa
    QString sx1,sx2,sx3,sx4,sx5,sx6,sx7,sx8,sx9,sx10,sx11,sx12,sx13,sx14,sx15,sx16,sx17,sx18,sx19,sx20,sx21;
    string st1 = patient_id.c_str();
    sx1 =QString::fromStdString(st1);
    if(sx1.isEmpty()||sx1.isNull())
        sx1 = "1";
    string st2 = surname.c_str();
    sx2=QString::fromStdString(st2);
    if(sx2.isEmpty()||sx2.isNull())
        sx2 = "no_data";
    string st3 = sex.c_str();
    sx3=QString::fromStdString(st3);
    if(sx3.isEmpty()||sx3.isNull())
        sx3 = "no_data";
    string st4 = date.c_str();
    sx4=QString::fromStdString(st4);
    if(sx4.isEmpty()||sx4.isNull())
        sx4 = "no_data";
    string st5 = age.c_str();
    sx5=QString::fromStdString(st5);
    if(sx5.isEmpty()||sx5.isNull())
        sx5 = "1";
    string st6 = study_id.c_str();
    sx6=QString::fromStdString(st6);
    if(sx6.isEmpty()||sx6.isNull())
        sx6 = "1";
    string st7 = tag.c_str();
    sx7=QString::fromStdString(st7);
    if(sx7.isEmpty()||sx7.isNull())
        sx7 = "1";
    string st8 = nrtag.c_str();
    sx8=QString::fromStdString(st8);
    if(sx8.isEmpty()||sx8.isNull())
        sx8 = "1";
    string st9 = studyDate.c_str();
    sx9=QString::fromStdString(st9);
    if(sx9.isEmpty()||sx9.isNull())
        sx9 = "no_data";
    string st10 = studyTime.c_str();
    sx10=QString::fromStdString(st10);
    if(sx10.isEmpty()||sx10.isNull())
        sx10 = "no_data";
    string st11 = modality.c_str();
    sx11=QString::fromStdString(st11);
    if(sx11.isEmpty()||sx11.isNull())
        sx11 = "no_data";
    string st12 = descrStudy.c_str();
    sx12=QString::fromStdString(st12);
    if(sx12.isEmpty()||sx12.isNull())
        sx12 = "no_data";
    string st13 = anyStudId.c_str();
    sx13=QString::fromStdString(st13);
    if(sx13.isEmpty()||sx13.isNull())
        sx13 = "no_data";
    string st14 = seriesID.c_str();
    sx14=QString::fromStdString(st14);
    if(sx14.isEmpty()||sx14.isNull())
        sx14 = "1";
    string st15 = serDate.c_str();
    sx15=QString::fromStdString(st15);
    if(sx15.isEmpty()||sx15.isNull())
        sx15 = "no_data";
    string st16 = seTime.c_str();
    sx16=QString::fromStdString(st16);
    if(sx17.isEmpty()||sx17.isNull())
        sx17 = "no_data";
    string st17 = numberOfSlices.c_str();
    sx17=QString::fromStdString(st17);
    if(sx17.isEmpty()||sx17.isNull())
        sx17 = "1";
    string st18 = serDesc.c_str();
    sx18=QString::fromStdString(st18);
    if(sx18.isEmpty()||sx18.isNull())
        sx18 = "no_data";
    string st19 = picId.c_str();
    sx19=QString::fromStdString(st19);
    if(sx19.isEmpty()||sx19.isNull())
        sx19 = "1";
    string st20 = picNumber.c_str();
    sx20=QString::fromStdString(st20);
    if(sx20.isEmpty()||sx20.isNull())
        sx20 = "1";
    string st21 = path.c_str();
    sx21=QString::fromStdString(st21);
    if(sx21.isEmpty()||sx21.isNull())
        sx21 = "no_data";
    dataList = sx1+"#"+sx2+"#"+sx3+"#"+sx4+"#"+sx5+"#"+sx6+"#"+sx7+"#"+sx8+"#"+sx9+"#"+sx10+"#"+sx11+"#"+sx12+"#"+sx13+"#"+sx14+"#"+sx15+"#"+sx16+"#"+sx17+"#"+sx18+"#"+sx19+"#"+sx20+"#"+sx21;
    return dataList;
}
