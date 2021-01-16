```c++
#ifndef CHESSBUTTON_H
#define CHESSBUTTON_H

#include <QPushButton>
#include <QMouseEvent>
#include "Init/ConstData.h"

class ChessButton : public QPushButton
{
    Q_OBJECT
private:
    int chessNumberInt;
    ChessType chessType;
    ChessType userClickType;

    QString emptyPixString;
    QString whitePixString;
    QString blackPixString;

    bool allowUserClickBool;

    void setChessPix();
public:
    ChessButton(int n ,
                ChessType userClick = BlackChess,
                const QString& emptyPix = EMPTY_PICTURE,
                const QString& whitePix = WHITE_PICTURE,
                const QString& blackPix = BLACK_PICTURE,
                QWidget* parent = nullptr);
    void setChessType(ChessType tp);
    ChessType getChessType()const;
    void resetButtonPix(const QString& emptyPix , const QString& whitePix , const QString& blackPix);
    void setAllowClick(bool fg);
    bool buttonStatu()const;
    void setUserClickType(ChessType userType);
protected:
    void mousePressEvent(QMouseEvent* event);
signals:
    void userHadClicked(int nu);
};

#endif // CHESSBUTTON_H



























```

```c++
#include "ChessButton.h"

ChessButton::ChessButton(int nu,
                         ChessType userClick ,
                         const QString& emptyPix,
                         const QString& whitePix,
                         const QString& blackPix,
                         QWidget* parent):
    QPushButton (parent),
    chessNumberInt(nu),
    chessType (EmptyChess),
    userClickType (userClick),
    emptyPixString(emptyPix),
    whitePixString(whitePix),
    blackPixString(blackPix),
    allowUserClickBool(true){
    setDefault(false);
}

void ChessButton::setChessPix(){
    QPixmap pix;
    if(chessType == EmptyChess)
        pix.load(emptyPixString);
    if(chessType == BlackChess)
        pix.load(blackPixString);
    if(chessType == WhiteChess)
        pix.load(whitePixString);
    int x = size().width();
    int y = size().height();
    setIcon(pix.scaled(QSize(x-2,y-2)));
    setIconSize(QSize(x-2,y-2));
}

void ChessButton::setChessType(ChessType tp){
    chessType = tp;
    setChessPix();
}

ChessType ChessButton::getChessType()const{
    return chessType;
}

void ChessButton::resetButtonPix(const QString& emptyPix , const QString& whitePix , const QString& blackPix){
    emptyPixString = emptyPix.isEmpty() ? emptyPixString : emptyPix;
    whitePixString = whitePix.isEmpty() ? whitePixString : whitePix;
    blackPixString = blackPix.isEmpty() ? blackPixString : blackPix;
    setChessPix();
}

void ChessButton::setAllowClick(bool fg){
    allowUserClickBool = fg;
}

bool ChessButton::buttonStatu()const{
    return allowUserClickBool;
}

void ChessButton::setUserClickType(ChessType userType){
    userClickType = userType;
}

void ChessButton::mousePressEvent(QMouseEvent* event){
    if(chessType == BlackChess || chessType == WhiteChess || !allowUserClickBool)
        return QPushButton::mousePressEvent(event);

    if(event->button() == Qt::LeftButton){
        chessType = userClickType;
        setChessPix();
        emit userHadClicked(chessNumberInt);
    }
    QPushButton::mousePressEvent(event);
}





































```

