#include "Gui/ChessButton.h"

ChessButton::ChessButton(int nu,
                         ChessType userClick ,
                         const QString& emptyPix,
                         const QString& whitePix,
                         const QString& blackPix,
                         QWidget* parent):
    QPushButton (parent),
    chessNumberInt(nu),
    chessType (EmptyChess),
    userClickType (userClick),
    emptyPixString(emptyPix),
    whitePixString(whitePix),
    blackPixString(blackPix),
    allowUserClickBool(true){
    setDefault(false);
}

void ChessButton::setChessPix(){
    QPixmap pix;
    if(chessType == EmptyChess)
        pix.load(emptyPixString);
    if(chessType == BlackChess)
        pix.load(blackPixString);
    if(chessType == WhiteChess)
        pix.load(whitePixString);
    int x = size().width();
    int y = size().height();
    setIcon(pix.scaled(QSize(x-2,y-2)));
    setIconSize(QSize(x-2,y-2));
}

void ChessButton::setChessType(ChessType tp){
    chessType = tp;
    setChessPix();
}

ChessType ChessButton::getChessType()const{
    return chessType;
}

void ChessButton::resetButtonPix(const QString& emptyPix , const QString& whitePix , const QString& blackPix){
    emptyPixString = emptyPix.isEmpty() ? emptyPixString : emptyPix;
    whitePixString = whitePix.isEmpty() ? whitePixString : whitePix;
    blackPixString = blackPix.isEmpty() ? blackPixString : blackPix;
    setChessPix();
}

void ChessButton::setAllowClick(bool fg){
    allowUserClickBool = fg;
}

bool ChessButton::buttonStatu()const{
    return allowUserClickBool;
}

void ChessButton::setUserClickType(ChessType userType){
    userClickType = userType;
}

void ChessButton::mousePressEvent(QMouseEvent* event){
    if(chessType == BlackChess || chessType == WhiteChess || !allowUserClickBool)
        return QPushButton::mousePressEvent(event);

    if(event->button() == Qt::LeftButton){
        chessType = userClickType;
        setChessPix();
        emit userHadClicked(chessNumberInt);
    }
    QPushButton::mousePressEvent(event);
}

