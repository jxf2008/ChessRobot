ConstData.h
```c++
#ifndef CONSTDATA_H
#define CONSTDATA_H

#include <QDebug>

const QString EMPTY_PICTURE = ":/images/EmptyChess.png";
const QString WHITE_PICTURE = ":/images/WhiteChess.png";
const QString BLACK_PICTURE = ":/images/BlackChess.png";
const QString DEFAULT_AI_PICTURE = ":/images/DefaultAI.png";
const QString EXE_ICO = ":/images/TitleImage.png";

const QString DATE_FORMAT = "yyyy-MM-dd hh:mm:ss";

enum ChessType
{
    BlackChess,
    WhiteChess,
    EmptyChess
};

struct ChessDetail
{
    int row;
    int col;
    int verLine;   //每个旗子在4个方向上的数量，如果这条线2个对方的棋子小于5，则该值为-1
    int horLine;
    int leftUpLine;
    int rightUpLine;
};

enum ErroInfo{
    NORMAL = 0,
    ARG_ERRO = 1,
};

const int CHESS_SIZE = 25;    //棋子的大小

const int CHESS_COUNT = 19;   //棋盘大小，每行/列的最大棋子数

const int VICTORY_CONDITION = 5;   //胜利条件

const int LOG_SIZE = 50;  //对战双方LOG的大小

const QString HUMAN_PLAYER_NAME = "玩家";

const QString DEFAULT_AI_NAME = "默认AI";

#ifdef Q_OS_WIN
const QString AI_LIBARY_FILE = ".dll";
#endif
#ifdef Q_OS_LINUX
const QString AI_LIBARY_FILE = ".so";
#endif
#ifdef Q_OS_MAC
const QString AI_LIBARY_FILE = ".dylib";
#endif

typedef int (*CALCULATECHESS)(int* , int , int);
const QString AI_FUN_NAME = "calculateChess";

const int MISSION_COUNT_ONTPAGE = 10;

#endif // CONSTDATA_H
```

```c++
#ifndef INITBUILDER_H
#define INITBUILDER_H

#include <QString>

class InitBuilder{
private:
    QString aiPathString;
    QString aiPicturePathString;
    InitBuilder();
    InitBuilder(const InitBuilder&);
    InitBuilder operator=(const InitBuilder&);
public:
    static InitBuilder& getInstance();
    QList<QString> getAIName()const;
    QString getAIDir()const;
    QString getAIPictureDir()const;
    void createFile();
};

#endif

```

```c++
#include <QDir>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "Init/InitBuilder.h"
#include "Init/ConstData.h"

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
    QString sqlStr = "CREATE TABLE IF NOT EXISTS ChessData(Name TEXT , Step TEXT , BlackChess TEXT , WhiteChess TEXT , Date TEXT);";
    sqlQuery.exec(sqlStr);
}


















































```