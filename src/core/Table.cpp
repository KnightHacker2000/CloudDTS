//
// Created by Frank Li on 6/22/21.
//

#include "core/Table.h"

#include <iostream>

using namespace std;

const string &Table::getDatabaseName() const {
    return database_name;
}

void Table::setDatabaseName(const string &databaseName) {
    database_name = databaseName;
}

const string &Table::getTableName() const {
    return table_name;
}

void Table::setTableName(const string &tableName) {
    table_name = tableName;
}

void Table::addColumn(const Column &column) {
    columns.push_back(column);
}

void Table::addIndex(const Index &index) {
    indexes.insert({index.getIndexName(), index});
}

void Table::add_pk_name(const string &pk, const int ordinal) {
    pks.insert({pk, ordinal});
}

unordered_map<string, int> Table::get_pk_names() {
    return pks;
}

void Table::reset() {
    database_name.clear();
    table_name.clear();
    columns.clear();
    indexes.clear();
    pks.clear();
}

vector<Column> Table::get_columns() {
    return columns;
}

Column Table::get_column(int ordinal) {
    if (ordinal <= 0 || ordinal > columns.size()) {
        cout << "ordinal error." << endl;
    }
    return columns.at(ordinal);
}

void Table::toString() {
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