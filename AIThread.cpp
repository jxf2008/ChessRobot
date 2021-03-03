#include <QLibrary>
#include "InitBuilder.h"
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