```c++
#ifndef CHESSMAPWIDGET_H
#define CHESSMAPWIDGET_H

#include "AI/AIThread.h"
#include "ChessButton.h"
#include "ChoosePlayerDialog.h"
#include "GameReplayDialog.h"

class ChessButton;
class ChoosePlayerDialog;
class GameReplayDialog;

class ChessMapWidget : public QWidget
{
    Q_OBJECT
private:
    bool playingBool;

    ChoosePlayerDialog* choosePlayer_Dialog;

    GameReplayDialog* gameReplay_Dialog;

    QPushButton* whitePlayerName_PushButton;
    QPushButton* whitePlayerPix_PushButton;
    QPushButton* blackPlayerName_PushButton;
    QPushButton* blackPlayerPix_PushButton;

    QLabel* vsLogo_Label;

    QPushButton* begin_PushButton;
    QPushButton* log_PushButton;
    QPushButton* recording_PushButton;
    QPushButton* setting_PushButton;

    QString whitePlayerNameString;
    QString blackPlayerNameString;

    ChessType currentType;
    QVector<ChessButton*> allChessVector;

    AIThread* ai_Thread;

    QVector<int> chessStepVector;

    int chessJudgment();
    void fight();
    void victory();
    QString getUserPicture(const QString& userName , ChessType tp)const;
    void saveGameData(const QString& nm = QString());
public:
    explicit ChessMapWidget(QWidget *parent = nullptr);
private slots:
    void userClickChess(int number);
    void aiClickChess(int indexs);
    void chooseWhitePlayer();
    void chooseBlackPlayer();
    void setPlayer(const QString& player , ChessType tp);
    void beginGame();
    void initMap();
    void chooseReplay();
};

#endif // CHESSMAPWIDGET_H








































```

