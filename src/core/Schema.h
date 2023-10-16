//
// Created by dts on 2021/6/6.
//

#ifndef TIANCHI_CPP_SCHEMA_H
#define TIANCHI_CPP_SCHEMA_H

#include "core/Table.h"

class Schema {
public:
    const string &getDatabaseName() const;

    void setDatabaseName(const string &databaseName);

    const unordered_map<string, Table> &getTableMap() const;

    void setTableMap(const unordered_map<string, Table> &tableMap);

    void add_table(const Table &table);

    const Table &get_table(const string &tb_name);

private:
    string database_name;
    unordered_map<string, Table> table_map;
};

#endif //TIANCHI_CPP_SCHEMA_H
