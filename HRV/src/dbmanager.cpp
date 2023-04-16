#include "dbmanager.h"


const QString DBManager::DATABASE_PATH = "/database/hrv.db";

/**
DBManager constructor
This constructor initializes the DBManager class by creating a new instance
of the QSqlDatabase with QSQLITE driver and setting the database name to
"hrv.db". It then attempts to open the database and initialize it.

If the database cannot be opened, it throws an error message:
"Error: Database could not be opened".

If the database cannot be initialized, it throws an error message:
"Error: Database could not be initialized".
*/
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

/**
DBManager::dropTables() method
This method is responsible for dropping the existing tables in the database.
It executes two SQL queries to drop the 'log' and 'profiles' tables if they
exist.
*/
void DBManager::dropTables() {
    QSqlQuery query;
    query.exec("DROP TABLE IF EXISTS log;");
    query.exec("DROP TABLE IF EXISTS profiles;");
}

/**
DBManager::deleteLogs() method
This method is responsible for deleting all log entries from the 'log' table.

It starts a new transaction and executes an SQL query to delete all records
from the 'log' table.

The method returns 'true' if the transaction is committed successfully,
otherwise 'false'.
*/
bool DBManager::deleteLogs() {
        hrvDB.transaction();

        QSqlQuery query;
        query.exec("DELETE FROM log;");

        return hrvDB.commit();
}

/**
DBManager::addLog(Log* log) method
This method is responsible for adding a new log entry to the 'log' table
using the provided 'Log' object. It starts a new transaction and prepares an
SQL query to insert the log data into the table.

The method converts the heart rates vector to a JSON array and then to a JSON
document, which is serialized into a compact JSON string.

It binds the values from the 'Log' object to the placeholders in the query
and executes it. If the execution is successful, the transaction is committed
and the method returns 'true'.
*/
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
        hrvDB.rollback();
        return false;
    }

    return hrvDB.commit();
}

/**
DBManager::deleteLog(int id) method
This method is responsible for deleting a log entry from the 'log' table
based on the provided 'id'. It starts a new transaction and prepares an
SQL query to delete the log entry with the specified 'id'.

The method binds the 'id' to the ':log_id' placeholder in the query and
executes it. If the execution is successful, the transaction is committed
and the method returns 'true'. Otherwise, it returns 'false'.

It also outputs the 'id' of the log entry being deleted using qDebug().
*/
bool DBManager::deleteLog(int id) {
        hrvDB.transaction();

        QSqlQuery query;
        query.prepare("DELETE FROM log WHERE session_id = :log_id;");
        query.bindValue(":log_id", id);
        query.exec();
        return hrvDB.commit();
}

/**
DBManager::doesLogExist(int id) method
This method is responsible for checking if a log entry with the specified 'id'
exists in the 'log' table. It starts a new transaction and prepares an SQL
query to select the log entry with the given 'id'.

The method binds the 'id' to the ':log_id' placeholder in the query and
executes it. If the transaction fails to commit, it throws an error message:
"Error: Query failed to execute".

If the query does not return any result, the method returns 'false', indicating
that the log entry does not exist. Otherwise, it returns 'true'.
*/
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

/**
DBManager::deleteProfile(int id) method
This method is responsible for deleting a profile entry from the 'profiles'
table based on the provided 'id'. It starts a new transaction and prepares an
SQL query to delete the profile entry with the specified 'id'.

The method binds the 'id' to the ':profile_id' placeholder in the query and
executes it. If the execution is successful, the transaction is committed
and the method returns 'true'. Otherwise, it returns 'false'.
*/
bool DBManager::deleteProfile(int id) {
    hrvDB.transaction();

    QSqlQuery query;
    query.prepare("DELETE FROM profiles WHERE id = :profile_id;");
    query.bindValue(":profile_id", id);
    query.exec();

    return hrvDB.commit();
}

/**
DBManager::updateProfile(int id, double batteryLvl) method
This method is responsible for updating the battery level of a profile entry
in the 'profiles' table based on the provided 'id'. It starts a new transaction
and prepares an SQL query to update the 'battery_level' field of the profile
entry with the specified 'id'.

The method binds the 'id' and 'batteryLvl' to the ':profile_id' and ':battery_level'
placeholders in the query, respectively, and executes it.

If the execution is successful, the transaction is committed and
the method returns 'true'. Otherwise, it returns 'false'.
*/
bool DBManager::updateProfile(int id, double batteryLvl) {
    hrvDB.transaction();

    QSqlQuery query;
    query.prepare("UPDATE profiles SET battery_level = :battery_level WHERE id = :profile_id;");
    query.bindValue(":profile_id", id);
    query.bindValue(":battery_level", batteryLvl);
    query.exec();

    return hrvDB.commit();
}

