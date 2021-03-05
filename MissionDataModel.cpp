#ifdef MSVC
#pragma  execution_character_set("utf-8")
#endif

#include <QSqlQuery>
#include <ConstData.h>
#include "MissionDataModel.h"

MissionDataModel::MissionDataModel(QObject* parent):QAbstractTableModel (parent),
                                                    blackPlayerNames(MISSION_COUNT_ONTPAGE,""),
                                                    whitePlayerNames(MISSION_COUNT_ONTPAGE,""),
                                                    dates(MISSION_COUNT_ONTPAGE,""){
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
        if(r >= dates.size())
            return QVariant();
        if (c == 0)
            return QVariant(missionNames.at(r));
        if (c == 1)
            return QVariant(blackPlayerNames.at(r));
        if (c == 2)
            return QVariant(whitePlayerNames.at(r));
        if (c == 3)
            return QVariant(dates.at(r));
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

void MissionDataModel::searchData(const QString& missionNm , const QString& playerNm , const QString& d){
    currentMissionName = missionNm;
    currentPlayerName = playerNm;
    currentDate = d;

    QString sqlStr = "SELECT MissionName , BlackPlayerName , WhitePlayerName , Date FROM PlayerData WHERE 0=0 ";
    sqlStr += "AND CD0 AND CD1 AND CD2 LIMIT LIM0 ,10;";
    QString cd0 = missionNm.isEmpty() ? "0=0" : "MissionName='"+missionNm+"'";
    QString cd1 = playerNm.isEmpty() ? "0=0" : "(BlackPlayerName='"+missionNm+"' OR WhitePlayerName='"+missionNm+"')";
    QString cd2 = d.isEmpty() ? "0=0" : "Date='"+d+"'";
    QString lim0 = QString::number(MISSION_COUNT_ONTPAGE * (currentPageNumber - 1));
    sqlStr.replace("CD0",cd0);
    sqlStr.replace("CD1",cd1);
    sqlStr.replace("CD2",cd2);
    sqlStr.replace("LIM0",lim0);

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
    QString sqlStr = "SELECT MissionName  FROM PlayerData;";
    QSqlQuery sqlQuery;
    sqlQuery.exec(sqlStr);
    dataCount = 0;
    while (sqlQuery.next()) ++dataCount;
    dataCount = dataCount % MISSION_COUNT_ONTPAGE == 0 ? dataCount / MISSION_COUNT_ONTPAGE : dataCount / MISSION_COUNT_ONTPAGE + 1;
    return dataCount;
}

void MissionDataModel::clearModel(){
    beginResetModel();
    dates.clear();
    blackPlayerNames.clear();
    whitePlayerNames.clear();
    missionNames.clear();
    currentPageNumber = 1;
    dataCount = 0;
    endResetModel();
}

void MissionDataModel::pageDown(){
    if(currentPageNumber == dataCount)
        return;
    ++currentPageNumber;
    searchData(currentMissionName,currentPlayerName,currentDate);
}

void MissionDataModel::pageUp(){
    if(currentPageNumber == 1)
        return;
    --currentPageNumber;
    searchData(currentMissionName,currentPlayerName,currentDate);
}


































