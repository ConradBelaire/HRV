#include "dbmanager.h"

const QString DBManager::DATABASE_PATH = "/database/denas.db";

DBManager::DBManager() {

    denasDB = QSqlDatabase::addDatabase("QSQLITE");
    denasDB.setDatabaseName("denas.db");

    if (!denasDB.open()) {
        throw "Error: Database could not be opened";
    }

    if (!DBInit()) {
        throw "Error: Database could not be initialized";
    }
}


bool DBManager::DBInit() {

    denasDB.transaction();

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS profiles ( id SERIAL PRIMARY KEY, battery_level FLOAT NOT NULL );");
    query.exec("CREATE TABLE IF NOT EXISTS log ( id SERIAL PRIMARY KEY, profile_id INTEGER NOT NULL, challenge_level INTEGER NOT NULL, is_low FLOAT NOT NULL, is_med FLOAT NOT NULL, is_high FLOAT NOT NULL, avg_coherence FLOAT NOT NULL, session_time INTEGER NOT NULL, achievement_score FLOAT NOT NULL, graph TEXT NOT NULL, date VARCHAR(255) NOT NULL, CONSTRAINT fk_profile FOREIGN KEY (profile_id) REFERENCES profile (id), CONSTRAINT check_percentages CHECK ((is_low + is_med + is_high) = 100) );");

    return denasDB.commit();
}


Profile* DBManager::getProfile(int id) {

    denasDB.transaction();

    QSqlQuery query;
    query.prepare("SELECT * FROM profile WHERE id = :profile_id;");
    query.bindValue(":profile_id", id);
    query.exec();

    if (!denasDB.commit()) {
        throw "Error: Query failed to execute";
    }

   // profile does not exist
    if (!query.next()) {
        addProfile(id, 100);
        Profile* pro = new Profile(id, 100);
        return pro;
    }

    // profile exists
    Profile* pro = new Profile(query.value(0).toInt(), query.value(1).toDouble());
    return pro;
}


bool DBManager::addProfile(int id, double batteryLvl, int sessionsAmt) {

    denasDB.transaction();

    QSqlQuery query;
    query.prepare("REPLACE INTO profiles (id, battery_level, sessionAmt) VALUES (:profile_id, :battery_level, :sessionAmt);");
    query.bindValue(":profile_id", id);
    query.bindValue(":battery_level", batteryLvl);
    query.bindValue(":sessionAmt", sessionsAmt);
    query.exec();

    return denasDB.commit();
}


bool DBManager::deleteProfile(int id) {

    denasDB.transaction();

    QSqlQuery query;
    query.prepare("DELETE FROM profiles WHERE id = :profile_id;");
    query.bindValue(":profile_id", id);
    query.exec();

    return denasDB.commit();
}


Log** DBManager::getProfileLogs(int profileId) {

    denasDB.transaction();

    QSqlQuery query;
    query.prepare("SELECT * FROM log WHERE profile_id = :profile_id;");
    query.bindValue(":profile_id", profileId);
    query.exec();

    if (!denasDB.commit()) {
        throw "Error: Query failed to execute";
    }

   // profile does not exist
    if (!query.next()) {
        throw "Error: Profile does not exist";
    }

    // profile exists
    Log** logs = new Log*[query.size()];
    int i = 0;
    while (query.next()) {
        logs[i] = new Log(
            query.value(0).toInt(),
            query.value(1).toInt(),
            query.value(2).toInt(),
            query.value(3).toFloat(),
            query.value(4).toFloat(),
            query.value(5).toFloat(),
            query.value(6).toFloat(),
            query.value(7).toInt(),
            query.value(8).toFloat(),
            query.value(9).toString(),
            query.value(10).toString()
        );
        i++;
    }
    return logs;
}


Log* DBManager::getLog(int id) {

    denasDB.transaction();

    QSqlQuery query;
    query.prepare("SELECT * FROM log WHERE id = :log_id;");
    query.bindValue(":log", id);
    query.exec();

    if (!denasDB.commit()) {
        throw "Error: Query failed to execute";
    }

   // Log does not exist
    if (!query.next()) {
        throw "Error: Log does not exist";
    }

    // Log exists
    Log* log = new Log(
        query.value(0).toInt(),
        query.value(1).toInt(),
        query.value(2).toInt(),
        query.value(3).toFloat(),
        query.value(4).toFloat(),
        query.value(5).toFloat(),
        query.value(6).toFloat(),
        query.value(7).toInt(),
        query.value(8).toFloat(),
        query.value(9).toString(),
        query.value(10).toString()
    );
    return log;
}


bool DBManager::addLog(
    int profile_id,
    int session_num,
    int challenge_level,
    float is_low,
    float is_med,
    float is_high,
    float avg_coherence,
    int session_time,
    float achievement_score,
    QString graph,
    QString date ) {

    denasDB.transaction();

    QSqlQuery query;
    query.prepare("INSERT INTO log ( profile_id, lod_id, challenge_level, is_low, is_med, is_high, avg_coherence, session_time, achievement_score, graph, date ) VALUES ( :profile_id, :lod_id, :challenge_level, :is_low, :is_med, :is_high, :avg_coherence, :session_time, :achievement_score, :graph, :date );");
    query.bindValue(":profile_id", profile_id);
    query.bindValue(":lod_id", session_num);
    query.bindValue(":challenge_level", challenge_level);
    query.bindValue(":is_low", is_low);
    query.bindValue(":is_med", is_med);
    query.bindValue(":is_high", is_high);
    query.bindValue(":avg_coherence", avg_coherence);
    query.bindValue(":session_time", session_time);
    query.bindValue(":achievement_score", achievement_score);
    query.bindValue(":graph", graph);
    query.bindValue(":date", date);
    query.exec();

    return denasDB.commit();
}


bool DBManager::deleteLog(int id) {
        denasDB.transaction();

        QSqlQuery query;
        query.prepare("DELETE FROM log WHERE id = :log_id;");
        query.bindValue(":log_id", id);
        query.exec();

        return denasDB.commit();
}


bool DBManager::deleteLogs() {
        denasDB.transaction();

        QSqlQuery query;
        query.exec("DELETE FROM log;");

        return denasDB.commit();
}

