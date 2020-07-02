#include "AI/DefaultAIThread.h"

DefaultAIThread::DefaultAIThread(QObject* parent):
    QThread (parent)
{

}

void DefaultAIThread::initAI(ChessType type , const QList<ChessType> types){
    aiChessType = type;
    againstChessType = type == BlackChess ? WhiteChess : BlackChess;
    for(auto A : types)
        currentTypeList.append(A);
}

void DefaultAIThread::run(){
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