```c++
#include <QGridLayout>
#include <QMessageBox>
#include <QDateTime>
#include <QSqlQuery>
#include <QStringRef>
#include "ChessMapWidget.h"
#include "Init/InitBuilder.h"
#include "Init/ConstData.h"

class InitBuilder;

ChessMapWidget::ChessMapWidget(QWidget *parent) :
    QWidget(parent),
    playingBool(false),
    currentType(ChessType::BlackChess){
    choosePlayer_Dialog = nullptr;
    gameReplay_Dialog = nullptr;

    whitePlayerNameString = DEFAULT_AI_NAME;
    blackPlayerNameString = HUMAN_PLAYER_NAME;

    whitePlayerName_PushButton = new QPushButton(whitePlayerNameString);
    whitePlayerPix_PushButton = new QPushButton();
    blackPlayerName_PushButton = new QPushButton(blackPlayerNameString);
    blackPlayerPix_PushButton = new QPushButton();

    vsLogo_Label = new QLabel("<h1><font color=red>VS</font></h1>");

    begin_PushButton = new QPushButton(tr("开始"));
    log_PushButton = new QPushButton(tr("Log"));
    recording_PushButton = new QPushButton(tr("录像"));
    setting_PushButton = new QPushButton(tr("设置"));

    whitePlayerName_PushButton->setFixedSize(LOG_SIZE*3,LOG_SIZE);
    whitePlayerPix_PushButton->setFixedSize(LOG_SIZE,LOG_SIZE);
    blackPlayerName_PushButton->setFixedSize(LOG_SIZE*3,LOG_SIZE);
    blackPlayerPix_PushButton->setFixedSize(LOG_SIZE,LOG_SIZE);

    QHBoxLayout* vs_Layout = new QHBoxLayout;
    vs_Layout->addStretch();
    vs_Layout->addWidget(blackPlayerPix_PushButton);
    vs_Layout->addWidget(blackPlayerName_PushButton);
    vs_Layout->addWidget(vsLogo_Label);
    vs_Layout->addWidget(whitePlayerName_PushButton);
    vs_Layout->addWidget(whitePlayerPix_PushButton);
    vs_Layout->addStretch();

    QGridLayout* chess_Layout = new QGridLayout;
    for(int rows = 0 ; rows < CHESS_COUNT ; ++rows){
        for(int cols = 0 ; cols < CHESS_COUNT ; ++cols){
            allChessVector.append(new ChessButton(rows*CHESS_COUNT+cols));
            allChessVector.last()->setFixedSize(CHESS_SIZE,CHESS_SIZE);
            allChessVector.last()->setChessType(ChessType::EmptyChess);
            allChessVector.last()->setStyleSheet("border: 1px solid black;");
            allChessVector.last()->setUserClickType(ChessType::BlackChess);
            connect(allChessVector.last(),SIGNAL(userHadClicked(int)),this,SLOT(userClickChess(int)));
            chess_Layout->addWidget(allChessVector.last(),rows,cols);
        }
    }
    chess_Layout->setSpacing(0);
    QVBoxLayout* chessMap_Layout = new QVBoxLayout;
    chessMap_Layout->addLayout(vs_Layout);
    chessMap_Layout->addLayout(chess_Layout);

    QVBoxLayout* ctrl_Layout = new QVBoxLayout;
    ctrl_Layout->addStretch();
    ctrl_Layout->addWidget(begin_PushButton);
    ctrl_Layout->addWidget(log_PushButton);
    ctrl_Layout->addWidget(recording_PushButton);
    ctrl_Layout->addWidget(setting_PushButton);
    ctrl_Layout->addStretch();

    QHBoxLayout* main_Layout = new QHBoxLayout;
    main_Layout->addLayout(chessMap_Layout);
    main_Layout->addLayout(ctrl_Layout);
    setLayout(main_Layout);
    main_Layout->setSizeConstraint(QLayout::SetFixedSize);

    connect(whitePlayerName_PushButton,SIGNAL(clicked()),this,SLOT(chooseWhitePlayer()));
    connect(blackPlayerName_PushButton,SIGNAL(clicked()),this,SLOT(chooseBlackPlayer()));
    connect(begin_PushButton,SIGNAL(clicked()),this,SLOT(beginGame()));
    connect(recording_PushButton,SIGNAL(clicked()),this,SLOT(chooseReplay()));

    whitePlayerName_PushButton->setStyleSheet("font-size:20px");
    blackPlayerName_PushButton->setStyleSheet("background-color:black;color:white;font-size:20px");

    initMap();
}

int ChessMapWidget::chessJudgment(){
    int maxLen = 0;
    for(int rows = 0 ; rows < CHESS_COUNT ; ++rows){
        for(int cols = 0 ; cols < CHESS_COUNT ; ++cols){
            ChessType tp = allChessVector.at(rows * CHESS_COUNT + cols)->getChessType();
            if(tp == EmptyChess)
                continue;

            int lens = 1;
            for(int i = cols + 1 ; i < CHESS_COUNT ; ++i){    //水平方向
                if(allChessVector.at(rows * CHESS_COUNT + i)->getChessType() == tp)
                    ++lens;
                else
                    break;
            }
            for(int i = cols - 1 ; i >= 0 ; --i){
                if(allChessVector.at(rows * CHESS_COUNT + i)->getChessType() == tp)
                    ++lens;
                else
                    break;
            }
            if(lens > maxLen)
                maxLen = lens;

            lens = 1;
            for(int i = rows + 1 ; i < CHESS_COUNT ; ++i){   //垂直方向
                if(allChessVector.at(i * CHESS_COUNT + cols)->getChessType() == tp)
                    ++lens;
                else
                    break;
            }
            for(int i = rows - 1 ; i >= 0 ; --i){
                if(allChessVector.at(i * CHESS_COUNT + cols)->getChessType() == tp)
                    ++lens;
                else
                    break;
            }
            if(lens > maxLen)
                maxLen = lens;

            lens = 1;
            for(int c = cols + 1,r = rows + 1 ; (c < CHESS_COUNT)&&(r < CHESS_COUNT); ++c,++r){   //左上<-->右下方向
                if(allChessVector.at(r * CHESS_COUNT + c)->getChessType() == tp)
                    ++lens;
                else
                    break;
            }
            for(int c = cols - 1,r = rows - 1 ; (c >= 0)&&(r >= 0); --c,--r){
                if(allChessVector.at(r * CHESS_COUNT + c)->getChessType() == tp)
                    ++lens;
                else
                    break;
            }
            if(lens > maxLen)
                maxLen = lens;

            lens = 1;
            for(int c = cols + 1,r = rows - 1 ; (c < CHESS_COUNT)&&(r >= 0); ++c,--r){   //左下<-->右上方向
                if(allChessVector.at(r * CHESS_COUNT + c)->getChessType() == tp)
                    ++lens;
                else
                    break;
            }
            for(int c = cols - 1,r = rows + 1 ; (c >= 0)&&(r < CHESS_COUNT); --c,++r){
                if(allChessVector.at(r * CHESS_COUNT + c)->getChessType() == tp)
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

void ChessMapWidget::fight(){
    if(currentType == ChessType::BlackChess){
        if(blackPlayerNameString == HUMAN_PLAYER_NAME){
            for(auto A : allChessVector)
                A->setAllowClick(true);
            return;
        }
        QList<ChessType> types;
        for(auto A : allChessVector)
            types.append(A->getChessType());
        ai_Thread = new AIThread(this);
        connect(ai_Thread,SIGNAL(aiClick(int)),this,SLOT(aiClickChess(int)));
        ai_Thread->initAI(currentType,types);
        ai_Thread->setAIName(blackPlayerNameString);
        ai_Thread->start();
    }
    if(currentType == ChessType::WhiteChess){
        if(whitePlayerNameString == HUMAN_PLAYER_NAME){
            for(auto A : allChessVector)
                A->setAllowClick(true);
            return;
        }
        QList<ChessType> types;
        for(auto A : allChessVector)
            types.append(A->getChessType());
        ai_Thread = new AIThread(this);
        connect(ai_Thread,SIGNAL(aiClick(int)),this,SLOT(aiClickChess(int)));
        ai_Thread->initAI(currentType,types);
        ai_Thread->setAIName(whitePlayerNameString);
        ai_Thread->start();
    }
}

void ChessMapWidget::victory(){
    QString vn = currentType == ChessType::BlackChess ? blackPlayerNameString : whitePlayerNameString;
    QMessageBox msgBox;
    msgBox.setWindowTitle("VECTORY!!");
    msgBox.setText(vn+" Is Winner!!");
    msgBox.setStandardButtons(QMessageBox::Ok);
    QString pixPath = getUserPicture(vn,currentType);
    msgBox.setIconPixmap(QPixmap(pixPath).scaled(LOG_SIZE,LOG_SIZE));
    msgBox.exec();
}

QString ChessMapWidget::getUserPicture(const QString& userName , ChessType tp)const{
    QString pixPath = InitBuilder::getInstance().getAIPictureDir();
    pixPath += "/";
    pixPath += userName;
    pixPath += ".png";
    if(userName == DEFAULT_AI_NAME)
        return DEFAULT_AI_PICTURE;
    QFile fs(pixPath);
    if(fs.exists())
        return pixPath;
    if(tp == ChessType::BlackChess)
        return BLACK_PICTURE;
    if(tp == ChessType::WhiteChess)
        return WHITE_PICTURE;
    return QString();
}

void ChessMapWidget::saveGameData(const QString& nm){
    QString gameDate = QDateTime::currentDateTime().toString(DATE_FORMAT);
    QString step;

    for(auto A : chessStepVector)
        step.append(QString::number(999-A));   //Make sure that every step is a three digit number;

    QSqlQuery sqlQuery;
    QString sqlStr = "INSERT INTO ChessData (Name, Step, BlackChess, WhiteChess,Date) VALUES('NAME','STEP', 'BLACKCHESS', 'WHITECHESS','DATE');";
    sqlStr.replace("NAME",gameDate);
    sqlStr.replace("STEP",step);
    sqlStr.replace("BLACKCHESS",blackPlayerNameString);
    sqlStr.replace("WHITECHESS",whitePlayerNameString);
    sqlStr.replace("DATE",nm.isEmpty() ? gameDate : nm);
    sqlQuery.exec(sqlStr);
}

void ChessMapWidget::userClickChess(int number){
    for(auto A : allChessVector)
        A->setAllowClick(false);
    chessStepVector.append(number);
    int cs = chessJudgment();
    if(cs >= VICTORY_CONDITION){
        victory();
        return;
    }
    currentType = currentType == ChessType::BlackChess ? ChessType::WhiteChess : ChessType::BlackChess;
    fight();
}

void ChessMapWidget::aiClickChess(int indexs){
    chessStepVector.append(indexs);
    if(indexs == -1 || allChessVector.at(indexs)->getChessType() != ChessType::EmptyChess){
        victory();
        return;
    }

    allChessVector.at(indexs)->setChessType(currentType);
    int cs = chessJudgment();
    if(cs >= VICTORY_CONDITION){
        victory();
        return;
    }
    currentType = currentType == ChessType::BlackChess ? ChessType::WhiteChess : ChessType::BlackChess;
    fight();
}

void ChessMapWidget::chooseWhitePlayer(){
    if(choosePlayer_Dialog == nullptr){
        choosePlayer_Dialog = new ChoosePlayerDialog(this);
        connect(choosePlayer_Dialog,SIGNAL(selectedPlayer(const QString&,ChessType)),this,SLOT(setPlayer(const QString&,ChessType)));
    }
    choosePlayer_Dialog->setCurrentPlayer(whitePlayerNameString,ChessType::WhiteChess);
    choosePlayer_Dialog->exec();
}

void ChessMapWidget::chooseBlackPlayer(){
    if(choosePlayer_Dialog == nullptr){
        choosePlayer_Dialog = new ChoosePlayerDialog(this);
        connect(choosePlayer_Dialog,SIGNAL(selectedPlayer(const QString&,ChessType)),this,SLOT(setPlayer(const QString&,ChessType)));
    }
    choosePlayer_Dialog->setCurrentPlayer(blackPlayerNameString);
    choosePlayer_Dialog->exec();
}

void ChessMapWidget::setPlayer(const QString& player , ChessType tp){
    QString imgPath = getUserPicture(player,tp);
    QPixmap p(imgPath);

    if(tp == ChessType::BlackChess){
        blackPlayerName_PushButton->setText(player);
        blackPlayerNameString = player;
        blackPlayerPix_PushButton->setIcon(QIcon(p.scaled(blackPlayerPix_PushButton->size())));
        blackPlayerPix_PushButton->setIconSize(blackPlayerPix_PushButton->size());
        for(auto A : allChessVector)
            A->resetButtonPix(QString(),QString(),imgPath);
    }else if(tp == ChessType::WhiteChess){
        whitePlayerName_PushButton->setText(player);
        whitePlayerNameString = player;
        whitePlayerPix_PushButton->setIcon(QIcon(p.scaled(whitePlayerPix_PushButton->size())));
        whitePlayerPix_PushButton->setIconSize(whitePlayerPix_PushButton->size());
        for(auto A : allChessVector)
            A->resetButtonPix(QString(),imgPath,QString());
    }
    if(player == HUMAN_PLAYER_NAME){
        for(auto A : allChessVector)
            A->setUserClickType(tp);
    }
}

void ChessMapWidget::beginGame(){
    bool hasHum = blackPlayerNameString == HUMAN_PLAYER_NAME || whitePlayerNameString == HUMAN_PLAYER_NAME;
    bool samePlayer = blackPlayerNameString == whitePlayerNameString;
    for(auto A : allChessVector)
        A->setAllowClick(hasHum && !samePlayer);

    if(samePlayer){
        QMessageBox msgBox;
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setText(tr("两个棋子的名字不能相同"));
        msgBox.setWindowTitle(tr("对手相同"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }
    if(playingBool){
        begin_PushButton->setText(tr("开始"));
        initMap();
        playingBool = false;
        blackPlayerName_PushButton->setEnabled(true);
        whitePlayerName_PushButton->setEnabled(true);
    }else{
        begin_PushButton->setText(tr("结束"));
        playingBool = true;
        fight();
        blackPlayerName_PushButton->setEnabled(false);
        whitePlayerName_PushButton->setEnabled(false);
    }
}

void ChessMapWidget::initMap(){
    if(playingBool){
        QMessageBox msgBox;
        msgBox.setText(tr("结束游戏？"));
        msgBox.setInformativeText(tr("当前游戏正在进行中，是否立刻结束？"));
        msgBox.setStandardButtons(QMessageBox::Save|QMessageBox::SaveAll|QMessageBox::Cancel);
        msgBox.button(QMessageBox::Save)->setText(tr(" 结束并保存本次对局 "));
        msgBox.button(QMessageBox::SaveAll)->setText(tr(" 结束不保存 "));
        msgBox.button(QMessageBox::Cancel)->setText(tr(" 退出 "));
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int btn = msgBox.exec();
        switch (btn) {
            case QMessageBox::Save:{
                playingBool = false;
                begin_PushButton->setEnabled(true);
                for(auto A : allChessVector){
                    A->setChessType(ChessType::EmptyChess);
                    A->setAllowClick(true);
                }
                saveGameData();
                break;
            }
            case QMessageBox::SaveAll:{
                playingBool = false;
                begin_PushButton->setEnabled(true);
                for(auto A : allChessVector){
                    A->setChessType(ChessType::EmptyChess);
                    A->setAllowClick(true);
                }
                chessStepVector.clear();
                break;
            }
            case QMessageBox::Cancel:break;
        }
    }
    chessStepVector.clear();

    currentType = ChessType::BlackChess;
    whitePlayerNameString = whitePlayerName_PushButton->text();
    blackPlayerNameString = blackPlayerName_PushButton->text();

    QString blackImage = getUserPicture(blackPlayerNameString,ChessType::BlackChess);
    QString whiteImage = getUserPicture(whitePlayerNameString,ChessType::WhiteChess);

    whitePlayerPix_PushButton->setIcon(QIcon(QPixmap(whiteImage).scaled(whitePlayerPix_PushButton->size())));
    whitePlayerPix_PushButton->setIconSize(whitePlayerPix_PushButton->size());
    blackPlayerPix_PushButton->setIcon(QIcon(QPixmap(blackImage).scaled(blackPlayerPix_PushButton->size())));
    blackPlayerPix_PushButton->setIconSize(blackPlayerPix_PushButton->size());

    for(auto A : allChessVector){
        A->setChessType(ChessType::EmptyChess);
        A->resetButtonPix(QString(),whiteImage,blackImage);
        A->setAllowClick(false);
    }
}

void ChessMapWidget::chooseReplay(){
    if(gameReplay_Dialog == nullptr){
        gameReplay_Dialog = new GameReplayDialog(this);
        gameReplay_Dialog->exec();
    }
    QString bName,wName,stepData,playNm;
    QSqlQuery sqlQuery;
    QString sqlStr = "SELECT Name , Step, BlackChess , WhiteChess FROM ChessData;";
    sqlQuery.exec(sqlStr);
    while(sqlQuery.next()){
        playNm = sqlQuery.value("Name").toString();
        stepData = sqlQuery.value("Step").toString();
        bName = sqlQuery.value("BlackChess").toString();
        wName = sqlQuery.value("WhiteChess").toString();
    }
    QVector<int> steps;
    int cs = stepData.length() / 3;
    for(int i = 0 ; i < cs ; ++i)
        steps.append(999-QStringRef(&stepData,i*3,3).toInt());
}

































```

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

