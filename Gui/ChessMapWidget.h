#ifndef CHESSMAPWIDGET_H
#define CHESSMAPWIDGET_H

#include "AI/AIThread.h"
#include "ChessButton.h"
#include "ChoosePlayerDialog.h"
#include "GameReplayDialog.h"

class ChessButton;
class ChoosePlayerDialog;
class GameReplayDialog;

class ChessMapWidget : public QWidget
{
    Q_OBJECT
private:
    bool playingBool;

    ChoosePlayerDialog* choosePlayer_Dialog;

    GameReplayDialog* gameReplay_Dialog;

    QPushButton* whitePlayerName_PushButton;
    QPushButton* whitePlayerPix_PushButton;
    QPushButton* blackPlayerName_PushButton;
    QPushButton* blackPlayerPix_PushButton;

    QLabel* vsLogo_Label;

    QPushButton* begin_PushButton;
    QPushButton* log_PushButton;
    QPushButton* recording_PushButton;
    QPushButton* setting_PushButton;

    QString whitePlayerNameString;
    QString blackPlayerNameString;

    ChessType currentType;
    QVector<ChessButton*> allChessVector;

    AIThread* ai_Thread;

    QVector<int> chessStepVector;

    int chessJudgment();
    void fight();
    void victory();
    QString getUserPicture(const QString& userName , ChessType tp)const;
    void saveGameData(const QString& nm = QString());
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
