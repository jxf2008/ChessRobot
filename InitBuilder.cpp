#include <QDir>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "InitBuilder.h"
#include "ConstData.h"

InitBuilder::InitBuilder(){

}

InitBuilder& InitBuilder::getInstance(){
    static InitBuilder a;
    return a;
}

QList<QString> InitBuilder::getAIName()const{
    QList<QString> nms;
    QDir aiDir(aiPathString);
    if(!aiDir.exists())
        return nms;
    int aiCount = static_cast<int>(aiDir.count());
    for(int i = 0 ; i < aiCount ; ++i){
        QString nm = aiDir[i];
        if(nm.right(AI_LIBARY_FILE.length()) == AI_LIBARY_FILE)
            nms.append(aiDir[i].replace(AI_LIBARY_FILE,""));
    }
    return nms;
}

QString InitBuilder::getAIDir()const{
    return aiPathString;
}

QString InitBuilder::getAIPictureDir()const{
    return aiPicturePathString;
}

void InitBuilder::createFile(){
    QString currentPath = QCoreApplication::applicationDirPath();
    aiPathString = currentPath + "/AI";
    QDir aiDir(aiPathString);
    if(!aiDir.exists())
        aiDir.mkdir(aiPathString);

    aiPicturePathString = currentPath + "/AIPicture";
    QDir aiPictureDir(aiPicturePathString);
    if(!aiPictureDir.exists())
        aiPictureDir.mkdir(aiPicturePathString);

    QString dbPath = currentPath + "/DB";
    QDir dbDir(dbPath);
    if(!dbDir.exists())
        dbDir.mkdir(dbPath);

    QString dbFilePath = dbPath + "/" + "ChessData.db";
    QFile dbFs(dbFilePath);
    if(!dbFs.exists()){
        dbFs.open(QIODevice::ReadWrite);
        dbFs.close();
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbFilePath);
    db.open();

    QSqlQuery sqlQuery;
    QString sqlStr = "CREATE TABLE IF NOT EXISTS ChessData(MissionName TEXT,ChessIndex INTEGER,ChessStep INTEGER, ChessType INTEGER);";
    sqlQuery.exec(sqlStr);
    sqlStr = "CREATE TABLE IF NOT EXISTS PlayerData(MissionName TEXT , BlackPlayerName TEXT , WhitePlayerName TEXT , Winner TEXT ,Date TEXT);";
    sqlQuery.exec(sqlStr);
}

void InitBuilder::createQss(){

}

































