#ifdef MSVC
#pragma  execution_character_set("utf-8")
#endif

#include <QGridLayout>
#include <QDateTime>
#include <QSqlQuery>
#include <QStringRef>
#include <QMessageBox>
#include <QDir>
#include <QLineEdit>
#include "ChessMapWidget.h"
#include "InitBuilder.h"
#include "ConstData.h"
#include "ChessMapWidget.h"

class InitBuilder;

ChessMapWidget::ChessMapWidget(QWidget *parent) :
    QWidget(parent),
    playingBool(false),
    currentType(ChessType::BlackChess){
    choosePlayer_Dialog = nullptr;
    gameReplay_Dialog = nullptr;
    saveGameData_Dialog = nullptr;

    whitePlayerName = DEFAULT_AI_NAME;
    blackPlayerName = HUMAN_PLAYER_NAME;

    whitePlayerName_PushButton = new QPushButton(whitePlayerName);
    whitePlayerPix_PushButton = new QPushButton();
    blackPlayerName_PushButton = new QPushButton(blackPlayerName);
    blackPlayerPix_PushButton = new QPushButton();

    vsLogo_Label = new QLabel("<h1><font color=red>VS</font></h1>");


    begin_PushButton = new QPushButton(tr("开始"));
    recording_PushButton = new QPushButton(tr("录像"));

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
            allChess.append(new ChessButton(rows*CHESS_COUNT+cols));
            allChess.last()->setFixedSize(CHESS_SIZE,CHESS_SIZE);
            allChess.last()->setChessType(ChessType::EmptyChess);
            allChess.last()->setStyleSheet("border: 1px solid black;");
            allChess.last()->setUserClickType(ChessType::BlackChess);
            connect(allChess.last(),SIGNAL(userHadClicked(int)),this,SLOT(userClickChess(int)));
            chess_Layout->addWidget(allChess.last(),rows,cols);
        }
    }
    chess_Layout->setSpacing(0);
    QVBoxLayout* chessMap_Layout = new QVBoxLayout;
    chessMap_Layout->addLayout(vs_Layout);
    chessMap_Layout->addLayout(chess_Layout);

    QVBoxLayout* ctrl_Layout = new QVBoxLayout;
    ctrl_Layout->addStretch();
    ctrl_Layout->addWidget(begin_PushButton);
    ctrl_Layout->addWidget(recording_PushButton);
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
            ChessType tp = allChess.at(rows * CHESS_COUNT + cols)->getChessType();
            if(tp == EmptyChess)
                continue;

            int lens = 1;
            for(int i = cols + 1 ; i < CHESS_COUNT ; ++i){    //水平方向
                if(allChess.at(rows * CHESS_COUNT + i)->getChessType() == tp)
                    ++lens;
                else
                    break;
            }
            for(int i = cols - 1 ; i >= 0 ; --i){
                if(allChess.at(rows * CHESS_COUNT + i)->getChessType() == tp)
                    ++lens;
                else
                    break;
            }
            if(lens > maxLen)
                maxLen = lens;

            lens = 1;
            for(int i = rows + 1 ; i < CHESS_COUNT ; ++i){   //垂直方向
                if(allChess.at(i * CHESS_COUNT + cols)->getChessType() == tp)
                    ++lens;
                else
                    break;
            }
            for(int i = rows - 1 ; i >= 0 ; --i){
                if(allChess.at(i * CHESS_COUNT + cols)->getChessType() == tp)
                    ++lens;
                else
                    break;
            }
            if(lens > maxLen)
                maxLen = lens;

            lens = 1;
            for(int c = cols + 1,r = rows + 1 ; (c < CHESS_COUNT)&&(r < CHESS_COUNT); ++c,++r){   //左上<-->右下方向
                if(allChess.at(r * CHESS_COUNT + c)->getChessType() == tp)
                    ++lens;
                else
                    break;
            }
            for(int c = cols - 1,r = rows - 1 ; (c >= 0)&&(r >= 0); --c,--r){
                if(allChess.at(r * CHESS_COUNT + c)->getChessType() == tp)
                    ++lens;
                else
                    break;
            }
            if(lens > maxLen)
                maxLen = lens;

            lens = 1;
            for(int c = cols + 1,r = rows - 1 ; (c < CHESS_COUNT)&&(r >= 0); ++c,--r){   //左下<-->右上方向
                if(allChess.at(r * CHESS_COUNT + c)->getChessType() == tp)
                    ++lens;
                else
                    break;
            }
            for(int c = cols - 1,r = rows + 1 ; (c >= 0)&&(r < CHESS_COUNT); --c,++r){
                if(allChess.at(r * CHESS_COUNT + c)->getChessType() == tp)
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
        if(blackPlayerName == HUMAN_PLAYER_NAME){
            for(auto A : allChess)
                A->setAllowClick(true);
            return;
        }
        QList<ChessType> types;
        for(auto A : allChess)
            types.append(A->getChessType());
        ai_Thread = new AIThread(this);
        connect(ai_Thread,SIGNAL(aiClick(int)),this,SLOT(aiClickChess(int)));
        ai_Thread->initAI(currentType,types);
        ai_Thread->setAIName(blackPlayerName);
        ai_Thread->start();
    }
    if(currentType == ChessType::WhiteChess){
        if(whitePlayerName == HUMAN_PLAYER_NAME){
            for(auto A : allChess)
                A->setAllowClick(true);
            return;
        }
        QList<ChessType> types;
        for(auto A : allChess)
            types.append(A->getChessType());
        ai_Thread = new AIThread(this);
        connect(ai_Thread,SIGNAL(aiClick(int)),this,SLOT(aiClickChess(int)));
        ai_Thread->initAI(currentType,types);
        ai_Thread->setAIName(whitePlayerName);
        ai_Thread->start();
    }
}

