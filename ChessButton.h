#ifndef CHESSBUTTON_H
#define CHESSBUTTON_H

#include <QPushButton>
#include <QMouseEvent>
#include "ConstData.h"

class ChessButton : public QPushButton
{
    Q_OBJECT
private:
    static int stepCount;
    int currentStep = -1;
    int chessStep;
    int chessNumberInt;
    ChessType chessType;
    ChessType userClickType;

    QString emptyPixString;
    QString whitePixString;
    QString blackPixString;

    bool allowUserClickBool;

    void setChessPix();
public:
    ChessButton(int n ,
                ChessType userClick = BlackChess,
                const QString& emptyPix = EMPTY_PICTURE,
                const QString& whitePix = WHITE_PICTURE,
                const QString& blackPix = BLACK_PICTURE,
                QWidget* parent = nullptr);
    void setChessType(ChessType tp);
    ChessType getChessType()const;
    void resetButtonPix(const QString& emptyPix , const QString& whitePix , const QString& blackPix);
    void setAllowClick(bool fg);
    bool buttonStatu()const;
    void setUserClickType(ChessType userType);
    int getChessStep()const { return currentStep; };
protected:
    void mousePressEvent(QMouseEvent* event);
signals:
    void userHadClicked(int nu);
};

#endif // CHESSBUTTON_H
