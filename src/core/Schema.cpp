//
// Created by Frank Li on 6/22/21.
//

#include "core/Schema.h"

const string& Schema::getDatabaseName() const {
    return database_name;
}

void Schema::setDatabaseName(const string &databaseName) {
    database_name = databaseName;
}

const unordered_map<string, Table> &Schema::getTableMap() const {
    return table_map;
}

void Schema::setTableMap(const unordered_map<string, Table> &tableMap) {
    table_map = tableMap;
}

void Schema::add_table(const Table &table) {
    table_map.insert({table.getTableName(), table});
}

const Table &Schema::get_table(const string &tb_name) {
    return table_map.at(tb_name);
}

