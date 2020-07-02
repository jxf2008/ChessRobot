#include "Gui/ChessRobert.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChessRobert w;
    w.show();

    return a.exec();
}
