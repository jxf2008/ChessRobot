#ifndef GAMEREPLAYWIDGET_H
#define GAMEREPLAYWIDGET_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QLabel>
#include <QCloseEvent>
#include <QDateEdit>
#include <QGroupBox>
#include "Model/MissionDataModel.h"

class MissionDataModel;

class GameReplayDialog : public QDialog{
    Q_OBJECT
private:
    QLabel* playerName_Label;
    QLabel* missionName_Label;
    QLabel* missionDate_Label;
    QLabel* pageCount_Label;

    QPushButton* search_PushButton;
    QPushButton* pageUp_PushButton;
    QPushButton* pageDown_PushButton;
    QPushButton* confirm_PushButton;

    QLabel* index_Label;

    QLineEdit* playerName_LineEdit;
    QLineEdit* missionName_LineEdit;
    QLineEdit* missionDate_LineEdit;

    QTableView* mission_TableView;

    QDateEdit* missionDate_DateEdit;

    QGroupBox* search_GroupBox;

    MissionDataModel* missionData_Model;

    int modelCount = 0;
    int modelIndex = 0;

    void showModelIndex();
    void showModel(const QString& missionNm = QString() , const QString& playerNm = QString() , const QString& d = QString());
public:
    explicit GameReplayDialog(QWidget* parent = nullptr);
protected:
    void closeEvent(QCloseEvent* event);
private slots:
    void findMissionData();
    void pageUp();
    void pageDown();
};

#endif // GAMEREPLAYWIDGET_H

















