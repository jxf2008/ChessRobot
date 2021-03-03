#ifndef CHESSROBERT_H
#define CHESSROBERT_H

#include <QMainWindow>
#include "ChessMapWidget.h"

class ChessMapWidget;

class ChessRobert : public QMainWindow
{
    Q_OBJECT
private:
    ChessMapWidget* chessMap_Widget;
public:
    ChessRobert(QWidget *parent = nullptr);
    ~ChessRobert();
};

#endif // CHESSROBERT_H