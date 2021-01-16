```c++
#ifndef ANALYSISFUNCTION_H
#define ANALYSISFUNCTION_H

#include "Init/ConstData.h"

int getChessDetail(const QList<ChessType>& currentTypeList , ChessDetail& chessDetail , int row , int col, ChessType chessType , int len = VICTORY_CONDITION);
int maxChessLen(const ChessDetail& chessDetail , int& maxLen , int& counts);
int analysisChessDetail(const QList<ChessDetail>& chessDetailList , int& maxLen , int& counts , int& index);

#endif // ANALYSISFUNCTION_H

```

```c++
#include "AI/AnalysisFunction.h"

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





































```

```c++
#ifndef AI1THREAD_H
#define AI1THREAD_H

#include <QThread>
#include "AnalysisFunction.h"

class AIThread : public QThread
{
    Q_OBJECT
private:
    QList<ChessType> currentTypeList;
    ChessType aiChessType;
    ChessType againstChessType;
    QString aiNameString;

    CALCULATECHESS customAI;
public:
    AIThread(QObject* parent = nullptr);
    void initAI(ChessType type , const QList<ChessType> types);
    void setAIName(const QString& nm);
protected:
    void run();
signals:
    void aiClick(int indexs);
};

#endif // AI1THREAD_H

```

```c++
#include <QLibrary>
#include "Init/InitBuilder.h"
#include "AIThread.h"

class InitBuilder;

AIThread::AIThread(QObject* parent):
    QThread (parent),
    aiNameString(DEFAULT_AI_NAME),
    customAI(nullptr)
{

}

void AIThread::initAI(ChessType type , const QList<ChessType> types){
    aiChessType = type;
    againstChessType = type == BlackChess ? WhiteChess : BlackChess;
    for(auto A : types)
        currentTypeList.append(A);
}

void AIThread::setAIName(const QString& nm){
    aiNameString = nm;
    if(aiNameString != DEFAULT_AI_NAME){
        QString dllFilePath = InitBuilder::getInstance().getAIDir() + "/" + aiNameString + AI_LIBARY_FILE;
        QLibrary loadDll(dllFilePath);
        customAI = reinterpret_cast<CALCULATECHESS>(loadDll.resolve(AI_FUN_NAME.toStdString().c_str()));
    }
}

void AIThread::run(){
    if(aiNameString != DEFAULT_AI_NAME){
        int ct[CHESS_COUNT*CHESS_COUNT];
        for(int i = 0 ; i < CHESS_COUNT * CHESS_COUNT ; ++i)
            ct[i] = currentTypeList.at(i);
        int indexs = -1;
        if(customAI)
            indexs = customAI(ct,CHESS_COUNT * CHESS_COUNT , aiChessType);
        emit aiClick(indexs);
        return;
    }
    bool isFirstChess = true;
    for(auto A : currentTypeList){
        if(A == ChessType::BlackChess || A == ChessType::WhiteChess){
            isFirstChess = false;
            break;
        }
    }
    if(isFirstChess){
        emit aiClick(CHESS_COUNT * CHESS_COUNT / 2);
        return;
    }
    int aiMaxLen = 0;
    int aiLineCount = 0;
    int aiChessIndex = 0;

    int againstMaxLen = 0;
    int againstLineCount = 0;
    int againstChessIndex = 0;

    QList<ChessDetail> aiChessDetailList;
    QList<ChessDetail> againstChessDetailList;
    for(int r = 0 ; r < CHESS_COUNT ; ++r){
        for(int c = 0 ; c < CHESS_COUNT ; ++c){
            ChessType ct = currentTypeList.at(r*CHESS_COUNT+c);
            if(ct != ChessType::EmptyChess)
                continue;
            ChessDetail aiChessDetail;
            ChessDetail againstChessDetail;

            getChessDetail(currentTypeList,aiChessDetail,r,c,aiChessType);
            getChessDetail(currentTypeList,againstChessDetail,r,c,againstChessType);

            aiChessDetailList.append(aiChessDetail);
            againstChessDetailList.append(againstChessDetail);
        }
    }
    analysisChessDetail(aiChessDetailList,aiMaxLen,aiLineCount,aiChessIndex);
    analysisChessDetail(againstChessDetailList,againstMaxLen,againstLineCount,againstChessIndex);

    if(aiMaxLen > againstMaxLen)
        emit aiClick(aiChessIndex);
    if(aiMaxLen < againstMaxLen)
        emit aiClick(againstChessIndex);
    if(aiMaxLen == againstMaxLen && aiLineCount > againstLineCount)
        emit aiClick(aiChessIndex);
    if(aiMaxLen == againstMaxLen && aiLineCount == againstLineCount)
        emit aiClick(aiChessIndex);
    if(aiMaxLen == againstMaxLen && aiLineCount < againstLineCount)
        emit aiClick(aiChessIndex);
}



































```