```

```c++
#include "ChessRobert.h"
#include "Init/InitBuilder.h"

class InitBuilder;

ChessRobert::ChessRobert(QWidget *parent)
    : QMainWindow(parent)
{
    InitBuilder::getInstance().createFile();
    setWindowIcon(QIcon(EXE_ICO));
    chessMap_Widget = new ChessMapWidget;
    setCentralWidget(chessMap_Widget);
}

ChessRobert::~ChessRobert()
{

}

```

```c++
#ifndef CHOOSEPLAYERDIALOG_H
#define CHOOSEPLAYERDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QCloseEvent>
#include "Init/ConstData.h"

class ChoosePlayerDialog : public QDialog
{
    Q_OBJECT
private:
    ChessType playerType;

    QLabel* warningInfo_Label;
    QLabel* playerName_Label;

    QListWidget* playerName_ListWidget;

    QPushButton* confirm_PushButton;
    QPushButton* cancel_PushButton;
public:
    ChoosePlayerDialog(QWidget* parent = nullptr);
    void setCurrentPlayer(const QString& currentPlayer , ChessType tp = ChessType::BlackChess);
protected:
    void closeEvent(QCloseEvent* event);
signals:
    void selectedPlayer(const QString& currentPlayer , ChessType tp);
private slots:
    void choosePlayer();
    void changePlayer(QListWidgetItem* item);
    void choosePlayer(QListWidgetItem* item);
};

