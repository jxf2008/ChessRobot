#ifdef MSVC
#pragma  execution_character_set("utf-8")
#endif

#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "GameReplayDialog.h"
#include "ConstData.h"

GameReplayDialog::GameReplayDialog(QWidget *parent) :QDialog(parent){
    setWindowTitle(tr("录像"));
    playerName_Label = new QLabel(tr("玩家名称"));
    missionDate_Label = new QLabel(tr("任务日期"));
    pageCount_Label = new QLabel();

    search_PushButton = new QPushButton(tr("查找"));
    pageUp_PushButton = new QPushButton(tr("上一页"));
    pageDown_PushButton = new QPushButton(tr("下一页"));
    confirm_PushButton = new QPushButton(tr("确定"));

    index_Label = new QLabel();

    playerName_LineEdit = new QLineEdit();
    missionDate_LineEdit = new QLineEdit();

    missionDate_DateEdit = new QDateEdit();

    search_GroupBox = new QGroupBox();

    mission_TableView = new QTableView;
    missionData_Model = new MissionDataModel();

    missionDate_DateEdit->setCalendarPopup(true);
    missionDate_DateEdit->setDate(QDate::currentDate());
    missionDate_DateEdit->setFixedSize(20, 20);

    playerName_Label->setAlignment(Qt::AlignCenter);
    index_Label->setText("1/1");

    playerName_Label->setFixedWidth(100);
    missionDate_Label->setFixedWidth(100);
    playerName_LineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    missionDate_LineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    playerName_Label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    missionDate_Label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    mission_TableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    mission_TableView->setModel(missionData_Model);
    mission_TableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mission_TableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mission_TableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mission_TableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mission_TableView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QHBoxLayout* playerName_Layout = new QHBoxLayout;
    playerName_Layout->addWidget(playerName_Label);
    playerName_Layout->addWidget(playerName_LineEdit);

    QHBoxLayout* missionDate_Layout = new QHBoxLayout;
    missionDate_Layout->addWidget(missionDate_Label);
    missionDate_Layout->addWidget(missionDate_LineEdit);
    missionDate_Layout->addWidget(missionDate_DateEdit);

    QHBoxLayout* searchButton_Layout = new QHBoxLayout;
    searchButton_Layout->addStretch();
    searchButton_Layout->addWidget(search_PushButton);

    QVBoxLayout* search_Layout = new QVBoxLayout;
    search_Layout->addLayout(playerName_Layout);
    search_Layout->addLayout(missionDate_Layout);
    search_Layout->addLayout(searchButton_Layout);
    search_GroupBox->setLayout(search_Layout);

    QHBoxLayout* button_Layout = new QHBoxLayout;
    button_Layout->addWidget(pageUp_PushButton);
    button_Layout->addWidget(pageDown_PushButton);
    button_Layout->addWidget(pageCount_Label);
    button_Layout->addWidget(index_Label);
    button_Layout->addStretch();
    button_Layout->addWidget(confirm_PushButton);

    QVBoxLayout* main_Layout = new QVBoxLayout;
    main_Layout->addWidget(search_GroupBox);
    main_Layout->addWidget(mission_TableView);
    main_Layout->addLayout(button_Layout);
    setLayout(main_Layout);
    //main_Layout->setSizeConstraint(QLayout::SetFixedSize);

    connect(pageUp_PushButton,SIGNAL(clicked()),this,SLOT(pageUp()));
    connect(pageDown_PushButton,SIGNAL(clicked()),this,SLOT(pageDown()));
    connect(search_PushButton,SIGNAL(clicked()),this,SLOT(findMissionData()));

    missionData_Model->selectData();
    setIndex(missionData_Model->getCurrentPageNumber(),missionData_Model->getDataCount());

    QString styleStr = "QDateEdit::drop-down{border:0;width:20;height:20;image:url(:/images/Calendar.png)} ";
    styleStr += "QDateEdit{border:0px;padding:0px;background - color:white} ";
    styleStr += "QTableView{gridline-color:black} ";
    styleStr += "QHeaderView::section{background-color:rgb(160,205,166)} ";
    styleStr += "QTableView QTableCornerButton::section{background-color:rgb(160,205,166)} ";
    styleStr += "QDialog{background-color:rgb(152,189,192)} ";
    styleStr += "QPushButton{width:120;height:40;font-size:20px}";
    styleStr += "QPushButton{border-radius:5px;border:1px solid rgb(106,166,155);background-color:rgb(142,136,116)} ";
    styleStr += "QPushButton:!enabled{background-color:rgb(191,183,174)} ";
    styleStr += "QPushButton:hover{border:1px solid rgb(46,75,70)} ";
    styleStr += "QPushButton:!hover{border:1px solid rgb(106,166,155)} ";
    setStyleSheet(styleStr);
}

void GameReplayDialog::setIndex(int index , int count){
    int pageCount = count % MISSION_COUNT_ONEPAGE == 0 ? (count / MISSION_COUNT_ONEPAGE) : (count / MISSION_COUNT_ONEPAGE + 1);
    index_Label->setText(QString::number(index) + "/" + QString::number(pageCount));
}

void GameReplayDialog::closeEvent(QCloseEvent* event){
    event->ignore();
    missionData_Model->selectData();
    setIndex(missionData_Model->getCurrentPageNumber(),missionData_Model->getDataCount());
    hide();
}

void GameReplayDialog::findMissionData(){
    QString playerName = playerName_LineEdit->text();
    QString missionDate = missionDate_LineEdit->text();
    missionData_Model->selectData(playerName,missionDate);
}

void GameReplayDialog::pageUp(){
    missionData_Model->pageUp();
    setIndex(missionData_Model->getCurrentPageNumber(),missionData_Model->getDataCount());
}

void GameReplayDialog::pageDown(){
    missionData_Model->pageDown();
    setIndex(missionData_Model->getCurrentPageNumber(),missionData_Model->getDataCount());
}























































