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
    query.exec("CREATE TABLE IF NOT EXISTS session ( id SERIAL PRIMARY KEY, profile_id INTEGER NOT NULL, challenge_level INTEGER NOT NULL, is_low FLOAT NOT NULL, is_med FLOAT NOT NULL, is_high FLOAT NOT NULL, avg_coherence FLOAT NOT NULL, session_time INTEGER NOT NULL, achievement_score FLOAT NOT NULL, graph TEXT NOT NULL, date VARCHAR(255) NOT NULL, CONSTRAINT fk_profile FOREIGN KEY (profile_id) REFERENCES profile (id), CONSTRAINT check_percentages CHECK ((is_low + is_med + is_high) = 100) );");

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
        addProfile(id);
        Profile* pro = new Profile(id, 100);
        return pro;
    }

    // profile exists
    Profile* pro = new Profile(query.value(0).toInt(), query.value(1).toDouble());
    return pro;
}


bool DBManager::addProfile(int id, double batteryLvl) {

    denasDB.transaction();

    QSqlQuery query;
    query.prepare("REPLACE INTO profiles (id, battery_level) VALUES (:profile_id, :battery_level);");
    query.bindValue(":profile_id", id);
    query.bindValue(":battery_level", batteryLvl);
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


Sessions** DBManager::getProfileSessions(int id) {

    denasDB.transaction();

    QSqlQuery query;
    query.prepare("SELECT * FROM session WHERE profile_id = :profile_id;");
    query.bindValue(":profile_id", id);
    query.exec();

    if (!denasDB.commit()) {
        throw "Error: Query failed to execute";
    }

   // profile does not exist
    if (!query.next()) {
        throw "Error: Profile does not exist";
    }

    // profile exists
    Session** sessions = new Session*[query.size()];
    while (query.next()) {
        sessions[i] = new Session(
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
    return sessions;
}


Session* DBManager::getSession(int id) {

    denasDB.transaction();

    QSqlQuery query;
    query.prepare("SELECT * FROM sessions WHERE id = :session_id;");
    query.bindValue(":session", id);
    query.exec();

    if (!denasDB.commit()) {
        throw "Error: Query failed to execute";
    }

   // session does not exist
    if (!query.next()) {
        throw "Error: Session does not exist";
    }

    // session exists
    Session* session = new Session(
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
    return session;
}


bool DBManager::addSession(
    int profile_id,
    int challenge_level,
    float is_low,
    float is_med,
    float is_high,
    float avg_coherence,
    int session_time,
    float achievement_score,
    string graph,
    string date ) {

    denasDB.transaction();

    QSqlQuery query;
    query.prepare("INSERT INTO session ( profile_id, challenge_level, is_low, is_med, is_high, avg_coherence, session_time, achievement_score, graph, date ) VALUES ( :profile_id, :challenge_level, :is_low, :is_med, :is_high, :avg_coherence, :session_time, :achievement_score, :graph, :date );");
    query.bindValue(":profile_id", profile_id);
    query.bindValue(":challenge_level", challenge_level);
    query.bindValue(":is_low", is_low);
    query.bindValue(":is_med", is_med);
    query.bindValue(":is_high", is_high);
    query.bindValue(":avg_coherence", avg_coherence);
    query.bindValue(":session_time", session_time);
    query.bindValue(":achievement_score", achievement_score);
    query.bindValue(":graph", QString::fromStdString(graph));
    query.bindValue(":date", QString::fromStdString(date));
    query.exec();

    return denasDB.commit();
}


bool DBManager::deleteSession(int id) {
        denasDB.transaction();

        QSqlQuery query;
        query.prepare("DELETE FROM session WHERE id = :session_id;");
        query.bindValue(":session_id", id);
        query.exec();

        return denasDB.commit();
}


bool DBManager::deleteSessions() {
        denasDB.transaction();

        QSqlQuery query;
        query.exec("DELETE FROM session;");

        return denasDB.commit();
}

