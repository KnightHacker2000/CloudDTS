//
// Created by Frank Li on 6/22/21.
//

#include "core/Record.h"

#include <iostream>

using namespace std;

Record::Record() {

}

Record::Record(string tb_name) {
    table_name = tb_name;
}

Record::Record(string db_name, string tb_name) {
    database_name = db_name;
    table_name = tb_name;
}

void Record::set_opeartion(string &op_type) {
    operation.set_operation_type(op_type);
}

Operation Record::get_operation() {
    return operation;
}

void Record::add_pk_value(string &pk_value, int ordinal) {
    pk_values[ordinal] = pk_value;
}

void Record::set_pk_values(const vector<string> &pk_values) {
    this->pk_values = pk_values;
}

void Record::setTable(const Table &table) {
    Record::table = table;
}

bool Record::operator<(const Record &other) const {
    vector<string> other_pk_values = other.pk_values;
    if (pk_values.size() != other_pk_values.size()) {
        cout << "pk values size error." << endl;
    }

    for (int i = 0; i <= pk_values.size() - 1; i++) {
        long num = atol(pk_values[i].c_str());
        long other_num = atol(other_pk_values[i].c_str());
        if (num != other_num) {
            return num < other_num;
        }
    }
    return false;
}

void Record::add_column(string col_name, string value) {
    after_img.insert({col_name, value});
}

void Record::toString() {
    vector<Column> columns = table.get_columns();
    cout << database_name << "." << table_name << ", [";
    int i = 0;
    for (auto column : columns) {
        cout << "(" << column.getColumnName() << ": " << after_img.at(column.getColumnName()) << ")";
        if (i++ != after_img.size() - 1) {
            cout << ", ";
        }
    }
    cout << endl;
}

void Record::to_file(string &record_str) {
    vector<Column> columns = table.get_columns();
    int i = 0;
    for (auto column : columns) {
        record_str += after_img.at(column.getColumnName());
        if (i++ != columns.size() - 1) {
            record_str += "\t";
        }
    }
}