#ifndef AITHREAD_H
#define AITHREAD_H

#include <QThread>
#include "AnalysisFunction.h"

class DefaultAIThread : public QThread
{
    Q_OBJECT
private:
    QList<ChessType> currentTypeList;
    ChessType aiChessType;
    ChessType againstChessType;
public:
    DefaultAIThread(QObject* parent = nullptr);
    void initAI(ChessType type , const QList<ChessType> types);
protected:
    void run();
signals:
    void aiClick(int indexs);
};

#endif // AITHREAD_H