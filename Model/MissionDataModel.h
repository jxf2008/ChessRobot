#ifndef MISSIONDATAMODEL_H_
#define MISSIONDATAMODEL_H_

#include <QAbstractTableModel>

class MissionDataModel : public QAbstractTableModel{
    Q_OBJECT
private:
    QVector<QString> titles;

    QVector<QString> dates;
    QVector<QString> blackPlayerNames;
    QVector<QString> whitePlayerNames;

    int currentOffSet = 0;
    int dataCount = 0;

    QString currentPlayerName = QString();
public:
    MissionDataModel(QObject* parent = nullptr);
    int rowCount(const QModelIndex& index)const;
    int columnCount(const QModelIndex& index)const;
    QVariant data(const QModelIndex& index , int role)const;
    QVariant headerData(int sec , Qt::Orientation orientation , int role)const;
    void searchData(const QString& playerNm = QString());
    int getCurrentOffset()const{return currentOffSet;}
    int getDataCount()const{return dataCount;}
    void clearModel();
public slots:
    void pageUp();
    void pageDown();
};
#endif
