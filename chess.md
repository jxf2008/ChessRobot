ChessRobet.pro
```shell
#-------------------------------------------------
#
# Project created by QtCreator 2019-12-10T15:52:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChessRebot
TEMPLATE = app


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        AI1Thread.cpp \
        AnalysisFunction.cpp \
        ChessButton.cpp \
        ChessMapWidget.cpp \
        ChoosePlayerDialog.cpp \
        main.cpp \
        ChessRobert.cpp

HEADERS += \
        AI1Thread.h \
        AnalysisFunction.h \
        ChessButton.h \
        ChessMapWidget.h \
        ChessRobert.h \
        ChoosePlayerDialog.h \
        ConstData.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    source.qrc
```
AIThread
```c++
#ifndef AI1THREAD_H
#define AI1THREAD_H

#include <QThread>
#include "AnalysisFunction.h"

class AI1Thread : public QThread
{
    Q_OBJECT
private:
    QList<ChessType> currentTypeList;
    ChessType aiChessType;
    ChessType againstChessType;
public:
    AI1Thread(QObject* parent = nullptr);
    void initAI(ChessType type , const QList<ChessType> types);
protected:
    void run();
signals:
    void aiClick(int indexs);
};

#endif // AI1THREAD_H

#include "AI1Thread.h"

AI1Thread::AI1Thread(QObject* parent):
    QThread (parent)
{

}

void AI1Thread::initAI(ChessType type , const QList<ChessType> types){
    aiChessType = type;
    againstChessType = type == BlackChess ? WhiteChess : BlackChess;
    for(auto A : types)
        currentTypeList.append(A);
}

void AI1Thread::run(){
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
            if(againstChessDetail.horLine == 1){
            qDebug()<<"ROW:"<<r;
            qDebug()<<"COL:"<<c;
            qDebug()<<"-------";
            }

            aiChessDetailList.append(aiChessDetail);
            againstChessDetailList.append(againstChessDetail);
        }
    }
    /*for(auto A : againstChessDetailList){
        if(A.horLine != 1)
            continue;
        qDebug()<<"ROW:"<<A.row;
        qDebug()<<"COL:"<<A.col;
        qDebug()<<"-------";
    }*/
    analysisChessDetail(aiChessDetailList,aiMaxLen,aiLineCount,aiChessIndex);
    analysisChessDetail(againstChessDetailList,againstMaxLen,againstLineCount,againstChessIndex);
    qDebug()<<"AI-MAXLEN"<<aiMaxLen;
    qDebug()<<"AI_COUNT"<<aiLineCount;
    qDebug()<<"AGAINST-MAXLEN"<<againstMaxLen;
    qDebug()<<"AGAINST-COUNT"<<againstLineCount;
    qDebug()<<"aiChessIndex"<<aiChessIndex;
    qDebug()<<"againstChessIndex"<<againstChessIndex;

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

AnalysisFunction
```c++
#ifndef ANALYSISFUNCTION_H
#define ANALYSISFUNCTION_H

#include "ConstData.h"

int getChessDetail(const QList<ChessType>& currentTypeList , ChessDetail& chessDetail , int row , int col, ChessType chessType , int len = VICTORY_CONDITION);
int maxChessLen(const ChessDetail& chessDetail , int& maxLen , int& counts);
int analysisChessDetail(const QList<ChessDetail>& chessDetailList , int& maxLen , int& counts , int& index);

#endif // ANALYSISFUNCTION_H

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

```

ChessButton
```c++
#ifndef CHESSBUTTON_H
#define CHESSBUTTON_H

#include <QPushButton>
#include <QMouseEvent>
#include "ConstData.h"

class ChessButton : public QPushButton
{
    Q_OBJECT
private:
    ChessType chessType;
    ChessType userClickType;

    QString emptyPixString;
    QString whitePixString;
    QString blackPixString;

    bool allowUserClickBool;

    void setChessPix();
public:
    ChessButton(ChessType userClick = BlackChess,
                const QString& emptyPix = EMPTY_PIX,
                const QString& whitePix = WHITE_PIX,
                const QString& blackPix = BLACK_PIX,
                QWidget* parent = nullptr);
    void setChessType(ChessType tp);
    ChessType getChessType()const;
    void resetButtonPix(const QString& emptyPix , const QString& whitePix , const QString& blackPix);
    void setAllowClick(bool fg);
    bool buttonStatu()const;
protected:
    void mousePressEvent(QMouseEvent* event);
signals:
    void userHadClicked();
};

