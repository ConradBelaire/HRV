#include "dbmanager.h"

const QString DBManager::DATABASE_PATH = "/database/hrv.db";

DBManager::DBManager() {

    hrvDB = QSqlDatabase::addDatabase("QSQLITE");
    hrvDB.setDatabaseName("hrv.db");

    if (!hrvDB.open()) {
        throw "Error: Database could not be opened";
    }

    if (!DBInit()) {
        throw "Error: Database could not be initialized";
    }
}


bool DBManager::DBInit() {

    hrvDB.transaction();

    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS profiles ( id INTEGER PRIMARY KEY, battery_level FLOAT NOT NULL );")){
        qDebug() << "Error: " << query.lastError();
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS log ( id INTEGER PRIMARY KEY, profile_id INTEGER NOT NULL, challenge_level INTEGER NOT NULL, is_low INTEGER NOT NULL, is_med INTEGER NOT NULL, is_high INTEGER NOT NULL, session_time INTEGER NOT NULL, achievement_score FLOAT NOT NULL, coherence_count INTEGER NOT NULL, heart_rates TEXT, CONSTRAINT fk_profile FOREIGN KEY (profile_id) REFERENCES profiles (id) ON DELETE CASCADE );")){
        qDebug() << "Error: " << query.lastError();
    }
    return hrvDB.commit();
}


Profile* DBManager::getProfile(int id) {

    hrvDB.transaction();

    QSqlQuery query;
    query.prepare("SELECT * FROM profile WHERE id = :profile_id;");
    query.bindValue(":profile_id", id);
    query.exec();

    if (!hrvDB.commit()) {
        throw "Error: Query failed to execute";
    }

   // profile does not exist
    if (!query.next()) {
        addProfile(id, 100, 0);
        Profile* pro = new Profile(id, 100, 1);
        return pro;
    }

    // profile exists
    Profile* pro = new Profile(query.value(0).toInt(), query.value(1).toDouble(), query.value(2).toInt());
    return pro;
}

bool DBManager::updateProfile(int id, double batteryLvl, int sessionAmt) {

    hrvDB.transaction();

    QSqlQuery query;
    query.prepare("UPDATE profiles SET battery_level = :battery_level, sessionAmt = :sessionAmt WHERE id = :profile_id;");
    query.bindValue(":profile_id", id);
    query.bindValue(":battery_level", batteryLvl);
    query.bindValue(":sessionAmt", sessionAmt);
    query.exec();

    return hrvDB.commit();
}

bool DBManager::addProfile(int id, double batteryLvl, int sessionsAmt) {

    hrvDB.transaction();

    QSqlQuery query;
    query.prepare("REPLACE INTO profiles (id, battery_level, sessionAmt) VALUES (:profile_id, :battery_level, :sessionAmt);");
    query.bindValue(":profile_id", id);
    query.bindValue(":battery_level", batteryLvl);
    query.bindValue(":sessionAmt", sessionsAmt);
    query.exec();

    return hrvDB.commit();
}


bool DBManager::deleteProfile(int id) {

    hrvDB.transaction();

    QSqlQuery query;
    query.prepare("DELETE FROM profiles WHERE id = :profile_id;");
    query.bindValue(":profile_id", id);
    query.exec();

    return hrvDB.commit();
}


QVector<Log*>* DBManager::getLogs(int id) {

    hrvDB.transaction();

    QSqlQuery query;
    query.prepare("SELECT * FROM log WHERE profile_id = :profile_id;");
    query.bindValue(":profile_id", id);
    query.exec();

    if (!hrvDB.commit()) {
        throw "Error: Query failed to execute";
    }

   // profile does not exist
    if (!query.next()) {
        qDebug() << "ERR: No logs found";
        return new QVector<Log*>();
    }

    QVector<Log*>* logs = new QVector<Log*>();

    // profile exists
    int i = 0;
    bool first = true;
    while (first || query.next()) {
        QVector<int> intVector;
        QJsonDocument heartRatesDoc = QJsonDocument::fromJson(query.value(10).toString().toUtf8());
        QJsonArray heartRatesArray = heartRatesDoc.array();
        for (const QJsonValue &value : heartRatesArray) {
            intVector.append(value.toInt());
        }

        logs->append(
            new Log(
                id,
                query.value(0).toInt(),
                query.value(1).toInt(),
                query.value(2).toInt(),
                query.value(3).toInt(),
                query.value(4).toInt(),
                query.value(5).toInt(),
                query.value(6).toFloat(),
                query.value(7).toInt(),
                intVector
            )
        );
        first = false;
        i++;
    }
    return logs;
}


