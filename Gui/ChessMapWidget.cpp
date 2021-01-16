#include <QGridLayout>
#include <QMessageBox>
#include <QDateTime>
#include <QSqlQuery>
#include <QStringRef>
#include "ChessMapWidget.h"
#include "Init/InitBuilder.h"
#include "Init/ConstData.h"
#include "Gui/ChessMapWidget.h"

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