/**
DBManager::addProfile(int id, double batteryLvl, int sessionsAmt) method
This method is responsible for adding or updating a profile entry in the
'profiles' table using the provided 'id' and 'batteryLvl'. It starts a new
transaction and prepares an SQL query to insert or replace the profile data
into the table.

The method binds the values 'id' and 'batteryLvl' to the placeholders in the
query and executes it. If the execution is successful, the transaction is
committed and the method returns 'true'. Otherwise, it returns 'false'.
*/
bool DBManager::addProfile(int id, double batteryLvl, int sessionsAmt) {
    hrvDB.transaction();

    QSqlQuery query;
    query.prepare("REPLACE INTO profiles (id, battery_level) VALUES (:id, :battery_level);");
    query.bindValue(":id", id);
    query.bindValue(":battery_level", batteryLvl);
    query.exec();

    return hrvDB.commit();
}



// getter methods
/**
DBManager::getLog(int id) method
This method is responsible for retrieving a log entry from the 'log' table
based on the provided 'id'. It starts a new transaction and prepares an SQL
query to select the log entry with the specified 'id'.
The method binds the 'id' to the ':id' placeholder in the query and
executes it.

It then converts the JSON string of heart rates back to a QVector<int> and
creates a new 'Log' object with the retrieved data.

The method returns a pointer to the created 'Log' object.
*/
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

/**
DBManager::getLogs() method
This method is responsible for retrieving all log entries from the 'log'
table. It starts a new transaction and executes an SQL query to select all
records from the 'log' table.

For each record, it converts the JSON string of heart rates back to a
QVector<int> and creates a new 'Log' object with the retrieved data. The
created 'Log' objects are appended to a QVector<Log*>.

The method returns a pointer to the QVector<Log*> containing all the logs.
*/
QVector<Log*>* DBManager::getLogs() {
    hrvDB.transaction();

    QSqlQuery query;
    query.exec("SELECT * FROM log;");

    QVector<Log*>* logs = new QVector<Log*>();

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
    }
    return logs;
}

/**
DBManager::getProfile(int id) method
This method is responsible for retrieving a profile entry from the 'profiles'
table based on the provided 'id'. It starts a new transaction and prepares an
SQL query to select the profile entry with the specified 'id'.

The method binds the 'id' to the ':id' placeholder in the query and
executes it. If the transaction fails to commit, it throws an error message:
"Error: Query failed to execute".

If the query does not return any result, the method creates a new profile
with the given 'id' and a battery level of 100, and adds it to the 'profiles'
table. It also creates a new 'Profile' object with the same data and an
initial log count of 1.

If the query returns a result, the method creates a new 'Profile' object
with the retrieved data and the log count obtained by calling 'getLogCount(id)'.
The method returns a pointer to the created 'Profile' object.
*/
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
        addProfile(id, 100, 0);
        Profile* pro = new Profile(id, 100, 1);
        return pro;
    }

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

/**
DBManager::DBInit() method
This method initializes the database by creating the required tables if they
do not already exist. It starts a new transaction and proceeds with the
creation of two tables:

'profiles' table with the following columns:
id (INTEGER PRIMARY KEY)
battery_level (FLOAT NOT NULL)

'log' table with the following columns:
session_id (INTEGER PRIMARY KEY)
profile_id (INTEGER NOT NULL)
challenge_level (INTEGER NOT NULL)
is_low (INTEGER NOT NULL)
is_med (INTEGER NOT NULL)
is_high (INTEGER NOT NULL)
session_time (INTEGER NOT NULL)
achievement_score (FLOAT NOT NULL)
coherence_count (INTEGER NOT NULL)
heart_rates (TEXT)
FOREIGN KEY (profile_id) REFERENCES profiles (id) ON DELETE CASCADE

The method returns 'true' if the transaction is committed successfully,
otherwise 'false'.
*/
bool DBManager::DBInit() {
    hrvDB.transaction();
    QSqlQuery query;

    //creates Profiles table
    query.exec("CREATE TABLE IF NOT EXISTS profiles ( id INTEGER PRIMARY KEY, battery_level FLOAT NOT NULL );");

    //creates Log table
    query.exec("CREATE TABLE IF NOT EXISTS log ( session_id INTEGER PRIMARY KEY, profile_id INTEGER NOT NULL, challenge_level INTEGER NOT NULL, is_low INTEGER NOT NULL, is_med INTEGER NOT NULL, is_high INTEGER NOT NULL, session_time INTEGER NOT NULL, achievement_score FLOAT NOT NULL, coherence_count INTEGER NOT NULL, heart_rates TEXT, CONSTRAINT fk_profile FOREIGN KEY (profile_id) REFERENCES profiles (id) ON DELETE CASCADE );");

    return hrvDB.commit();
}
