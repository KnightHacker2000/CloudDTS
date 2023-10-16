//
// Created by dts on 2021/6/6.
//

#ifndef TIANCHI_CPP_RECORD_H
#define TIANCHI_CPP_RECORD_H

#include <vector>
#include <string>

#include "core/Operation.h"
#include "core/Table.h"

class Record {
public :
    Record();

    Record(string tb_name);

    Record(string db_name, string tb_name);

    void set_opeartion(string &op_type);

    Operation get_operation();

    void add_pk_value(string &pk_value, int ordinal);

    void set_pk_values(const vector<string> &pk_values);

    void setTable(const Table &table);

    /**
     * Compare two records (for global sorting) based on primary keys' value
     * @param other
     * @return
     */
    bool operator<(const Record &other) const;

    void add_column(string col_name, string value);

    void toString();

    void to_file(string &record_str);

private:
    string database_name;
    string table_name;

    //first: column_name, second: value
    unordered_map<string, string> before_img;
    unordered_map<string, string> after_img;

    Operation operation;
    vector<string> pk_values;

    Table table;

};

#endif //TIANCHI_CPP_RECORD_H
