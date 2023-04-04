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
    query.exec("CREATE TABLE IF NOT EXISTS profiles ( id SERIAL PRIMARY KEY, battery_level FLOAT NOT NULL );");
    query.exec("CREATE TABLE IF NOT EXISTS log ( id SERIAL PRIMARY KEY, profile_id INTEGER NOT NULL, challenge_level INTEGER NOT NULL, is_low INTEGER NOT NULL, is_med INTEGER NOT NULL, is_high INTEGER NOT NULL, avg_coherence FLOAT NOT NULL, session_time INTEGER NOT NULL, pacer_duration INTEGER NOT NULL, achievement_score FLOAT NOT NULL, date VARCHAR(255)NOT NULL, CONSTRAINT fk_profile FOREIGN KEY (profile_id) REFERENCES profiles (id) ON DELETE CASCADE, CONSTRAINT check_percentages CHECK ((is_low + is_med + is_high) = 100) );" );

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
        Profile* pro = new Profile(id, 100, 0);
        return pro;
    }

    // profile exists
    Profile* pro = new Profile(query.value(0).toInt(), query.value(1).toDouble(), query.value(2).toInt());
    return pro;
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
        throw "Error: Profile does not exist";
    }

    QVector<Log*>* logs = new QVector<Log*>();

    // profile exists
    int i = 0;
    while (query.next()) {
        logs->append(
            new Log(
                id,
                query.value(0).toInt(),
                query.value(1).toInt(),
                query.value(2).toInt(),
                query.value(3).toInt(),
                query.value(4).toInt(),
                query.value(5).toFloat(),
                query.value(6).toInt(),
                query.value(7).toInt(),
                query.value(8).toFloat(),
                query.value(9).toString())
        );
        i++;
    }
    return logs;
}


bool doesLogExist(int id){
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

    // Log exists
    Log* log = new Log(
        id,
        query.value(0).toInt(),
        query.value(1).toInt(),
        query.value(2).toInt(),
        query.value(3).toInt(),
        query.value(4).toInt(),
        query.value(5).toFloat(),
        query.value(6).toInt(),
        query.value(7).toInt(),
        query.value(8).toFloat(),
        query.value(9).toString()
    );
    return log;
}

bool DBManager::addLog(Log* log) {

    hrvDB.transaction();

    QSqlQuery query;
    query.prepare("INSERT INTO log (profile_id, challenge_level, is_low, is_med, is_high, avg_coherence, session_time, pacer_duration, achievement_score, graph, date) VALUES (:profile_id, :challenge_level, :is_low, :is_med, :is_high, :avg_coherence, :session_time, :pacer_duration, :achievement_score, :date);");
    query.bindValue(":profile_id", log->getProfileId());
    query.bindValue(":challenge_level", log->getChallengeLevel());
    query.bindValue(":is_low", log->getIsLow());
    query.bindValue(":is_med", log->getIsMed());
    query.bindValue(":is_high", log->getIsHigh());
    query.bindValue(":avg_coherence", log->getAvgCoherence());
    query.bindValue(":session_time", log->getSessionTime());
    query.bindValue(":pacer_duration", log->getPacerDuration());
    query.bindValue(":achievement_score", log->getAchievementScore());
    query.bindValue(":date", log->getDate());
    query.exec();

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

