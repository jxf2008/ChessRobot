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