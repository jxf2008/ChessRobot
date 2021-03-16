#include "ChessRobert.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*QString qssFilePath = QCoreApplication::applicationDirPath() + "/" + "AppStyle.qss";
    qssFilePath = "E:/";
    qssFilePath += "AppStyle.qss";
    QFile qssFile(qssFilePath);
    qssFile.open(QIODevice::ReadOnly);
    QTextStream textStream(&qssFile);
    QString styleStr = textStream.readAll();
    a.setStyleSheet(styleStr);*/
    ChessRobert w;
    w.show();

    return a.exec();
}
