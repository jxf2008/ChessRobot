#ifndef MISSIONDATAMODEL_H_
#define MISSIONDATAMODEL_H_

#include <QAbstractTableModel>

class MissionDataModel : public QAbstractTableModel{
    Q_OBJECT
private:
    QVector<QString> titles;

    QVector<QString> missionNames;
    QVector<QString> blackPlayerNames;
    QVector<QString> whitePlayerNames;
    QVector<QString> dates;

    QString currentMissionName = "";
    QString currentPlayerName = "";
    QString currentDate = "";

    int currentPageNumber = 1;
    int dataCount = 0;
public:
    MissionDataModel(QObject* parent = nullptr);
    int rowCount(const QModelIndex& index)const;
    int columnCount(const QModelIndex& index)const;
    QVariant data(const QModelIndex& index , int role)const;
    QVariant headerData(int sec , Qt::Orientation orientation , int role)const;
    void searchData(const QString& missionNm = QString() , const QString& playerNm = QString() , const QString& d = QString() , int offset = 0);
    int getCurrentPageNumber()const{return currentPageNumber;}
    int getDataCount();
    void clearModel();
public slots:
    void pageUp();
    void pageDown();
};
#endif









































