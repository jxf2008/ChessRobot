#ifdef MSVC
#pragma  execution_character_set("utf-8")
#endif

#include <QHBoxLayout>
#include <QVBoxLayout>
#include "ChoosePlayerDialog.h"
#include "InitBuilder.h"

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
