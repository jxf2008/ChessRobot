#ifdef MSVC
#pragma  execution_character_set("utf-8")
#endif

#include <QHBoxLayout>
#include <QVBoxLayout>
#include "SaveGameDataDialog.h"

SaveGameDataDialog::SaveGameDataDialog(QWidget *parent) : QDialog(parent){
    isSave_Label = new QLabel(tr("保存"));
    missionName_Label = new QLabel("任务名称");
    missionNote_Label = new QLabel("任务注释");

    save_RadioButton = new QRadioButton(tr("保存"));
    giveUp_RadioButton = new QRadioButton(tr("放弃保存"));

    missionName_LineEdit = new QLineEdit();
    missionNote_LineEdit = new QLineEdit();

    save_PushButton = new QPushButton(tr("保存"));
    cancel_PushButton = new QPushButton(tr("取消"));

    save_PushButton->setChecked(true);

    isSave_Label->setAlignment(Qt::AlignRight|Qt::AlignHCenter);
    missionName_Label->setAlignment(Qt::AlignRight|Qt::AlignHCenter);
    missionNote_Label->setAlignment(Qt::AlignRight|Qt::AlignHCenter);

    QHBoxLayout* sameType_Layout = new QHBoxLayout();
    sameType_Layout->addWidget(isSave_Label);
    sameType_Layout->addWidget(save_RadioButton);
    sameType_Layout->addWidget(giveUp_RadioButton);

    QHBoxLayout* name_Layout = new QHBoxLayout();
    name_Layout->addWidget(missionName_Label);
    name_Layout->addWidget(missionName_LineEdit);

    QHBoxLayout* note_Layout = new QHBoxLayout();
    note_Layout->addWidget(missionNote_Label);
    note_Layout->addWidget(missionNote_LineEdit);

    QHBoxLayout* button_Layout = new QHBoxLayout();
    button_Layout->addStretch();
    button_Layout->addWidget(save_PushButton);
    button_Layout->addStretch();
    button_Layout->addWidget(cancel_PushButton);
    button_Layout->addStretch();

    QVBoxLayout* main_Layout = new QVBoxLayout();
    main_Layout->addLayout(sameType_Layout);
    main_Layout->addLayout(name_Layout);
    main_Layout->addLayout(note_Layout);
    main_Layout->addLayout(button_Layout);
    main_Layout->setSizeConstraint(QLayout::SetFixedSize);

    connect(save_PushButton,SIGNAL(clicked()),this,SLOT(svmeGameData()));
    connect(cancel_PushButton,SIGNAL(clicked()),this,SLOT(cancelSave()));
}

void SaveGameDataDialog::closeEvent(QCloseEvent* event){
    event->ignore();
    missionName_LineEdit->clear();
    missionNote_LineEdit->clear();
    save_RadioButton->setChecked(true);
}

void SaveGameDataDialog::saveGameData(){
    emit sameGame(save_RadioButton->isChecked(),missionName_LineEdit->text(),missionNote_LineEdit->text());
    close();
}

void SaveGameDataDialog::cancelSave(){
    emit sameGame(false,missionName_LineEdit->text(),missionNote_LineEdit->text());
    close();
}











































