#ifndef LOADFILE_H
#define LOADFILE_H
#include "dcmtklib.h"

using namespace std;
class LoadFile
{
public:
    LoadFile();
    DcmDataset* lodingFileFromPath(QString filePath);
    QPixmap loadImageFromDCM(QString filePath);
    QPixmap image;
    QByteArray replaceDataText(QByteArray data);
    void deleteConvertedFiles();
    void convert(QString filePath);
    void deleteConvertedFiles2();
    void convert2(QString filePath);
    void showMessage(QString msg);
    QString getData(QString filePath,  QString pathInDB);
    QString getPathForServer(QString filePath);

};

#endif // LOADFILE_H