#endif // CHESSBUTTON_H


#include "ChessButton.h"

ChessButton::ChessButton(ChessType userClick ,
                         const QString& emptyPix,
                         const QString& whitePix,
                         const QString& blackPix,
                         QWidget* parent):
    QPushButton (parent),
    chessType (EmptyChess),
    userClickType (userClick),
    emptyPixString(emptyPix),
    whitePixString(whitePix),
    blackPixString(blackPix),
    allowUserClickBool(true){
    setDefault(false);
    setStyleSheet("border-style: outset; border-width: 2px;border-color: black;");
}

void ChessButton::setChessPix(){
    QPixmap pix;
    if(chessType == EmptyChess)
        pix.load(emptyPixString);
    if(chessType == BlackChess)
        pix.load(blackPixString);
    if(chessType == WhiteChess)
        pix.load(whitePixString);
    setIcon(pix.scaled(size()));
    setIconSize(size());
}

void ChessButton::setChessType(ChessType tp){
    chessType = tp;
    setChessPix();
}

ChessType ChessButton::getChessType()const{
    return chessType;
}

void ChessButton::resetButtonPix(const QString& emptyPix , const QString& whitePix , const QString& blackPix){
    emptyPixString = emptyPix;
    whitePixString = whitePix;
    blackPixString = blackPix;
    setChessPix();
}

void ChessButton::setAllowClick(bool fg){
    allowUserClickBool = fg;
}

bool ChessButton::buttonStatu()const{
    return allowUserClickBool;
}

void ChessButton::mousePressEvent(QMouseEvent* event)
{
    if(chessType == BlackChess || chessType == WhiteChess || !allowUserClickBool)
        return QPushButton::mousePressEvent(event);

    if(event->button() == Qt::LeftButton)
    {
        chessType = userClickType;
        setChessPix();
        emit userHadClicked();
    }
    QPushButton::mousePressEvent(event);
}
```

ChessMapWidget
```c++
#ifndef CHESSMAPWIDGET_H
#define CHESSMAPWIDGET_H

#include <QLabel>
#include "AI1Thread.h"
#include "ChessButton.h"
#include "ChoosePlayerDialog.h"

class ChessButton;
class ChoosePlayerDialog;

class ChessMapWidget : public QWidget
{
    Q_OBJECT
private:
    ChoosePlayerDialog* choosePlayer_Dialog;

    QPushButton* whitePlayerName_PushButton;
    QPushButton* whitePlayerPix_PushButton;
    QPushButton* blackPlayerName_PushButton;
    QPushButton* blackPlayerPix_PushButton;

    QLabel* vsLogo_Label;

    QPushButton* begin_QPushButton;
    QPushButton* log_PushButton;
    QPushButton* recording_PushButton;
    QPushButton* setting_PushButton;

    QString whitePlayerNameString;
    QString blackPlayerNameString;

    ChessType userType;
    QList<ChessButton*> allChessList;
    int chessJudgment();
    AI1Thread* ai1;
public:
    explicit ChessMapWidget(QWidget *parent = nullptr);
private slots:
    void userClickChess();
    void aiClickChess(int indexs);
    void choosePlayer();
    void setPlayer(QString whitePlayer , QString blackPlayer);
    void beginGame();
};

#endif // CHESSMAPWIDGET_H

#include <QGridLayout>
#include "ChessMapWidget.h"

