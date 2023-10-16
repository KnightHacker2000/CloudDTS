//
// Created by dts on 2021/6/6.
//

#ifndef TIANCHI_CPP_RECORD_H
#define TIANCHI_CPP_RECORD_H

#include <vector>
#include <string>

#include "Operation.h"
#include "Table.h"

class Record {
public :
    Record() {}

    Record(string tb_name) {
        table_name = tb_name;
    }

    Record(string db_name, string tb_name) {
        database_name = db_name;
        table_name = tb_name;
    }

private:
    string database_name;
    string table_name;

    //first: column_name, second: value
    unordered_map<string, string> before_img;
    unordered_map<string, string> after_img;

    Operation operation;
    vector<string> pk_values;

    Table table;
public:
    void set_opeartion(string& op_type) {
        operation.set_operation_type(op_type);
    }

    Operation get_operation() {
        return operation;
    }

    void add_pk_value(string& pk_value, int ordinal) {
        pk_values[ordinal] = pk_value;
    }

    void set_pk_values(const vector<string>& pk_values) {
        this->pk_values = pk_values;
    }

public:
    void setTable(const Table &table) {
        Record::table = table;
    }

public:
    /**
     * Compare two records (for global sorting) based on primary keys' value
     * @param other
     * @return
     */
    bool operator < (const Record& other) const {
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

    void add_column(string col_name, string value) {
        after_img.insert({col_name, value});
    }

    void toString() {
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

    void to_file(string& record_str) {
        vector<Column> columns = table.get_columns();
        int i = 0;
        for (auto column : columns) {
            record_str += after_img.at(column.getColumnName());
            if (i++ != columns.size() - 1) {
                record_str += "\t";
            }
        }
    }
};
#endif //TIANCHI_CPP_RECORD_H
