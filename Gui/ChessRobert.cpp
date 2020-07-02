#include "ChessRobert.h"

ChessRobert::ChessRobert(QWidget *parent)
    : QMainWindow(parent)
{
    chessMap_Widget = new ChessMapWidget;
    setCentralWidget(chessMap_Widget);
}

ChessRobert::~ChessRobert()
{

}