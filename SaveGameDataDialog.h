#ifndef SAVEGAMEDATADIALOG_H
#define SAVEGAMEDATADIALOG_H

#include <QDialog>
#include <QRadioButton>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCloseEvent>

class SaveGameDataDialog : public QDialog{
    Q_OBJECT
private:
    QLabel* isSave_Label;
    QLabel* missionName_Label;

    QRadioButton* save_RadioButton;
    QRadioButton* giveUp_RadioButton;

    QLineEdit* missionName_LineEdit;

    QPushButton* save_PushButton;
    QPushButton* cancel_PushButton;
public:
    explicit SaveGameDataDialog(QWidget *parent = nullptr);
protected:
    void closeEvent(QCloseEvent* event) override;
signals:
    void sameGame(bool save , QString nm);
private slots:
    void saveGameData();
    void cancelSave();
};

#endif // SAVEGAMEDATADIALOG_H




































