```c++
#ifndef MISSIONDATAMODEL_H_
#define MISSIONDATAMODEL_H_

#include <QAbstractTableModel>

class MissionDataModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    QVector<QString> titleVector;

    QVector<QString> missionNameVector;
    QVector<QString> blackPlayerNameVector;
    QVector<QString> whitePlayerNameVector;
    QVector<QString> dateVector;
public:
    MissionDataModel(QObject* parent = nullptr);
    int rowCount(const QModelIndex& index)const;
    int columnCount(const QModelIndex& index)const;
    QVariant data(const QModelIndex& index , int role)const;
    QVariant headerData(int sec , Qt::Orientation orientation , int role)const;
    void searchData(const QString& missionNm , const QString& playerNm , int offset = 0);
};
#endif

```

```c++
#include <QSqlQuery>
#include <Init/ConstData.h>
#include "Model/MissionDataModel.h"

MissionDataModel::MissionDataModel(QObject* parent):QAbstractTableModel (parent){
    titleVector.append(tr("对局名词"));
    titleVector.append(tr("黑棋"));
    titleVector.append(tr("白棋"));
    titleVector.append(tr("日期"));
}

int MissionDataModel::rowCount(const QModelIndex& index)const{
    Q_UNUSED(index)
    return MISSION_COUNT_ONTPAGE;
}

int MissionDataModel::columnCount(const QModelIndex& index)const{
    Q_UNUSED(index);
    return titleVector.count();
}

QVariant MissionDataModel::data(const QModelIndex& index , int role)const{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole){
        int r = index.row();
        int c = index.column();
        if(c == 0)
            return QVariant(missionNameVector.at(r));
        if(c == 1)
            return QVariant(blackPlayerNameVector.at(r));
        if(c == 2)
            return QVariant(whitePlayerNameVector.at(r));
        if(c == 3)
            return QVariant(dateVector.at(r));
    }
    if(role == Qt::TextAlignmentRole)
        return QVariant(Qt::AlignCenter);

    return QVariant();
}

QVariant MissionDataModel::headerData(int sec , Qt::Orientation orientation , int role)const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(titleVector.at(sec));
    if(orientation == Qt::Vertical && role == Qt::DisplayRole)
        return QVariant(sec+1);
    return QVariant();
}

void MissionDataModel::searchData(const QString& missionNm , const QString& playerNm , int offset){
    QString sqlStr = "SELECT Name , Step , BlackChess , WhiteChess , Date FROM ChessData WHERE ";
    sqlStr += "Name='NAME' AND (BlackChess='PLAYERNAME' OR WhiteChess='PLAYERNAME') LIMIT LEN OFFSET OSET;";
    if(missionNm.isEmpty())
        sqlStr.replace("Name='NAME'","0=0");
    else
        sqlStr.replace("NAME",missionNm);
    if(playerNm.isEmpty())
        sqlStr.replace("BlackChess='BLACKCHESS' OR WhiteChess='WHITECHESS'","0=0");
    else
        sqlStr.replace("PLAYERNAME",playerNm);
    sqlStr.replace("LEN",QString::number(MISSION_COUNT_ONTPAGE));
    sqlStr.replace("OSET",QString::number(offset));
}









































```