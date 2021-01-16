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