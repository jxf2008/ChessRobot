#include <QGridLayout>
#include "Gui/ChessMapWidget.h"

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

    ai1 = new DefaultAIThread(this);
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
        ai1 = new DefaultAIThread(this);
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
