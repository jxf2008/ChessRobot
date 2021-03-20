#ifndef MISSIONDATAMODEL_H_
#define MISSIONDATAMODEL_H_

#include <QAbstractTableModel>

class MissionDataModel : public QAbstractTableModel{
    Q_OBJECT
private:
    QVector<QString> titles;

    QVector<QString> blackPlayerNames;
    QVector<QString> whitePlayerNames;
    QVector<QString> dates;

    int currentPageNumber = 0;
    int dataCount = 0;
public:
    MissionDataModel(QObject* parent = nullptr);
    int rowCount(const QModelIndex& index)const;
    int columnCount(const QModelIndex& index)const;
    QVariant data(const QModelIndex& index , int role)const;
    QVariant headerData(int sec , Qt::Orientation orientation , int role)const;
    //void searchData(const QString& missionNm = QString() , const QString& playerNm = QString() , const QString& d = QString());
    void selectData(const QString& playerNm = QString() , const QString& d = QString());
    int getCurrentPageNumber()const{return currentPageNumber;}
    int getDataCount()const{return dataCount;}
    void clearModel();
public slots:
    void pageUp();
    void pageDown();
};
#endif









































