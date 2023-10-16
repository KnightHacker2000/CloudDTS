//
// Created by dts on 2021/6/6.
//

#ifndef TIANCHI_CPP_TABLE_H
#define TIANCHI_CPP_TABLE_H

#include "Column.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Index.h"

using namespace std;

class Table {
private:
    string database_name;
    string table_name;

    vector<Column> columns;
//    unordered_map<string, Column> columns;
    //not useful
    unordered_map<string, Index> indexes;

    // first: pk_name, second: ordinal
    unordered_map<string, int> pks;

public:
    const string &getDatabaseName() const {
        return database_name;
    }

    void setDatabaseName(const string &databaseName) {
        database_name = databaseName;
    }

    const string &getTableName() const {
        return table_name;
    }

    void setTableName(const string &tableName) {
        table_name = tableName;
    }


    void addColumn(const Column &column) {
        columns.push_back(column);
    }

    void addIndex(const Index &index) {
        indexes.insert({index.getIndexName(), index});
    }

    void add_pk_name(const string &pk, const int ordinal) {
        pks.insert({pk, ordinal});
    }

    unordered_map<string, int> get_pk_names() {
        return pks;
    }

    void reset() {
        database_name.clear();
        table_name.clear();
        columns.clear();
        indexes.clear();
        pks.clear();
    }

    vector<Column> get_columns() {
        return columns;
    }

    Column get_column(int ordinal) {
        if (ordinal <= 0 || ordinal > columns.size()) {
            cout << "ordinal error." << endl;
        }
        return columns.at(ordinal);
    }

    void toString() {
        cout << database_name << "." << table_name << ", total: " << columns.size() << " column, columns: [";
        int count = 1;
        for (auto column : columns) {
            cout << column.getColumnName() << "(" << column.getOrdinal() << ")";
            if (count++ != columns.size()) {
                cout << ", ";
            }
        }
        cout << "], ";
        for (auto pk : pks) {
            cout << pk.first << "(" << pk.second << "), ";
        }
        cout << endl;
    }
};


#endif //TIANCHI_CPP_TABLE_H
