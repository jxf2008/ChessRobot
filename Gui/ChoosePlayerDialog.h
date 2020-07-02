#ifndef CHOOSEPLAYERDIALOG_H
#define CHOOSEPLAYERDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QCloseEvent>

class ChoosePlayerDialog : public QDialog
{
    Q_OBJECT
private:
    QLabel* warningInfo_Label;
    QLabel* whitePlayerName_Label;
    QLabel* blackPlayerName_Label;

    QListWidget* white_ListWidget;
    QListWidget* black_ListWidget;

    QPushButton* confirm_PushButton;
    QPushButton* cancel_PushButton;
public:
    ChoosePlayerDialog(QWidget* parent = nullptr);
    void setPlayer(const QList<QString> allNames , QString whitePlayer , QString blackPlayer);
protected:
    void closeEvent(QCloseEvent* event);
signals:
    void twoPlayer(QString whitePlayer , QString blackPlayer);
private slots:
    void chooseTwoPlayer();
    void changeWhitePlayer(QListWidgetItem* item);
    void changeBlackPlayer(QListWidgetItem* item);
};

#endif // CHOOSEPLAYERDIALOG_H