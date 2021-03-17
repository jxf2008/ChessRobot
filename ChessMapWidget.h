#ifndef CHESSMAPWIDGET_H
#define CHESSMAPWIDGET_H

#include "AIThread.h"
#include "ChessButton.h"
#include "ChoosePlayerDialog.h"
#include "GameReplayDialog.h"
#include "SaveGameDataDialog.h"

class ChessButton;
class ChoosePlayerDialog;
class GameReplayDialog;
class SaveGameDataDialog;

class ChessMapWidget : public QWidget
{
    Q_OBJECT
private:
    bool playingBool;

    ChoosePlayerDialog* choosePlayer_Dialog;

    GameReplayDialog* gameReplay_Dialog;

    SaveGameDataDialog* saveGameData_Dialog;

    QPushButton* whitePlayerName_PushButton;
    QPushButton* whitePlayerPix_PushButton;
    QPushButton* blackPlayerName_PushButton;
    QPushButton* blackPlayerPix_PushButton;

    QLabel* vsLogo_Label;

    QPushButton* begin_PushButton;
    QPushButton* recording_PushButton;

    QString whitePlayerName;
    QString blackPlayerName;

    ChessType currentType;
    QVector<ChessButton*> allChess;

    AIThread* ai_Thread;

    int chessJudgment();
    void fight();
    void victory();
    QString getUserPicture(const QString& userName , ChessType tp)const;
    void saveGameData();
public:
    explicit ChessMapWidget(QWidget *parent = nullptr);
private slots:
    void userClickChess(int number);
    void aiClickChess(int indexs);
    void chooseWhitePlayer();
    void chooseBlackPlayer();
    void setPlayer(const QString& player , ChessType tp);
    void beginGame();
    void initMap();
    void chooseReplay();
};


#endif // CHESSMAPWIDGET_H
