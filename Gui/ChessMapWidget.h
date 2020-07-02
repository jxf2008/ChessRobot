#ifndef CHESSMAPWIDGET_H
#define CHESSMAPWIDGET_H

#include <QLabel>
#include "AI/DefaultAIThread.h"
#include "ChessButton.h"
#include "ChoosePlayerDialog.h"

class ChessButton;
class ChoosePlayerDialog;

class ChessMapWidget : public QWidget
{
    Q_OBJECT
private:
    ChoosePlayerDialog* choosePlayer_Dialog;

    QPushButton* whitePlayerName_PushButton;
    QPushButton* whitePlayerPix_PushButton;
    QPushButton* blackPlayerName_PushButton;
    QPushButton* blackPlayerPix_PushButton;

    QLabel* vsLogo_Label;

    QPushButton* begin_QPushButton;
    QPushButton* log_PushButton;
    QPushButton* recording_PushButton;
    QPushButton* setting_PushButton;

    QString whitePlayerNameString;
    QString blackPlayerNameString;

    ChessType userType;
    QList<ChessButton*> allChessList;
    int chessJudgment();
    DefaultAIThread* ai1;
public:
    explicit ChessMapWidget(QWidget *parent = nullptr);
private slots:
    void userClickChess();
    void aiClickChess(int indexs);
    void choosePlayer();
    void setPlayer(QString whitePlayer , QString blackPlayer);
    void beginGame();
};

#endif // CHESSMAPWIDGET_H
