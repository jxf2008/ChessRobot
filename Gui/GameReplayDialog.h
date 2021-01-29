#ifndef GAMEREPLAYWIDGET_H
#define GAMEREPLAYWIDGET_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QLabel>
#include <QCloseEvent>
#include "Model/MissionDataModel.h"

class MissionDataModel;

class GameReplayDialog : public QDialog{
    Q_OBJECT
private:
    QLabel* playerName_Label;
    QLabel* pageCount_Label;

    QPushButton* search_PushButton;
    QPushButton* pageUp_PushButton;
    QPushButton* pageDown_PushButton;
    QPushButton* confirm_PushButton;

    QLabel* index_Label;

    QLineEdit* playerName_LineEdit;

    QTableView* mission_TableView;

    MissionDataModel* missionData_Model;

    void showModelIndex();
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

