void ChessMapWidget::victory(){
    QString vn = currentType == ChessType::BlackChess ? blackPlayerName : whitePlayerName;
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

void ChessMapWidget::saveGameData(){
    QString gameDate = QDateTime::currentDateTime().toString(DATE_FORMAT);

    QSqlQuery sqlQuery;
    sqlQuery.exec("BEGIN;");
    for(int i = 0 ; i < allChess.size() ; ++i){
        if (allChess.at(i)->getChessType() == ChessType::EmptyChess)
            continue;
        QString sqlStr = "INSERT INTO ChessData (MissionName,ChessIndex,ChessStep,ChessType) VALUES('DATE','INDEX','STEP', 'CHESSTYPE');";
        sqlStr.replace("DATE",gameDate);
        sqlStr.replace("INDEX",QString::number(allChess.at(i)->getChessStep()));
        sqlStr.replace("STEP", QString::number(i));
        sqlStr.replace("CHESSTYPE",QString::number(allChess.at(i)->getChessType()));
        sqlQuery.exec(sqlStr);
    }
    sqlQuery.exec("COMMIT;");
    QString sqlStr = "INSERT INTO PlayerData(MissionName,BlackPlayerName, WhitePlayerName,Winner,Date) VALUES('MISSIONNAME','BLACKNAME', 'WHITENAME','WINNER','DATE');";
    sqlStr.replace("MISSIONNAME", gameDate);
    sqlStr.replace("BLACKNAME",blackPlayerName);
    sqlStr.replace("WHITENAME",whitePlayerName);
    sqlStr.replace("WINNER", currentType == ChessType::BlackChess ? blackPlayerName : whitePlayerName);
    sqlStr.replace("DATE", gameDate);
    sqlQuery.exec(sqlStr);
}

void ChessMapWidget::userClickChess(int number){
    for(auto A : allChess)
        A->setAllowClick(false);
    int cs = chessJudgment();
    if(cs >= VICTORY_CONDITION){
        victory();
        return;
    }
    currentType = currentType == ChessType::BlackChess ? ChessType::WhiteChess : ChessType::BlackChess;
    fight();
}

void ChessMapWidget::aiClickChess(int indexs){
    if(indexs == -1 || allChess.at(indexs)->getChessType() != ChessType::EmptyChess){
        victory();
        return;
    }

    allChess.at(indexs)->setChessType(currentType);
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
    choosePlayer_Dialog->setCurrentPlayer(whitePlayerName,ChessType::WhiteChess);
    choosePlayer_Dialog->exec();
}

void ChessMapWidget::chooseBlackPlayer(){
    if(choosePlayer_Dialog == nullptr){
        choosePlayer_Dialog = new ChoosePlayerDialog(this);
        connect(choosePlayer_Dialog,SIGNAL(selectedPlayer(const QString&,ChessType)),this,SLOT(setPlayer(const QString&,ChessType)));
    }
    choosePlayer_Dialog->setCurrentPlayer(blackPlayerName);
    choosePlayer_Dialog->exec();
}

void ChessMapWidget::setPlayer(const QString& player , ChessType tp){
    QString imgPath = getUserPicture(player,tp);
    QPixmap p(imgPath);

    if(tp == ChessType::BlackChess){
        blackPlayerName_PushButton->setText(player);
        blackPlayerName = player;
        blackPlayerPix_PushButton->setIcon(QIcon(p.scaled(blackPlayerPix_PushButton->size())));
        blackPlayerPix_PushButton->setIconSize(blackPlayerPix_PushButton->size());
        for(auto A : allChess)
            A->resetButtonPix(QString(),QString(),imgPath);
    }else if(tp == ChessType::WhiteChess){
        whitePlayerName_PushButton->setText(player);
        whitePlayerName = player;
        whitePlayerPix_PushButton->setIcon(QIcon(p.scaled(whitePlayerPix_PushButton->size())));
        whitePlayerPix_PushButton->setIconSize(whitePlayerPix_PushButton->size());
        for(auto A : allChess)
            A->resetButtonPix(QString(),imgPath,QString());
    }
    if(player == HUMAN_PLAYER_NAME){
        for(auto A : allChess)
            A->setUserClickType(tp);
    }
}

void ChessMapWidget::beginGame(){
    bool hasHum = blackPlayerName == HUMAN_PLAYER_NAME || whitePlayerName == HUMAN_PLAYER_NAME;
    bool samePlayer = blackPlayerName == whitePlayerName;
    for(auto A : allChess)
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
        saveGameData();

        playingBool = false;
        begin_PushButton->setEnabled(true);
        for(auto A : allChess){
            A->setChessType(ChessType::EmptyChess);
            A->setAllowClick(true);
        }
    }

    currentType = ChessType::BlackChess;
    whitePlayerName = whitePlayerName_PushButton->text();
    blackPlayerName = blackPlayerName_PushButton->text();

    QString blackImage = getUserPicture(blackPlayerName,ChessType::BlackChess);
    QString whiteImage = getUserPicture(whitePlayerName,ChessType::WhiteChess);

    whitePlayerPix_PushButton->setIcon(QIcon(QPixmap(whiteImage).scaled(whitePlayerPix_PushButton->size())));
    whitePlayerPix_PushButton->setIconSize(whitePlayerPix_PushButton->size());
    blackPlayerPix_PushButton->setIcon(QIcon(QPixmap(blackImage).scaled(blackPlayerPix_PushButton->size())));
    blackPlayerPix_PushButton->setIconSize(blackPlayerPix_PushButton->size());

    for(auto A : allChess){
        A->setChessType(ChessType::EmptyChess);
        A->resetButtonPix(QString(),whiteImage,blackImage);
        A->setAllowClick(false);
    }
}

void ChessMapWidget::chooseReplay(){
    if(gameReplay_Dialog == nullptr)
        gameReplay_Dialog = new GameReplayDialog(this);
    gameReplay_Dialog->exec();
}
