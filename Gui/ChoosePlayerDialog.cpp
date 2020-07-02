#include <QHBoxLayout>
#include <QVBoxLayout>
#include "ChoosePlayerDialog.h"
#include "Init/ConstData.h"

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
