#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "GameReplayDialog.h"
#include "Init/ConstData.h"

GameReplayDialog::GameReplayDialog(QWidget *parent) : QDialog(parent){
    setWindowTitle(tr("录像"));

    playerName_Label = new QLabel(tr("玩家名称"));
    pageCount_Label = new QLabel();

    search_PushButton = new QPushButton(tr("查找"));
    pageUp_PushButton = new QPushButton(tr("上一页"));
    pageDown_PushButton = new QPushButton(tr("下一页"));
    confirm_PushButton = new QPushButton(tr("确定"));

    index_Label = new QLabel();

    playerName_LineEdit = new QLineEdit();

    mission_TableView = new QTableView;
    missionData_Model = new MissionDataModel();

    playerName_Label->setAlignment(Qt::AlignCenter);
    index_Label->setText("1/1");

    playerName_Label->setFixedWidth(100);
    playerName_LineEdit->setFixedWidth(300);

    mission_TableView->setModel(missionData_Model);
    mission_TableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mission_TableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mission_TableView->setAutoScroll(false);
    mission_TableView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QHBoxLayout* playerName_Layout = new QHBoxLayout;
    playerName_Layout->addWidget(playerName_Label);
    playerName_Layout->addWidget(playerName_LineEdit);
    playerName_Layout->addStretch();
    playerName_Layout->addWidget(search_PushButton);

    QHBoxLayout* button_Layout = new QHBoxLayout;
    button_Layout->addWidget(pageUp_PushButton);
    button_Layout->addWidget(pageDown_PushButton);
    button_Layout->addWidget(pageCount_Label);
    button_Layout->addWidget(index_Label);
    button_Layout->addStretch();
    button_Layout->addWidget(confirm_PushButton);

    QVBoxLayout* main_Layout = new QVBoxLayout;
    main_Layout->addLayout(playerName_Layout);
    main_Layout->addWidget(mission_TableView);
    main_Layout->addLayout(button_Layout);
    setLayout(main_Layout);
    //main_Layout->setSizeConstraint(QLayout::SetFixedSize);

    connect(pageUp_PushButton,SIGNAL(clicked()),this,SLOT(pageUp()));
    connect(pageDown_PushButton,SIGNAL(clicked()),this,SLOT(pageDown()));
    connect(search_PushButton,SIGNAL(clicked()),this,SLOT(findMissionData()));

    missionData_Model->searchData();
    showModelIndex();
}

void GameReplayDialog::showModelIndex(){
    int index = missionData_Model->getCurrentOffset();
    int count = missionData_Model->getDataCount();

    QString gameIndex = QString::number(index+1)+"/";
    gameIndex += QString::number((count%MISSION_COUNT_ONTPAGE == 0) ? (count/MISSION_COUNT_ONTPAGE) : (count/MISSION_COUNT_ONTPAGE+1));
    index_Label->setText(gameIndex);
}

void GameReplayDialog::closeEvent(QCloseEvent* event){
    event->ignore();
    hide();
}

void GameReplayDialog::findMissionData(){
    QString playerName = playerName_LineEdit->text();
    missionData_Model->searchData(playerName);
}

void GameReplayDialog::pageUp(){
    missionData_Model->pageUp();
    showModelIndex();
}

void GameReplayDialog::pageDown(){
    missionData_Model->pageDown();
    showModelIndex();
}























































