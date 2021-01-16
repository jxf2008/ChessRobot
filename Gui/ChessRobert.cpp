#include "ChessRobert.h"
#include "Init/InitBuilder.h"

class InitBuilder;

ChessRobert::ChessRobert(QWidget *parent)
    : QMainWindow(parent)
{
    InitBuilder::getInstance().createFile();
    setWindowIcon(QIcon(EXE_ICO));
    chessMap_Widget = new ChessMapWidget;
    setCentralWidget(chessMap_Widget);
}

ChessRobert::~ChessRobert()
{

}