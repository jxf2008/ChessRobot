#ifndef CONSTDATA_H
#define CONSTDATA_H

#include <QDebug>

const QString EMPTY_PIX = ":/images/EmptyChess.png";
const QString WHITE_PIX = ":/images/WhiteChess.png";
const QString BLACK_PIX = ":/images/BlackChess.png";

enum ChessType
{
    BlackChess,
    WhiteChess,
    EmptyChess
};

struct ChessDetail
{
    int row;
    int col;
    int verLine;   //每个旗子在4个方向上的数量，如果这条线2个对方的棋子小于5，则该值为-1
    int horLine;
    int leftUpLine;
    int rightUpLine;
};

enum ErroInfo{
    NORMAL = 0,
    ARG_ERRO = 1,
};

const int CHESS_SIZE = 25;    //棋子的大小

const int CHESS_COUNT = 19;   //棋盘大小，每行/列的最大棋子数

const int VICTORY_CONDITION = 5;   //胜利条件

const int LOG_SIZE = 50;  //对战双方LOG的大小

const QString PLAYER_NAME = "玩家";

const QString DEFAULT_AI_NAME = "默认AI";

#endif // CONSTDATA_H