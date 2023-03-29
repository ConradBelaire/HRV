#include "dbmanager.h"



const QString DBManager::DATABASE_PATH = "/database/denas.db";


/*
 * Create a new DBManager to a existing denas database, defined by DBManager::DATABASE_PATH
 *
 * Throws:
 *  If the database could not be opened
 */
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
    query.exec("CREATE TABLE IF NOT EXISTS profiles ( id SERIAL PRIMARY KEY );");
    query.exec("CREATE TABLE IF NOT EXISTS session ( ( id SERIAL PRIMARY KEY, profile_id INTEGER NOT NULL, challenge_level INTEGER NOT NULL, is_low DECIMAL(5, 2) NOT NULL, is_med DECIMAL(5, 2) NOT NULL, is_high DECIMAL(5, 2) NOT NULL, avg_coherence FLOAT NOT NULL, session_time INTEGER NOT NULL, achievement_score FLOAT NOT NULL, graph INTEGER[] NOT NULL, FOREIGN KEY (profile_id) REFERENCES profile (id), CHECK (is_low + is_med + is_high = 100));");

    return denasDB.commit();
}


Profile* DBManager::getProfile(int id) {

    denasDB.transaction();

    QSqlQuery query;
    query.prepare("SELECT * FROM profiles WHERE pid=:pid");
    query.bindValue(":pid", id);
    query.exec();

    if (!denasDB.commit()) {
        throw "Error: Query failed to execute";
    }

   // profile does not exist
    if (!query.next()) {
        addProfile(id, 100.0, 1);
        Profile* pro = new Profile(id, 100, 0);
        return pro;
    }

    // profile exists
    Profile* pro = new Profile(query.value(0).toInt(), query.value(1).toDouble(), query.value(2).toInt());
    return pro;
}


QVector<Record*> DBManager::getRecordings() {

    QSqlQuery query;
    QVector<Record*> qvr;
    denasDB.transaction();

    query.prepare("SELECT name as treatment,date,power_level,duration FROM ( SELECT name as name,tid as rid FROM therapy_records UNION SELECT name as name,fid as rid FROM frequency_records ) NATURAL JOIN records ORDER BY rid;");
    query.exec();

    while (query.next()) {
        QString name = query.value(0).toString();
        QDateTime start = QDateTime::fromString(query.value(1).toString(), DATE_FORMAT);
        int power = query.value(2).toString().toInt();
        int duration = query.value(3).toString().toInt();
        Record* r = new Record(name, start, power, duration);
        qvr.push_back(r);
    }
    return qvr;
}


bool DBManager::addProfile(int id, double batteryLvl, int powerLvl) {

    denasDB.transaction();

    QSqlQuery query;
    //query.prepare("INSERT OR REPLACE INTO profiles (pid, battery_level, power_level) VALUES (:pid, :battery_level, :power_level);");
    query.prepare("REPLACE INTO profiles (pid, battery_level, power_level) VALUES (:pid, :battery_level, :power_level);");
    query.bindValue(":pid", id);
    query.bindValue(":battery_level", batteryLvl);
    query.bindValue(":power_level", powerLvl);
    query.exec();

    return denasDB.commit();
}


/*
 * Type: Public
 * Adds a therapy record to the database, if the arguments are valid.
 *
 * Parameters:
 *  therapy - the name of the therapy to add to the records, must be capitalized and in the database already
 *  time - the time the therapy started in QDateTime format
 *  powerLevel - the maximum power level observed during a therapy
 *  duration - the amount of time in seconds that the therapy was in use
 *
 * Returns:
 *  True - If the record was successfully added to the database
 *  False - If the arguments were invalid, or if the record couldn't be added to the database
 */
bool DBManager::addTherapyRecord(const QString& therapy, const QDateTime& time, int powerLevel, int duration) {

    if (!isValidRecord("therapy", time, powerLevel, duration)) {
        return false;
    }
    return addRecord("therapy", therapy, time, powerLevel, duration);
}


