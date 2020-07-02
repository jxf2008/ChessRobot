#ifndef ANALYSISFUNCTION_H
#define ANALYSISFUNCTION_H

#include "Init/ConstData.h"

int getChessDetail(const QList<ChessType>& currentTypeList , ChessDetail& chessDetail , int row , int col, ChessType chessType , int len = VICTORY_CONDITION);
int maxChessLen(const ChessDetail& chessDetail , int& maxLen , int& counts);
int analysisChessDetail(const QList<ChessDetail>& chessDetailList , int& maxLen , int& counts , int& index);

#endif // ANALYSISFUNCTION_H
