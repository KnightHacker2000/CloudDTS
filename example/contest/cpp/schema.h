//
// Created by dts on 2021/6/6.
//

#ifndef TIANCHI_CPP_SCHEMA_H
#define TIANCHI_CPP_SCHEMA_H
#include "Table.h"

class Schema {
private:
    string database_name;
    unordered_map<string, Table> table_map;
public:
    const string &getDatabaseName() const {
        return database_name;
    }

    void setDatabaseName(const string &databaseName) {
        database_name = databaseName;
    }

    const unordered_map <string, Table> &getTableMap() const {
        return table_map;
    }

    void setTableMap(const unordered_map <string, Table> &tableMap) {
        table_map = tableMap;
    }

    void add_table(const Table& table) {
        table_map.insert({table.getTableName(), table});
    }

    const Table& get_table(const string& tb_name) {
        return table_map.at(tb_name);
    }
};
#endif //TIANCHI_CPP_SCHEMA_H