/*
 * Type: Public
 * Adds a frequency record to the database, if the arguments are valid.
 *
 * Parameters:
 *  frequency - the name of the frequency to add to the records, must be in the database already
 *  time - the time the frequency treatment started in QDateTime format
 *  powerLevel - the maximum power level observed during a frequency treatment
 *  duration - the amount of time in seconds that the frequency treatment was in use
 *
 * Returns:
 *  True - If the record was successfully added to the database
 *  False - If the arguments were invalid, or if the record couldn't be added to the database
 */
bool DBManager::addFrequencyRecord(const QString& frequency, const QDateTime& time, int powerLevel, int duration) {
    if (!isValidRecord("frequency", time, powerLevel, duration)) {
        return false;
    }
    return addRecord("frequency", frequency, time, powerLevel, duration);
}


/*
 * Type: Public
 * Deletes all records from the database.
 *
 * Returns:
 *  True - If the records were successfully removed from the database
 *  False - If the records couldn't be deleted from the database
 */
bool DBManager::deleteRecords() {

    QSqlQuery query;
    query.prepare("DELETE FROM records");
    query.exec();
    query.prepare("DELETE FROM therapy_records");
    query.exec();
    query.prepare("DELETE FROM frequency_records");
    return query.exec();
}


QList<QString>* DBManager::frequencies() {

    QList<QString>* frequencies = new QList<QString>;

    QSqlQuery query;
    query.exec("SELECT * FROM FREQUENCIES");
    while (query.next()) {
        frequencies->append(query.value(0).toString());
    }
    return frequencies;
}


QList<Therapy*>* DBManager::therapies() {

    QList<Therapy*>* therapies = new QList<Therapy*>;

    QSqlQuery query;
    query.exec("SELECT * FROM therapies");
    while (query.next()) {
        Therapy* therapy = new Therapy(query.value(0).toString(), query.value(2).toInt(), query.value(1).toString());
        therapies->append(therapy);
    }
    return therapies;
}


/*
 * Type: Private
 * Validates the given parameters.
 *
 * Parameters:
 *  recordType - The type of record, used only for debugging
 *  time - the time the treatment started in QDateTime format
 *  powerLevel - the maximum power level observed during a treatment
 *  duration - the amount of time in milliseconds that the treatment was in use
 *
 * Returns:
 *  True - If the parameters are acceptable
 *  False - Otherwise
 *
 * See Also:
 *  DBManager::addTherapyRecord
 *  DBManager::addFrequencyRecord
 */
bool DBManager::isValidRecord(const QString& recordType, const QDateTime& time, int powerLevel, int duration) {

    bool valid = true;
    if (!time.isValid()) {
        qDebug() << "Error: Cannot add " << recordType << " record, time is not valid";
        valid = false;
    }
    else if (powerLevel < 0 || powerLevel > 100) {
        qDebug() << "Error: Cannot add " << recordType << " record, power level is not valid";
        valid = false;
    }
    else if (duration < 0) {
        qDebug() << "Error: Cannot add " << recordType << " record, duration is not valid";
        valid = false;
    }
    return valid;
}


/*
 * Type: Private
 * Adds a treatment record to the database.
 *
 * Parameters:
 *  tableName - The database table to insert this record into
 *  name - The name of the frequency or therapy
 *  time - the time the treatment started in QDateTime format
 *  powerLevel - the maximum power level observed during a treatment
 *  duration - the amount of time in milliseconds that the treatment was in use
 *
 * Returns:
 *  True - If the record was added successfully
 *  False - Otherwise
 *
 * See Also:
 *  DBManager::addTherapyRecord
 *  DBManager::addFrequencyRecord
 */
bool DBManager::addRecord(const QString& tableName, const QString& name, const QDateTime& time, int powerLevel, int duration) {

    denasDB.transaction();

    QSqlQuery query;
    query.prepare("INSERT INTO records (date, power_level, duration) VALUES (:date, :power_level, :duration);");
    query.bindValue(":date", time.toString(DATE_FORMAT));
    query.bindValue(":power_level", powerLevel);
    query.bindValue(":duration", duration);
    query.exec();

    int rowid = query.lastInsertId().toInt();
    query.prepare("INSERT INTO " + tableName + "_records VALUES (:name, :id);");
    query.bindValue(":name", name);
    query.bindValue(":id", rowid);
    query.exec();

    return denasDB.commit();
}
