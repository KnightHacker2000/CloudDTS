//
// Created by dts on 2021/6/6.
//

#ifndef TIANCHI_CPP_TABLE_H
#define TIANCHI_CPP_TABLE_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "core/Column.h"
#include "core/Index.h"

using namespace std;

class Table {
public:
    const string &getDatabaseName() const;

    void setDatabaseName(const string &databaseName);

    const string &getTableName() const;

    void setTableName(const string &tableName);

    void addColumn(const Column &column);

    void addIndex(const Index &index);

    void add_pk_name(const string &pk, const int ordinal);

    unordered_map<string, int> get_pk_names();

    void reset();

    vector<Column> get_columns();

    Column get_column(int ordinal);

    void toString();

private:
    string database_name;
    string table_name;

    vector<Column> columns;
//    unordered_map<string, Column> columns;
    //not useful
    unordered_map<string, Index> indexes;

    // first: pk_name, second: ordinal
    unordered_map<string, int> pks;

};


#endif //TIANCHI_CPP_TABLE_H
