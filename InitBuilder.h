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
    void createQss();
};

#endif
