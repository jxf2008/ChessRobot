#include "Gui/ChessButton.h"

ChessButton::ChessButton(ChessType userClick ,
                         const QString& emptyPix,
                         const QString& whitePix,
                         const QString& blackPix,
                         QWidget* parent):
    QPushButton (parent),
    chessType (EmptyChess),
    userClickType (userClick),
    emptyPixString(emptyPix),
    whitePixString(whitePix),
    blackPixString(blackPix),
    allowUserClickBool(true){
    setDefault(false);
    setStyleSheet("border-style: outset; border-width: 2px;border-color: black;");
}

void ChessButton::setChessPix(){
    QPixmap pix;
    if(chessType == EmptyChess)
        pix.load(emptyPixString);
    if(chessType == BlackChess)
        pix.load(blackPixString);
    if(chessType == WhiteChess)
        pix.load(whitePixString);
    setIcon(pix.scaled(size()));
    setIconSize(size());
}

void ChessButton::setChessType(ChessType tp){
    chessType = tp;
    setChessPix();
}

ChessType ChessButton::getChessType()const{
    return chessType;
}

void ChessButton::resetButtonPix(const QString& emptyPix , const QString& whitePix , const QString& blackPix){
    emptyPixString = emptyPix;
    whitePixString = whitePix;
    blackPixString = blackPix;
    setChessPix();
}

void ChessButton::setAllowClick(bool fg){
    allowUserClickBool = fg;
}

bool ChessButton::buttonStatu()const{
    return allowUserClickBool;
}

void ChessButton::mousePressEvent(QMouseEvent* event)
{
    if(chessType == BlackChess || chessType == WhiteChess || !allowUserClickBool)
        return QPushButton::mousePressEvent(event);

    if(event->button() == Qt::LeftButton)
    {
        chessType = userClickType;
        setChessPix();
        emit userHadClicked();
    }
    QPushButton::mousePressEvent(event);
}