bool DBManager::doesLogExist(int id){
    hrvDB.transaction();

    QSqlQuery query;
    query.prepare("SELECT * FROM log WHERE id = :log_id;");
    query.bindValue(":log_id", id);
    query.exec();

    if (!hrvDB.commit()) {
        throw "Error: Query failed to execute";
    }

    if (!query.next()) {
        return false;
    }
    return true;
}


Log* DBManager::getLog(int id) {
    hrvDB.transaction();

    QSqlQuery query;
    query.prepare("SELECT * FROM log WHERE id = :log_id;");
    query.bindValue(":log", id);
    query.exec();

    if (!hrvDB.commit()) {
        throw "Error: Query failed to execute";
    }

   // Log does not exist
    if (!query.next()) {
        throw "Error: Log does not exist";
    }

    QVector<int> intVector;
    QJsonDocument heartRatesDoc = QJsonDocument::fromJson(query.value(10).toString().toUtf8());
    QJsonArray heartRatesArray = heartRatesDoc.array();
    for (const QJsonValue &value : heartRatesArray) {
        intVector.append(value.toInt());
    }

    // Log exists
    Log* log = new Log(
        id,
        query.value(0).toInt(),
        query.value(1).toInt(),
        query.value(2).toInt(),
        query.value(3).toInt(),
        query.value(4).toInt(),
        query.value(5).toInt(),
        query.value(6).toFloat(),
        query.value(7).toInt(),
        intVector
    );
    return log;
}

bool DBManager::addLog(Log* log) {
    hrvDB.transaction();

    QJsonArray heartRatesArray;
    for (int value : log->getHeartRates_int()) {
        heartRatesArray.append(value);
    }
    QJsonDocument heartRatesDoc(heartRatesArray);
    QString heartRatesJson = heartRatesDoc.toJson(QJsonDocument::Compact);

    QSqlQuery query;
    query.prepare("INSERT INTO log (profile_id, challenge_level, is_low, is_med, is_high, session_time, achievement_score, coherence_count, heart_rates) VALUES (:profile_id, :challenge_level, :is_low, :is_med, :is_high, :session_time, :achievement_score, :coherence_count, :heart_rates);");
    query.bindValue(":profile_id", log->getProfileId());
    query.bindValue(":challenge_level", log->getChallengeLevel());
    query.bindValue(":is_low", log->getIsLow());
    query.bindValue(":is_med", log->getIsMed());
    query.bindValue(":is_high", log->getIsHigh());
    query.bindValue(":session_time", log->getSessionTime());
    query.bindValue(":achievement_score", log->getAchievementScore());
    query.bindValue(":coherence_count", log->getCoherenceCount());
    query.bindValue(":heart_rates", heartRatesJson);
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        hrvDB.rollback();
        return false;
    }

    qDebug() << "Log added";
    return hrvDB.commit();
}


bool DBManager::deleteLog(int id) {
        hrvDB.transaction();

        QSqlQuery query;
        query.prepare("DELETE FROM log WHERE id = :log_id;");
        query.bindValue(":log_id", id);
        query.exec();

        return hrvDB.commit();
}


bool DBManager::deleteLogs() {
        hrvDB.transaction();

        QSqlQuery query;
        query.exec("DELETE FROM log;");

        return hrvDB.commit();
}

