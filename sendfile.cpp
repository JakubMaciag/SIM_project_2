#include "sendfile.h"
//konstruktor
SendFile::SendFile()
{

}
//wyswietlanie powiadomien

void SendFile::showMessage(QString msg)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.exec();
}
//kopiowanie pliku do dysku bazy po odpowiedniej sciezce
void SendFile::sendIntoFile(QString pathfrom, QString pathinto)
{
    //rozdzielenie sciezki na skladowe - po to bo sciezka zawiera nazwe pliku a trzeba stworzyc foldery po drodze

    QStringList query = pathinto.split("/");
    QString str = query.takeAt(0)+"/"+query.takeAt(1)+"/"+query.takeAt(2)+"/"+query.takeAt(3)+"/"+query.takeAt(4);
    //tworzenie folderow
    QDir dir(str);
    if (!dir.exists()) {
        dir.mkpath(str);
    }
    //kopiowanie pliku
    QProcess::execute("cp "+pathfrom+" "+pathinto);
}
//usuwanie pliku dcm na dysku
void SendFile::deleteFileInPath(QString pathinto){
        QProcess::execute("rm "+pathinto);
        showMessage("File is deleted");
}
//usuwanie folderu wraz z zawartoscia dicom
void SendFile::deleteFolder(QString pathinto)
{
    cout<<pathinto.toStdString()<<endl;
    QProcess::execute("rm -r "+pathinto);
    showMessage("File is deleted");
}
//zapis pliku po podaniu sciezki wejsciowej i wyjsciowej - stosowane przy pobraniu z bazy
void SendFile::saveOnAnyDisk(QString pathinto, QString pathout)
{
    QProcess::execute("cp "+pathinto+" "+pathout);
}
