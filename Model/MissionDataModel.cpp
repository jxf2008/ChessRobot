#include <QSqlQuery>
#include <Init/ConstData.h>
#include "Model/MissionDataModel.h"

MissionDataModel::MissionDataModel(QObject* parent):QAbstractTableModel (parent),
                                                    dates(MISSION_COUNT_ONTPAGE,""),
                                                    blackPlayerNames(MISSION_COUNT_ONTPAGE,""),
                                                    whitePlayerNames(MISSION_COUNT_ONTPAGE,""){
    titles.append(tr("对局名称"));
    titles.append(tr("黑棋"));
    titles.append(tr("白棋"));
    titles.append(tr("日期"));
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

void MissionDataModel::searchData(const QString& missionNm , const QString& playerNm , const QString& d , int offset){
    currentMissionName = missionNm;
    currentPlayerName = playerNm;
    currentDate = d;

    QString sqlStr = "SELECT MissionName , BlackPlayerName , WhitePlayerName , Date FROM PlayerData WHERE 0=0 ";
    sqlStr += "AND CD0 AND CD1 AND CD2 LIMIT LIM0 OFFSET OS0;";
    QString cd0 = missionNm.isEmpty() ? "0=0" : "MissionName='"+missionNm+"'";
    QString cd1 = playerNm.isEmpty() ? "0=0" : "(BlackPlayerName='"+missionNm+"' OR WhitePlayerName='"+missionNm+"')";
    QString cd2 = d.isEmpty() ? "0=0" : "Date='"+d+"'";
    QString lim0 = QString::number(MISSION_COUNT_ONTPAGE);
    QString os0 = QString::number(offset);
    sqlStr.replace("CD0",cd0);
    sqlStr.replace("CD1",cd1);
    sqlStr.replace("CD2",cd2);
    sqlStr.replace("LIM0",lim0);
    sqlStr.replace("OS0",os0);

    QSqlQuery sqlQuery;
    sqlQuery.exec(sqlStr);

    beginResetModel();
    missionNames.clear();
    blackPlayerNames.clear();
    whitePlayerNames.clear();
    dates.clear();
    while (sqlQuery.next()) {
        missionNames.append(sqlQuery.value("MissionName").toString());
        blackPlayerNames.append(sqlQuery.value("BlackPlayerName").toString());
        whitePlayerNames.append(sqlQuery.value("WhitePlayerName").toString());
        dates.append(sqlQuery.value("Date").toString());
    }
    endResetModel();
}

int MissionDataModel::getDataCount(){
    QString sqlStr = "SELECT MissionName , BlackPlayerName , WhitePlayerName , Date FROM PlayerData WHERE 0=0 ";
    sqlStr += "AND CD0 AND CD1 AND CD2;";
    QString cd0 = currentMissionName.isEmpty() ? "0=0" : "MissionName='"+currentMissionName+"'";
    QString cd1 = currentPlayerName.isEmpty() ? "0=0" : "(BlackPlayerName='"+currentPlayerName+"' OR WhitePlayerName='"+currentPlayerName+"')";
    QString cd2 = currentDate.isEmpty() ? "0=0" : "Date='"+currentDate+"'";
    sqlStr.replace("CD0",cd0);
    sqlStr.replace("CD1",cd1);
    sqlStr.replace("CD2",cd2);

    QSqlQuery sqlQuery;
    sqlQuery.exec(sqlStr);
    return sqlQuery.size();
}

void MissionDataModel::clearModel(){
    beginResetModel();
    dates.clear();
    blackPlayerNames.clear();
    whitePlayerNames.clear();
    currentOffSet = 0;
    dataCount = 0;
    endResetModel();
}

void MissionDataModel::pageDown(){
    if((currentOffSet+1)* MISSION_COUNT_ONTPAGE >= dataCount)
        return;
    beginResetModel();
    ++currentOffSet;
    searchData(currentMissionName,currentPlayerName,currentDate,currentOffSet);
    endResetModel();
}

void MissionDataModel::pageUp(){
    if(currentOffSet == 0)
        return;
    beginResetModel();
    --currentOffSet;
    searchData(currentMissionName,currentPlayerName,currentDate,currentOffSet);
    endResetModel();
}


































