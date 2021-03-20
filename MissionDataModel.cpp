#ifdef MSVC
#pragma  execution_character_set("utf-8")
#endif

#include <QSqlQuery>
#include <ConstData.h>
#include <QColor>
#include <QBrush>
#include "MissionDataModel.h"

MissionDataModel::MissionDataModel(QObject* parent):QAbstractTableModel (parent),
                                                    titles({"黑棋","白棋","日期"}){

}

int MissionDataModel::rowCount(const QModelIndex& index)const{
    Q_UNUSED(index)
    return MISSION_COUNT_ONEPAGE;
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
        if((currentPageNumber - 1) * MISSION_COUNT_ONEPAGE + r >= dataCount)
            return QVariant();
        if (c == 0)
            return QVariant(blackPlayerNames.at((currentPageNumber-1) * MISSION_COUNT_ONEPAGE + r));
        if (c == 1)
            return QVariant(whitePlayerNames.at((currentPageNumber-1) * MISSION_COUNT_ONEPAGE + r));
        if (c == 2)
            return QVariant(dates.at((currentPageNumber-1) * MISSION_COUNT_ONEPAGE + r));
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

/*void MissionDataModel::searchData(const QString& missionNm , const QString& playerNm , const QString& d){
    currentPlayerName = playerNm;
    currentDate = d;

    QString sqlStr = "SELECT MissionName , BlackPlayerName , WhitePlayerName , Date FROM PlayerData WHERE 0=0 ";
    sqlStr += "AND CD0 AND CD1 AND CD2 LIMIT LIM0 ,10;";
    QString cd0 = missionNm.isEmpty() ? "0=0" : "MissionName='"+missionNm+"'";
    QString cd1 = playerNm.isEmpty() ? "0=0" : "(BlackPlayerName='"+playerNm+"' OR WhitePlayerName='"+playerNm+"')";
    QString cd2 = d.isEmpty() ? "0=0" : "Date='"+d+"'";
    QString lim0 = QString::number(MISSION_COUNT_ONEPAGE * (currentPageNumber - 1));
    sqlStr.replace("CD0",cd0);
    sqlStr.replace("CD1",cd1);
    sqlStr.replace("CD2",cd2);
    sqlStr.replace("LIM0",lim0);

    QSqlQuery sqlQuery;
    sqlQuery.exec(sqlStr);

    beginResetModel();
    blackPlayerNames.clear();
    whitePlayerNames.clear();
    dates.clear();
    while (sqlQuery.next()) {
        blackPlayerNames.append(sqlQuery.value("BlackPlayerName").toString());
        whitePlayerNames.append(sqlQuery.value("WhitePlayerName").toString());
        dates.append(sqlQuery.value("Date").toString());
    }
    endResetModel();
}*/

void MissionDataModel::selectData(const QString& playerNm , const QString& d){
    QString sqlStr = "SELECT MissionName , BlackPlayerName , WhitePlayerName , Date FROM PlayerData WHERE 0=0 ";
    sqlStr += "AND CD0 AND CD1 ;";
    QString cd0 = playerNm.isEmpty() ? "0=0" : "(BlackPlayerName='"+playerNm+"' OR WhitePlayerName='"+playerNm+"')";
    QString cd1 = d.isEmpty() ? "0=0" : "Date='"+d+"'";
    sqlStr.replace("CD0",cd0);
    sqlStr.replace("CD1",cd1);

    QSqlQuery sqlQuery;
    sqlQuery.exec(sqlStr);

    beginResetModel();
    blackPlayerNames.clear();
    whitePlayerNames.clear();
    dates.clear();
    while (sqlQuery.next()) {
        blackPlayerNames.append(sqlQuery.value("BlackPlayerName").toString());
        whitePlayerNames.append(sqlQuery.value("WhitePlayerName").toString());
        dates.append(sqlQuery.value("Date").toString());
    }
    endResetModel();

    dataCount = whitePlayerNames.size();
    currentPageNumber = dataCount == 0 ? 0 : 1;
}

void MissionDataModel::clearModel(){
    beginResetModel();
    dates.clear();
    blackPlayerNames.clear();
    whitePlayerNames.clear();
    currentPageNumber = 0;
    dataCount = 0;
    endResetModel();
}

void MissionDataModel::pageDown(){
    if(currentPageNumber * MISSION_COUNT_ONEPAGE >= dataCount || dataCount == 0)
        return;
    beginResetModel();
    ++currentPageNumber;
    endResetModel();
}

void MissionDataModel::pageUp(){
    if(currentPageNumber == 1)
        return;
    beginResetModel();
    --currentPageNumber;
    endResetModel();
}


































