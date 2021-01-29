#include <QSqlQuery>
#include <Init/ConstData.h>
#include "Model/MissionDataModel.h"

MissionDataModel::MissionDataModel(QObject* parent):QAbstractTableModel (parent),
                                                    dates(MISSION_COUNT_ONTPAGE,""),
                                                    blackPlayerNames(MISSION_COUNT_ONTPAGE,""),
                                                    whitePlayerNames(MISSION_COUNT_ONTPAGE,""){
    titles.append(tr("日期"));
    titles.append(tr("黑棋"));
    titles.append(tr("白棋"));
}

int MissionDataModel::rowCount(const QModelIndex& index)const{
    Q_UNUSED(index)
    return MISSION_COUNT_ONTPAGE;
}

int MissionDataModel::columnCount(const QModelIndex& index)const{
    Q_UNUSED(index)
    return titles.size();
}

QVariant MissionDataModel::data(const QModelIndex& index , int role)const{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole){
        int r = index.row();
        int c = index.column();
        if((r + currentOffSet * MISSION_COUNT_ONTPAGE) >= dates.size())
            return QVariant();
        if(c == 1)
            return QVariant(blackPlayerNames.at(r + currentOffSet * MISSION_COUNT_ONTPAGE));
        if(c == 2)
            return QVariant(whitePlayerNames.at(r + currentOffSet * MISSION_COUNT_ONTPAGE));
        if(c == 0)
            return QVariant(dates.at(r + currentOffSet * MISSION_COUNT_ONTPAGE));
    }
    if(role == Qt::TextAlignmentRole)
        return QVariant(Qt::AlignCenter);

    return QVariant();
}

QVariant MissionDataModel::headerData(int sec , Qt::Orientation orientation , int role)const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(titles.at(sec));
    if(orientation == Qt::Vertical && role == Qt::DisplayRole)
        return QVariant(sec+1);
    return QVariant();
}

void MissionDataModel::searchData(const QString& playerNm){
    currentPlayerName = playerNm;
    QString sqlStr = "SELECT Date , BlackPlayerName , WhitePlayerName FROM PlayerData WHERE ";
    sqlStr += "(CONDITION);";
    if(currentPlayerName.isEmpty())
        sqlStr.replace("CONDITION","0=0");
    else
        sqlStr.replace("CONDITION","BlackPlayerName = '"+currentPlayerName+"' OR WhitePlayerName = '"+currentPlayerName+"'");

    QSqlQuery sqlQuery;
    sqlQuery.exec(sqlStr);

    beginResetModel();
    dates.clear();
    blackPlayerNames.clear();
    whitePlayerNames.clear();
    while (sqlQuery.next()) {
        dates.append(sqlQuery.value("Date").toString());
        blackPlayerNames.append(sqlQuery.value("BlackPlayerName").toString());
        whitePlayerNames.append(sqlQuery.value("WhitePlayerName").toString());
    }
    endResetModel();
    dataCount = dates.size();
}

void MissionDataModel::clearModel(){
    beginResetModel();
    dates.clear();
    blackPlayerNames.clear();
    whitePlayerNames.clear();
    dataCount = 0;
    currentOffSet = 0;
    endResetModel();
}

void MissionDataModel::pageDown(){
    if((currentOffSet+1)* MISSION_COUNT_ONTPAGE >= dataCount)
        return;
    beginResetModel();
    ++currentOffSet;
    endResetModel();
}

void MissionDataModel::pageUp(){
    if(currentOffSet == 0)
        return;
    beginResetModel();
    --currentOffSet;
    endResetModel();
}


































