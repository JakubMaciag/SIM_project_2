#ifndef SENDFILE_H
#define SENDFILE_H
#include "dcmtklib.h"
using namespace std;

class SendFile
{
public:
    SendFile();
    void sendIntoFile(QString path, QString pathinto) ;
    void showMessage(QString msg);
    void deleteFileInPath(QString pathinto);
    void deleteFolder(QString pathinto);
    void saveOnAnyDisk(QString pathinto, QString pathout);
};

#endif // SENDFILE_H
