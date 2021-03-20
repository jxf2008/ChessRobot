#include <QApplication>
#include "ChessRobert.h"
#include "InitBuilder.h"

class InitBuilder;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InitBuilder::getInstance().createFile();
    ChessRobert w;
    w.show();
    return a.exec();
}
