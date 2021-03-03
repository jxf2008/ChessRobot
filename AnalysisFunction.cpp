#ifdef MSVC
#pragma  execution_character_set("utf-8")
#endif

#include "AnalysisFunction.h"

int getChessDetail(const QList<ChessType>& currentTypeList ,ChessDetail& chessDetail , int row , int col, ChessType chessType , int len){
    int chessCount = 0;  //己方棋子的数量
    int rangeLen = 0;   //没有对方棋子的范围
    bool sameTypeChess = true;
    ChessType againstType = chessType == ChessType::WhiteChess ? ChessType::BlackChess : ChessType::WhiteChess;

    bool hasAgainstHead = false;   //头和尾是空白棋子还是对方棋子
    bool hasAgainstTail = false;

    for(int c = col + 1 ; c < CHESS_COUNT ; ++c){
        ChessType currentChessType = currentTypeList.at(row*CHESS_COUNT+c);
        if(currentChessType == chessType && sameTypeChess)
            ++chessCount;
        if(currentChessType == ChessType::EmptyChess)
            sameTypeChess = false;
        if(currentChessType == againstType)
            break;
        ++rangeLen;
    }
    if(sameTypeChess)
        hasAgainstTail = true;
    sameTypeChess = true;
    for(int c = col - 1 ; c >= 0 ; --c){
        ChessType currentChessType = currentTypeList.at(row*CHESS_COUNT+c);
        if(currentChessType == chessType && sameTypeChess)
            ++chessCount;
        if(currentChessType == ChessType::EmptyChess)
            sameTypeChess = false;
        if(currentChessType == againstType)
            break;
        ++rangeLen;
    }
    if(sameTypeChess)
        hasAgainstHead = true;
    chessDetail.horLine = rangeLen >= len ? chessCount : -1;
    if((hasAgainstHead || hasAgainstTail) && chessDetail.horLine != len - 1 && chessDetail.horLine > 0)
        chessDetail.horLine = 0;
    hasAgainstHead = false;
    hasAgainstTail = false;

    chessCount = 0;
    sameTypeChess = true;
    for(int r = row + 1 ; r < CHESS_COUNT ; ++r){
        ChessType currentChessType = currentTypeList.at(r*CHESS_COUNT+col);
        if(currentChessType == chessType && sameTypeChess)
            ++chessCount;
        if(currentChessType == ChessType::EmptyChess)
            sameTypeChess = false;
        if(currentChessType == againstType)
            break;
        ++rangeLen;
    }
    if(sameTypeChess)
        hasAgainstHead = true;
    sameTypeChess = true;
    for(int r = row - 1 ; r >= 0 ; --r){
        ChessType currentChessType = currentTypeList.at(r*CHESS_COUNT+col);
        if(currentChessType == chessType && sameTypeChess)
            ++chessCount;
        if(currentChessType == ChessType::EmptyChess)
            sameTypeChess = false;
        if(currentChessType == againstType)
            break;
        ++rangeLen;
    }
    if(sameTypeChess)
        hasAgainstTail = true;
    chessDetail.verLine = rangeLen >= len ? chessCount : -1;
    if((hasAgainstHead || hasAgainstTail) && chessDetail.verLine != len - 1 && chessDetail.verLine > 0)
        chessDetail.verLine = 0;
    hasAgainstHead = false;
    hasAgainstTail = false;

    chessCount = 0;
    sameTypeChess = true;
    for(int c = col - 1 , r = row - 1 ; c >= 0 && r >= 0 ; --c,--r){
        ChessType currentChessType = currentTypeList.at(r*CHESS_COUNT+c);
        if(currentChessType == chessType && sameTypeChess)
            ++chessCount;
        if(currentChessType == ChessType::EmptyChess)
            sameTypeChess = false;
        if(currentChessType == againstType)
            break;
        ++rangeLen;
    }
    if(sameTypeChess)
        hasAgainstHead = true;
    sameTypeChess = true;
    for(int c = col + 1 , r = row + 1 ; c < CHESS_COUNT && r < CHESS_COUNT ; ++c,++r){
        ChessType currentChessType = currentTypeList.at(r*CHESS_COUNT+c);
        if(currentChessType == chessType && sameTypeChess)
            ++chessCount;
        if(currentChessType == ChessType::EmptyChess)
            sameTypeChess = false;
        if(currentChessType == againstType)
            break;
        ++rangeLen;
    }
    if(sameTypeChess)
        hasAgainstTail = true;
    chessDetail.leftUpLine = rangeLen >= len ? chessCount : -1;
    if((hasAgainstHead || hasAgainstTail) && chessDetail.leftUpLine != len - 1 && chessDetail.leftUpLine > 0)
        chessDetail.leftUpLine = 0;
    hasAgainstHead = false;
    hasAgainstTail = false;

    chessCount = 0;
    sameTypeChess = true;
    for(int c = col + 1 , r = row - 1 ; c < CHESS_COUNT && r >= 0 ; ++c,--r){
        ChessType currentChessType = currentTypeList.at(r*CHESS_COUNT+c);
        if(currentChessType == chessType && sameTypeChess)
            ++chessCount;
        if(currentChessType == ChessType::EmptyChess)
            sameTypeChess = false;
        if(currentChessType == againstType)
            break;
        ++rangeLen;
    }
    if(sameTypeChess)
        hasAgainstHead = true;
    sameTypeChess = true;
    for(int c = col - 1 , r = row + 1 ; c >= 0 && r < CHESS_COUNT ; --c,++r){
        ChessType currentChessType = currentTypeList.at(r*CHESS_COUNT+c);
        if(currentChessType == chessType && sameTypeChess)
            ++chessCount;
        if(currentChessType == ChessType::EmptyChess)
            sameTypeChess = false;
        if(currentChessType == againstType)
            break;
        ++rangeLen;
    }
    if(sameTypeChess)
        hasAgainstTail = true;
    chessDetail.rightUpLine = rangeLen >= len ? chessCount : -1;
    if((hasAgainstHead || hasAgainstTail) && chessDetail.rightUpLine != len - 1 && chessDetail.rightUpLine > 0)
        chessDetail.rightUpLine = 0;
    hasAgainstHead = false;
    hasAgainstTail = false;

    chessDetail.col = col;
    chessDetail.row = row;

    return ErroInfo::NORMAL;
}


int maxChessLen(const ChessDetail& chessDetail , int& maxLen , int& counts){
    maxLen = chessDetail.horLine;
    counts = 1;
    if(chessDetail.verLine > maxLen){
        maxLen = chessDetail.verLine;
        counts = 1;
    }else if(chessDetail.verLine == maxLen)
        ++counts;

    if(chessDetail.leftUpLine > maxLen){
        maxLen = chessDetail.leftUpLine;
        counts = 1;
    }else if(chessDetail.leftUpLine == maxLen)
        ++counts;

    if(chessDetail.rightUpLine > maxLen){
        maxLen = chessDetail.rightUpLine;
        counts = 1;
    }else if(chessDetail.rightUpLine == maxLen)
        ++counts;

    return ErroInfo::NORMAL;
}


int analysisChessDetail(const QList<ChessDetail>& chessDetailList , int& maxLen , int& counts , int& index){
    maxLen = -1;
    counts = -1;
    for(auto A : chessDetailList){
        int len = 0;
        int cs = 0;
        maxChessLen(A,len,cs);
        if(len > maxLen){
            maxLen = len;
            counts = cs;
            index = A.row * CHESS_COUNT + A.col;
        }
    }
    return ErroInfo::NORMAL;
}
