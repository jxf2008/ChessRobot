#ifndef CHOOSEPLAYERDIALOG_H
#define CHOOSEPLAYERDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QCloseEvent>
#include "ConstData.h"

class ChoosePlayerDialog : public QDialog
{
    Q_OBJECT
private:
    ChessType playerType;

    QLabel* warningInfo_Label;
    QLabel* playerName_Label;

    QListWidget* playerName_ListWidget;

    QPushButton* confirm_PushButton;
    QPushButton* cancel_PushButton;
public:
    ChoosePlayerDialog(QWidget* parent = nullptr);
    void setCurrentPlayer(const QString& currentPlayer , ChessType tp = ChessType::BlackChess);
protected:
    void closeEvent(QCloseEvent* event);
signals:
    void selectedPlayer(const QString& currentPlayer , ChessType tp);
private slots:
    void choosePlayer();
    void changePlayer(QListWidgetItem* item);
    void choosePlayer(QListWidgetItem* item);
};

#endif // CHOOSEPLAYERDIALOG_H
