#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QDateTime>
#include <QDebug>
#include <QSqlQuery>
#include <QList>
#include <QApplication>


class DBManager {

public:
    const QString DATE_FORMAT = "yyyy-MM-dd hh:mm";
    static const QString DATABASE_PATH;

    DBManager();
    bool addSessionRecord(const QString& therapy, const QDateTime& time, int powerLevel, int duration);
    bool UpdateSessionRecord(const QString& therapy, const QDateTime& time, int powerLevel, int duration);
    bool addProfile(int id, double batterLvl, int powerLvl);
    bool deleteRecords();

    Profile* getProfile(int id);
    QVector<Record*> getSession(int id);


private:
    QSqlDatabase denasDB;

    bool isValidRecord(const QString& recordType, const QDateTime& time, int powerLevel, int duration);
    bool addRecord(const QString& tableName, const QString& name, const QDateTime& time, int powerLevel, int duration);
    bool DBInit();

};

#endif // DBMANAGER_