ChessMapWidget::ChessMapWidget(QWidget *parent) :
    QWidget(parent),
    userType(BlackChess){
    choosePlayer_Dialog = nullptr;

    whitePlayerNameString = PLAYER_NAME;
    blackPlayerNameString = DEFAULT_AI_NAME;

    whitePlayerName_PushButton = new QPushButton(whitePlayerNameString);
    whitePlayerPix_PushButton = new QPushButton();
    blackPlayerName_PushButton = new QPushButton(blackPlayerNameString);
    blackPlayerPix_PushButton = new QPushButton();

    vsLogo_Label = new QLabel("<h1><font color=red>VS</font></h1>");

    begin_QPushButton = new QPushButton(tr("开始"));
    log_PushButton = new QPushButton(tr("Log"));
    recording_PushButton = new QPushButton(tr("录像"));
    setting_PushButton = new QPushButton(tr("设置"));

    whitePlayerName_PushButton->setFixedSize(LOG_SIZE*3,LOG_SIZE);
    whitePlayerPix_PushButton->setFixedSize(LOG_SIZE,LOG_SIZE);
    blackPlayerName_PushButton->setFixedSize(LOG_SIZE*3,LOG_SIZE);
    blackPlayerPix_PushButton->setFixedSize(LOG_SIZE,LOG_SIZE);

    QHBoxLayout* vs_Layout = new QHBoxLayout;
    vs_Layout->addStretch();
    vs_Layout->addWidget(whitePlayerPix_PushButton);
    vs_Layout->addWidget(whitePlayerName_PushButton);
    vs_Layout->addWidget(vsLogo_Label);
    vs_Layout->addWidget(blackPlayerName_PushButton);
    vs_Layout->addWidget(blackPlayerPix_PushButton);
    vs_Layout->addStretch();

    QGridLayout* chess_Layout = new QGridLayout;
    for(int rows = 0 ; rows < CHESS_COUNT ; ++rows){
        for(int cols = 0 ; cols < CHESS_COUNT ; ++cols){
            allChessList.append(new ChessButton);
            allChessList.last()->setFixedSize(CHESS_SIZE,CHESS_SIZE);
            allChessList.last()->setAllowClick(false);
            allChessList.last()->setChessType(ChessType::EmptyChess);
            connect(allChessList.last(),SIGNAL(userHadClicked()),this,SLOT(userClickChess()));
            chess_Layout->addWidget(allChessList.last(),rows,cols);
        }
    }
    chess_Layout->setSpacing(0);
    QVBoxLayout* chessMap_Layout = new QVBoxLayout;
    chessMap_Layout->addLayout(vs_Layout);
    chessMap_Layout->addLayout(chess_Layout);

    QVBoxLayout* ctrl_Layout = new QVBoxLayout;
    ctrl_Layout->addStretch();
    ctrl_Layout->addWidget(begin_QPushButton);
    ctrl_Layout->addWidget(log_PushButton);
    ctrl_Layout->addWidget(recording_PushButton);
    ctrl_Layout->addWidget(setting_PushButton);
    ctrl_Layout->addStretch();

    QHBoxLayout* main_Layout = new QHBoxLayout;
    main_Layout->addLayout(chessMap_Layout);
    main_Layout->addLayout(ctrl_Layout);
    setLayout(main_Layout);
    main_Layout->setSizeConstraint(QLayout::SetFixedSize);

    connect(whitePlayerName_PushButton,SIGNAL(clicked()),this,SLOT(choosePlayer()));
    connect(blackPlayerName_PushButton,SIGNAL(clicked()),this,SLOT(choosePlayer()));
    connect(begin_QPushButton,SIGNAL(clicked()),this,SLOT(beginGame()));

    whitePlayerName_PushButton->setStyleSheet("font-size:20px");
    blackPlayerName_PushButton->setStyleSheet("background-color:black;color:white;font-size:20px");
}

int ChessMapWidget::chessJudgment(){
    int maxLen = 0;
    for(int rows = 0 ; rows < CHESS_COUNT ; ++rows)
    {
        for(int cols = 0 ; cols < CHESS_COUNT ; ++cols)
        {
            ChessType tp = allChessList.at(rows * CHESS_COUNT + cols)->getChessType();
            if(tp == EmptyChess)
                continue;

            int lens = 1;
            for(int i = cols + 1 ; i < CHESS_COUNT ; ++i)    //水平方向
            {
                if(allChessList.at(rows * CHESS_COUNT + i)->getChessType() == tp)
                    ++lens;
                else
                    break;
            }
            for(int i = cols - 1 ; i >= 0 ; --i)
            {
                if(allChessList.at(rows * CHESS_COUNT + i)->getChessType() == tp)
                    ++lens;
                else
                    break;
            }
            if(lens > maxLen)
                maxLen = lens;

            lens = 1;
            for(int i = rows + 1 ; i < CHESS_COUNT ; ++i)   //垂直方向
            {
                if(allChessList.at(i * CHESS_COUNT + cols)->getChessType() == tp)
                    ++lens;
                else
                    break;
            }
            for(int i = rows - 1 ; i >= 0 ; --i)
            {
                if(allChessList.at(i * CHESS_COUNT + cols)->getChessType() == tp)
                    ++lens;
                else
                    break;
            }
            if(lens > maxLen)
                maxLen = lens;

            lens = 1;
            for(int c = cols + 1,r = rows + 1 ; (c < CHESS_COUNT)&&(r < CHESS_COUNT); ++c,++r)   //左上<-->右下方向
            {
                if(allChessList.at(r * CHESS_COUNT + c)->getChessType() == tp)
                    ++lens;
                else
                    break;
            }
            for(int c = cols - 1,r = rows - 1 ; (c >= 0)&&(r >= 0); --c,--r)
            {
                if(allChessList.at(r * CHESS_COUNT + c)->getChessType() == tp)
                    ++lens;
                else
                    break;
            }
            if(lens > maxLen)
                maxLen = lens;

            lens = 1;
            for(int c = cols + 1,r = rows - 1 ; (c < CHESS_COUNT)&&(r >= 0); ++c,--r)   //左下<-->右上方向
            {
                if(allChessList.at(r * CHESS_COUNT + c)->getChessType() == tp)
                    ++lens;
                else
                    break;
            }
            for(int c = cols - 1,r = rows + 1 ; (c >= 0)&&(r < CHESS_COUNT); --c,++r)
            {
                if(allChessList.at(r * CHESS_COUNT + c)->getChessType() == tp)
                    ++lens;
                else
                    break;
            }
            if(lens > maxLen)
                maxLen = lens;
        }
    }
    return maxLen;
}

