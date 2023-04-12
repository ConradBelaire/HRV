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

    if (!query.exec("CREATE TABLE IF NOT EXISTS log ( session_id INTEGER PRIMARY KEY, profile_id INTEGER NOT NULL, challenge_level INTEGER NOT NULL, is_low INTEGER NOT NULL, is_med INTEGER NOT NULL, is_high INTEGER NOT NULL, session_time INTEGER NOT NULL, achievement_score FLOAT NOT NULL, coherence_count INTEGER NOT NULL, heart_rates TEXT, CONSTRAINT fk_profile FOREIGN KEY (profile_id) REFERENCES profiles (id) ON DELETE CASCADE );")){
        qDebug() << "Error: " << query.lastError();
    }
    return hrvDB.commit();
}


Profile* DBManager::getProfile(int id) {

    hrvDB.transaction();

    QSqlQuery query;
    query.prepare("SELECT * FROM profiles WHERE :id = id");
    query.bindValue(":id", id);
    query.exec();

    if (!hrvDB.commit()) {
        throw "Error: Query failed to execute";
    }

   // profile does not exist
    if (!query.next()) {
        qDebug() << "new Profile created";
        addProfile(id, 100, 0);
        Profile* pro = new Profile(id, 100, 1);
        return pro;
    }

    qDebug() << query.value(0).toInt() << query.value(1).toDouble() << "id and battery level";

    // profile exists
    Profile* pro = new Profile(query.value(0).toInt(), query.value(1).toDouble(), getLogCount(id));
    return pro;
}

int DBManager::getLogCount(int profile_id) {
    hrvDB.transaction();

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM log WHERE profile_id=:profile_id");
    query.bindValue(":profile_id", profile_id);
    query.exec();

    if (!hrvDB.commit()) {
        throw "Error: Query failed to execute";
    }

    if (!query.next()) {
        return 0;
    }

    return query.value(0).toInt();
}

bool DBManager::updateProfile(int id, double batteryLvl) {

    hrvDB.transaction();

    QSqlQuery query;
    query.prepare("UPDATE profiles SET battery_level = :battery_level WHERE id = :profile_id;");
    query.bindValue(":profile_id", id);
    query.bindValue(":battery_level", batteryLvl);
    query.exec();

    qDebug() << query.lastError().text();

    return hrvDB.commit();
}


bool DBManager::addProfile(int id, double batteryLvl, int sessionsAmt) {

    hrvDB.transaction();

    QSqlQuery query;
    query.prepare("REPLACE INTO profiles (id, battery_level) VALUES (:id, :battery_level);");
    query.bindValue(":id", id);
    query.bindValue(":battery_level", batteryLvl);
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


QVector<Log*>* DBManager::getLogs() {

    hrvDB.transaction();

    QSqlQuery query;
    query.exec("SELECT * FROM log;");

    QVector<Log*>* logs = new QVector<Log*>();

    int i = 0;
    while (query.next()) {
        QVector<int> intVector;
        QJsonDocument heartRatesDoc = QJsonDocument::fromJson(query.value(9).toString().toUtf8());
        QJsonArray heartRatesArray = heartRatesDoc.array();
        for (const QJsonValue &value : heartRatesArray) {
            intVector.append(value.toInt());
        }

        logs->append(
            new Log(
                query.value(0).toInt(), // profile id
                query.value(1).toInt(), // session id
                query.value(2).toInt(), // challenge level
                query.value(3).toInt(), // is low
                query.value(4).toInt(), // is med
                query.value(5).toInt(), // is high
                query.value(6).toInt(), // session time
                query.value(7).toFloat(), // achievement score
                query.value(8).toInt(), // coherence count
                intVector
            )
        );
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
    query.prepare("SELECT * FROM log WHERE :id = session_id");
    query.bindValue(":id", id);
    query.exec();


    QVector<int> intVector;
    QJsonDocument heartRatesDoc = QJsonDocument::fromJson(query.value(9).toString().toUtf8());
    QJsonArray heartRatesArray = heartRatesDoc.array();
    for (const QJsonValue &value : heartRatesArray) {
        intVector.append(value.toInt());
    }

    // Log exists
    Log* log = new Log(
                query.value(0).toInt(), // profile id
                query.value(1).toInt(), // session id
                query.value(2).toInt(), // challenge level
                query.value(3).toInt(), // is low
                query.value(4).toInt(), // is med
                query.value(5).toInt(), // is high
                query.value(6).toInt(), // session time
                query.value(7).toFloat(), // achievement score
                query.value(8).toInt(), // coherence count
                intVector
    );
    return log;
}


bool DBManager::addLog(Log* log) {
    qDebug() << log->getId() << " DAB ME up before database";
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
        qDebug() << id << "the Id of the log deleted";

        QSqlQuery query;
        query.prepare("DELETE FROM log WHERE session_id = :log_id;");
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


void DBManager::dropTables() {
    QSqlQuery query;
    query.exec("DROP TABLE IF EXISTS log;");
    qDebug() << query.lastError().text();
    query.exec("DROP TABLE IF EXISTS profiles;");

    qDebug() << query.lastError().text();
}

