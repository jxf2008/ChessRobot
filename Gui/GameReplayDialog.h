#ifndef GAMEREPLAYWIDGET_H
#define GAMEREPLAYWIDGET_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QCloseEvent>

class GameReplayDialog : public QDialog{
    Q_OBJECT
private:
    QLabel* missionName_Label;
    QLabel* playerName_Label;
    QLabel* pageCount_Label;

    QPushButton* search_PushButton;
    QPushButton* pageUp_PushButton;
    QPushButton* PageDown_PushButton;
    QPushButton* confirm_PushButton;

    QLineEdit* missionName_LineEdit;
    QLineEdit* playerName_LineEdit;

    QTableView* mission_TableView;
public:
    explicit GameReplayDialog(QWidget* parent = nullptr);
protected:
    void closeEvent(QCloseEvent* event);

signals:

public slots:
};

#endif // GAMEREPLAYWIDGET_H