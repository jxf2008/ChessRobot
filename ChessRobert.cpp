#include "ChessRobert.h"

ChessRobert::ChessRobert(QWidget *parent)
    : QMainWindow(parent){
    setWindowIcon(QIcon(EXE_ICO));
    chessMap_Widget = new ChessMapWidget;
    setCentralWidget(chessMap_Widget);
}

ChessRobert::~ChessRobert()
{

}