#endif // CHOOSEPLAYERDIALOG_H









































```

```c++
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "ChoosePlayerDialog.h"
#include "Init/InitBuilder.h"

class InitBuilder;

ChoosePlayerDialog::ChoosePlayerDialog(QWidget* parent):QDialog (parent){
    warningInfo_Label = new QLabel();
    playerName_Label = new QLabel();

    playerName_ListWidget = new QListWidget();

    confirm_PushButton = new QPushButton(tr("确定"));
    cancel_PushButton = new QPushButton(tr("取消"));

    playerName_Label->setFixedWidth(LOG_SIZE*3);

    QStringList players;
    players.append(HUMAN_PLAYER_NAME);
    players.append(DEFAULT_AI_NAME);
    players.append(InitBuilder::getInstance().getAIName());
    QString aiImagePath = InitBuilder::getInstance().getAIPictureDir() + "/";
    for(auto A : players){
        QString imgPath = aiImagePath + A + ".png";
        QFile f(imgPath);
        if(f.exists())
            playerName_ListWidget->addItem(new QListWidgetItem(QIcon(imgPath),A));
        else
            playerName_ListWidget->addItem(new QListWidgetItem(QIcon(BLACK_PICTURE),A));
    }

    QHBoxLayout* button_Layout = new QHBoxLayout;
    button_Layout->addStretch();
    button_Layout->addWidget(confirm_PushButton);
    button_Layout->addStretch();
    button_Layout->addWidget(cancel_PushButton);
    button_Layout->addStretch();
    QVBoxLayout* main_Layout = new QVBoxLayout;
    main_Layout->addWidget(playerName_Label);
    main_Layout->addWidget(playerName_ListWidget);
    main_Layout->addLayout(button_Layout);
    main_Layout->addWidget(warningInfo_Label);
    setLayout(main_Layout);
    main_Layout->setSizeConstraint(QLayout::SetFixedSize);

    connect(confirm_PushButton,SIGNAL(clicked()),this,SLOT(choosePlayer()));
    connect(cancel_PushButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(playerName_ListWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(changePlayer(QListWidgetItem*)));
    connect(playerName_ListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(choosePlayer(QListWidgetItem*)));

    warningInfo_Label->setStyleSheet("color:red");
}

void ChoosePlayerDialog::setCurrentPlayer(const QString& currentPlayer , ChessType tp){
    playerType = tp;
    playerName_Label->setText(currentPlayer);
    int cs = playerName_ListWidget->count();
    for(int i = 0 ; i < cs ; ++i){
        QListWidgetItem* item = playerName_ListWidget->item(i);
        QString imgPath = tp == ChessType::BlackChess ? BLACK_PICTURE : WHITE_PICTURE;
        QString itemTxt = item->text();
        if(itemTxt == DEFAULT_AI_NAME)
            imgPath = DEFAULT_AI_PICTURE;
        QFile fs(InitBuilder::getInstance().getAIPictureDir() + "/" + itemTxt + ".png");
        if(fs.exists())
            imgPath = InitBuilder::getInstance().getAIPictureDir() + "/" + itemTxt + ".png";
        item->setIcon(QIcon(imgPath));
    }
}

void ChoosePlayerDialog::closeEvent(QCloseEvent* event){
    event->ignore();
    warningInfo_Label->setText("");
    hide();
}

void ChoosePlayerDialog::choosePlayer(){
    emit selectedPlayer(playerName_Label->text(),playerType);
    close();
}

void ChoosePlayerDialog::changePlayer(QListWidgetItem* item){
    playerName_Label->setText(item->text());
}

void ChoosePlayerDialog::choosePlayer(QListWidgetItem* item){
    emit selectedPlayer(item->text(),playerType);
    close();
}









































```

```c++
#ifndef GAMEREPLAYWIDGET_H
#define GAMEREPLAYWIDGET_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QCloseEvent>

class GameReplayDialog : public QDialog{
    Q_OBJECT
private:
    QLabel* missionName_Label;
    QLabel* playerName_Label;
    QLabel* pageCount_Label;

    QPushButton* search_PushButton;
    QPushButton* pageUp_PushButton;
    QPushButton* PageDown_PushButton;
    QPushButton* confirm_PushButton;

    QLineEdit* missionName_LineEdit;
    QLineEdit* playerName_LineEdit;

    QTableView* mission_TableView;
public:
    explicit GameReplayDialog(QWidget* parent = nullptr);
protected:
    void closeEvent(QCloseEvent* event);

signals:

public slots:
};

#endif // GAMEREPLAYWIDGET_H





























```

```c++
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "GameReplayDialog.h"

GameReplayDialog::GameReplayDialog(QWidget *parent) : QDialog(parent){
    setWindowTitle(tr("录像"));

    missionName_Label = new QLabel(tr("对局名称"));
    playerName_Label = new QLabel(tr("玩家名称"));
    pageCount_Label = new QLabel();

    search_PushButton = new QPushButton(tr("查找"));
    pageUp_PushButton = new QPushButton(tr("上一页"));
    PageDown_PushButton = new QPushButton(tr("下一页"));
    confirm_PushButton = new QPushButton(tr("确定"));

    missionName_LineEdit = new QLineEdit();
    playerName_LineEdit = new QLineEdit();

    mission_TableView = new QTableView;

    missionName_Label->setAlignment(Qt::AlignCenter);
    playerName_Label->setAlignment(Qt::AlignCenter);

    missionName_Label->setFixedWidth(100);
    playerName_Label->setFixedWidth(100);
    missionName_LineEdit->setFixedWidth(200);
    playerName_LineEdit->setFixedWidth(200);

    QHBoxLayout* missionName_Layout = new QHBoxLayout;
    missionName_Layout->addWidget(missionName_Label);
    missionName_Layout->addWidget(missionName_LineEdit);
    QHBoxLayout* playerName_Layout = new QHBoxLayout;
    playerName_Layout->addWidget(playerName_Label);
    playerName_Layout->addWidget(playerName_LineEdit);
    QVBoxLayout* search_Layout = new QVBoxLayout;
    search_Layout->addWidget(search_PushButton,0,Qt::AlignRight);
    search_Layout->addStretch();

    QVBoxLayout* info_Layout = new QVBoxLayout;
    info_Layout->addLayout(missionName_Layout);
    info_Layout->addLayout(playerName_Layout);

    QHBoxLayout* title_Layout = new QHBoxLayout;
    title_Layout->addLayout(info_Layout);
    title_Layout->addLayout(search_Layout);

    QHBoxLayout* button_Layout = new QHBoxLayout;
    button_Layout->addWidget(pageUp_PushButton);
    button_Layout->addWidget(PageDown_PushButton);
    button_Layout->addWidget(pageCount_Label);
    button_Layout->addStretch();
    button_Layout->addWidget(confirm_PushButton);

    QVBoxLayout* main_Layout = new QVBoxLayout;
    main_Layout->addLayout(title_Layout);
    main_Layout->addWidget(mission_TableView);
    main_Layout->addLayout(button_Layout);
    setLayout(main_Layout);
    main_Layout->setSizeConstraint(QLayout::SetFixedSize);
}

void GameReplayDialog::closeEvent(QCloseEvent* event){
    event->ignore();
    hide();
}











































```