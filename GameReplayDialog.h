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
#include <QCheckBox>
#include "MissionDataModel.h"

class MissionDataModel;

class GameReplayDialog : public QDialog{
    Q_OBJECT
private:
    QLabel* playerName_Label;
    QLabel* missionDateBegin_Label;
    QLabel* missionDate_Label;
    QLabel* pageCount_Label;

    QPushButton* search_PushButton;
    QPushButton* pageUp_PushButton;
    QPushButton* pageDown_PushButton;
    QPushButton* confirm_PushButton;

    QLabel* index_Label;

    QLineEdit* playerName_LineEdit;
    QLineEdit* missionDate_LineEdit;

    QTableView* mission_TableView;

    QDateEdit* missionDate_DateEdit;

    QGroupBox* search_GroupBox;

    MissionDataModel* missionData_Model;

    void setIndex(int index , int count);
public:
    explicit GameReplayDialog(QWidget* parent = nullptr);
protected:
    void closeEvent(QCloseEvent* event);
private slots:
    void findMissionData();
    void pageUp();
    void pageDown();
    void chooseMissionData(const QDateTime& datetime);
};

#endif // GAMEREPLAYWIDGET_H

