void ChessMapWidget::userClickChess(){
    for(auto A : allChessList)
        A->setAllowClick(false);
    int cs = chessJudgment();
    if(cs >= VICTORY_CONDITION)
    {
        qDebug()<<"USER:"<<cs;
        return;
    }

    ai1 = new AI1Thread(this);
    QList<ChessType> types;
    for(auto A : allChessList)
        types.append(A->getChessType());

    ChessType aiType = userType == BlackChess ? WhiteChess : BlackChess;
    ai1->initAI(aiType,types);
    connect(ai1,SIGNAL(aiClick(int)),this,SLOT(aiClickChess(int)));
    ai1->start();
}

void ChessMapWidget::aiClickChess(int indexs){
    if(indexs == -1){
        qDebug()<<"AI认输";
        return;
    }
    allChessList.at(indexs)->setChessType(userType == BlackChess ? WhiteChess : BlackChess);
    for(auto A : allChessList)
        A->setAllowClick(true);
}

void ChessMapWidget::choosePlayer(){
    if(choosePlayer_Dialog == nullptr){
        choosePlayer_Dialog = new ChoosePlayerDialog(this);
        connect(choosePlayer_Dialog,SIGNAL(twoPlayer(QString,QString)),this,SLOT(setPlayer(QString,QString)));
    }
    QList<QString> allPlayer;
    allPlayer.append(whitePlayerNameString);
    allPlayer.append(blackPlayerNameString);
    choosePlayer_Dialog->setPlayer(allPlayer,whitePlayerNameString,blackPlayerNameString);
    choosePlayer_Dialog->exec();
}

void ChessMapWidget::setPlayer(QString whitePlayer , QString blackPlayer){
    whitePlayerName_PushButton->setText(whitePlayer);
    blackPlayerName_PushButton->setText(blackPlayer);
}

void ChessMapWidget::beginGame(){
    for(auto A : allChessList){
        A->setChessType(ChessType::EmptyChess);
        A->setAllowClick(true);
    }
    if(whitePlayerName_PushButton->text() == DEFAULT_AI_NAME){
        ai1 = new AI1Thread(this);
        QList<ChessType> types;
        for(auto A : allChessList)
            types.append(A->getChessType());

        userType = ChessType::BlackChess;
        ai1->initAI(ChessType::WhiteChess,types);
        connect(ai1,SIGNAL(aiClick(int)),this,SLOT(aiClickChess(int)));
        ai1->start();
        return;
    }
}
```

ChessRobert
```c++
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
```

ChoosePlayerDialog
```c++
#ifndef CHOOSEPLAYERDIALOG_H
#define CHOOSEPLAYERDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QCloseEvent>

class ChoosePlayerDialog : public QDialog
{
    Q_OBJECT
private:
    QLabel* warningInfo_Label;
    QLabel* whitePlayerName_Label;
    QLabel* blackPlayerName_Label;

    QListWidget* white_ListWidget;
    QListWidget* black_ListWidget;

