#ifndef CHESSBUTTON_H
#define CHESSBUTTON_H

#include <QPushButton>
#include <QMouseEvent>
#include "Init/ConstData.h"

class ChessButton : public QPushButton
{
    Q_OBJECT
private:
    ChessType chessType;
    ChessType userClickType;

    QString emptyPixString;
    QString whitePixString;
    QString blackPixString;

    bool allowUserClickBool;

    void setChessPix();
public:
    ChessButton(ChessType userClick = BlackChess,
                const QString& emptyPix = EMPTY_PIX,
                const QString& whitePix = WHITE_PIX,
                const QString& blackPix = BLACK_PIX,
                QWidget* parent = nullptr);
    void setChessType(ChessType tp);
    ChessType getChessType()const;
    void resetButtonPix(const QString& emptyPix , const QString& whitePix , const QString& blackPix);
    void setAllowClick(bool fg);
    bool buttonStatu()const;
protected:
    void mousePressEvent(QMouseEvent* event);
signals:
    void userHadClicked();
};

#endif // CHESSBUTTON_H
