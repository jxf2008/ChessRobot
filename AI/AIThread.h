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