    QPushButton* confirm_PushButton;
    QPushButton* cancel_PushButton;
public:
    ChoosePlayerDialog(QWidget* parent = nullptr);
    void setPlayer(const QList<QString> allNames , QString whitePlayer , QString blackPlayer);
protected:
    void closeEvent(QCloseEvent* event);
signals:
    void twoPlayer(QString whitePlayer , QString blackPlayer);
private slots:
    void chooseTwoPlayer();
    void changeWhitePlayer(QListWidgetItem* item);
    void changeBlackPlayer(QListWidgetItem* item);
};

#endif // CHOOSEPLAYERDIALOG_H

#include <QHBoxLayout>
#include <QVBoxLayout>
#include "ChoosePlayerDialog.h"
#include "ConstData.h"

ChoosePlayerDialog::ChoosePlayerDialog(QWidget* parent):QDialog (parent){
    warningInfo_Label = new QLabel();
    whitePlayerName_Label = new QLabel();
    blackPlayerName_Label = new QLabel();

    white_ListWidget = new QListWidget();
    black_ListWidget = new QListWidget();

    confirm_PushButton = new QPushButton(tr("确定"));
    cancel_PushButton = new QPushButton(tr("取消"));

    whitePlayerName_Label->setFixedWidth(LOG_SIZE*3);
    blackPlayerName_Label->setFixedWidth(LOG_SIZE*3);

    QHBoxLayout* playerName_Layout = new QHBoxLayout;
    playerName_Layout->addStretch();
    playerName_Layout->addWidget(whitePlayerName_Label);
    playerName_Layout->addStretch();
    playerName_Layout->addWidget(blackPlayerName_Label);
    playerName_Layout->addStretch();
    QHBoxLayout* player_Layout = new QHBoxLayout;
    player_Layout->addWidget(white_ListWidget);
    player_Layout->addWidget(black_ListWidget);
    player_Layout->setSpacing(10);
    QHBoxLayout* button_Layout = new QHBoxLayout;
    button_Layout->addStretch();
    button_Layout->addWidget(confirm_PushButton);
    button_Layout->addStretch();
    button_Layout->addWidget(cancel_PushButton);
    button_Layout->addStretch();
    QVBoxLayout* main_Layout = new QVBoxLayout;
    main_Layout->addLayout(playerName_Layout);
    main_Layout->addLayout(player_Layout);
    main_Layout->addLayout(button_Layout);
    main_Layout->addWidget(warningInfo_Label);
    setLayout(main_Layout);
    main_Layout->setSizeConstraint(QLayout::SetFixedSize);

    connect(confirm_PushButton,SIGNAL(clicked()),this,SLOT(chooseTwoPlayer()));
    connect(cancel_PushButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(white_ListWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(changeWhitePlayer(QListWidgetItem*)));
    connect(black_ListWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(changeBlackPlayer(QListWidgetItem*)));

    warningInfo_Label->setStyleSheet("color:red");
}

void ChoosePlayerDialog::setPlayer(const QList<QString> allNames , QString whitePlayer , QString blackPlayer){
    white_ListWidget->addItems(allNames);
    black_ListWidget->addItems(allNames);
    whitePlayerName_Label->setText(whitePlayer);
    blackPlayerName_Label->setText(blackPlayer);
}

void ChoosePlayerDialog::closeEvent(QCloseEvent* event){
    event->ignore();
    warningInfo_Label->setText("");
    white_ListWidget->clear();
    black_ListWidget->clear();
    hide();
}

void ChoosePlayerDialog::chooseTwoPlayer(){
    if(whitePlayerName_Label->text() == blackPlayerName_Label->text()){
        warningInfo_Label->setText("对战双方不能是同一个");
        return;
    }
    emit twoPlayer(whitePlayerName_Label->text(),blackPlayerName_Label->text());
    close();
}

void ChoosePlayerDialog::changeWhitePlayer(QListWidgetItem* item){
    whitePlayerName_Label->setText(item->text());
}

void ChoosePlayerDialog::changeBlackPlayer(QListWidgetItem* item){
    blackPlayerName_Label->setText(item->text());
}

```


ConstData
```c++
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
```

main
```c++
#include "ChessRobert.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChessRobert w;
    w.show();

    return a.exec();
}
```


source
```xml
<RCC>
  <qresource>
    <file>images/BlackChess.png</file>
	<file>images/WhiteChess.png</file>
    <file>images/EmptyChess.png</file>
  </qresource>
</RCC>
```