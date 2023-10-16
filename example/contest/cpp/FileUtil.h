//
// Created by dts on 2021/6/6.
//

#ifndef TIANCHI_CPP_FILEUTIL_H
#define TIANCHI_CPP_FILEUTIL_H

#include <unordered_map>
#include <string>
#include <stdlib.h>
#include <vector>
#include <set>
#include <stdlib.h>
#include <cstdint>
#include <sstream>
#include <cstring>

#include "Table.h"
#include "Index.h"
#include "Column.h"
#include "Record.h"
#include "Schema.h"

#include <glog/logging.h>

using namespace std;
enum State {
    LOAD_DB_TB_NAME,
    LOAD_COLUMN_NUM,
    LOAD_COLUMNS,
    LOAD_INDEX_NUM,
    LOAD_INDEXES,
    LOAD_PK_NUM,
    LOAD_PKS
};

static State state = State::LOAD_DB_TB_NAME;

static int column_num;
static int index_num;
static int pk_num;

static int pk_ordinal;
static Schema schema;

// Data cleaning
static const string char_tag = "char";
static const string datetime_tag = "datetime";
static const string decimal_tag = "decimal";
static const string int_tag = "int";

// Datetime cleaning related
static const string correct_datetime = "2020-04-01 00:00:00.0";
static const uint32_t year_digits = 4;
static const uint32_t month_digits = 2;
static const uint32_t day_digits = 2;
static const uint32_t hour_digits = 2;
static const uint32_t minute_digits = 2;
static const uint32_t second_digits = 4;            // Format: xx.x (4 digits)
static const vector <uint32_t> days_of_months = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


class FileUtil {
private:

public:
    static void setSchema(const Schema &s) {
        schema = s;
    }

private:
    static void handle_db_table(unordered_map <string, string> ret, Table &table) {
//        cout << ret.begin()->first << "." << ret.begin()->second << endl;
        table.setDatabaseName(ret.begin()->first);
        table.setTableName(ret.begin()->second);
    }

    static void handle_columns(unordered_map <string, string> ret, Table &table) {
        Column column(ret);
        table.addColumn(column);
    }

    /**
     * Add 'Indexes' to Schema (currently disabled)
     * @param ret (in) pair of <key, value>
     * @param table (out)
     */
    static void hand_indexes(unordered_map <string, string> ret, Table &table) {
//        not use
//        Index index(ret);
//        table.addIndex(index);
    }

    /**
     * add pk and its ordinal
     * @param ret
     * @param table
     */
    static void handle_pks(unordered_map <string, string> ret, Table &table) {
        string pk(ret["IndexCols"]);
        table.add_pk_name(pk, pk_ordinal++);
    }

public:
    static State get_state() {
        return state;
    }

    /**
     *
     * @param line
     * @param table
     * @return true: finish one table, false: in process
     */
    static bool deserialize_schema(string &line, Table &table) {
        State old_state = state;
        unordered_map <string, string> ret = _deserialize_schema_with_state(line);
        switch (old_state) {
            case LOAD_DB_TB_NAME:
                pk_ordinal = 0;
                handle_db_table(ret, table);
                break;
            case LOAD_COLUMN_NUM:
                break;
            case LOAD_COLUMNS:
                handle_columns(ret, table);
                break;
            case LOAD_INDEX_NUM:
                break;
            case LOAD_INDEXES:
                hand_indexes(ret, table);
                break;
            case LOAD_PK_NUM:
                break;
            case LOAD_PKS:
                handle_pks(ret, table);
                break;
            default:
                break;
        }
        if (state == LOAD_DB_TB_NAME) {
            return true;
        } else {
            return false;
        }
    }

    /**
     *
     * @param line
     * @return
     */
    static unordered_map <string, string> _deserialize_schema_with_state(string &line) {
        switch (state) {
            case LOAD_DB_TB_NAME:
                return load_db_table(line);
            case LOAD_COLUMN_NUM:
                return load_tag_num(line, "COLUMN_NUM");
            case LOAD_COLUMNS:
                return load_json(line, LOAD_COLUMNS);
            case LOAD_INDEX_NUM:
                return load_tag_num(line, "INDEX_NUM");
            case LOAD_INDEXES:
                return load_json(line, LOAD_INDEXES);
            case LOAD_PK_NUM:
                return load_tag_num(line, "PRIMARY_KEY_NUM");
            case LOAD_PKS:
                return load_json(line, LOAD_PKS);
            default:
                break;
        }
        return unordered_map<string, string>();
    }

    //[DATABASE] tianchi_dts_data [TABLE] item
    //first: db_name, second: tb_name
    static unordered_map <string, string> load_db_table(const string &line) {
        column_num = 0;
        index_num = 0;
        pk_num = 0;
        unordered_map <string, string> db_tb_name;
        int db_start = -1;
        int db_end = -1;
        int tb_start = -1;
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == ' ') {
                if (db_start == -1) {
                    db_start = i + 1;
                } else if (db_end == -1) {
                    db_end = i - 1;
                } else if (tb_start == -1) {
                    tb_start = i + 1;
                }
            }
        }
        string db_name = line.substr(db_start, db_end - db_start + 1);
        string tb_name = line.substr(tb_start);
        db_tb_name[db_name] = tb_name;
        state = LOAD_COLUMN_NUM;
        return db_tb_name;
    }

    // 3 situations:
    // COLUMN NUMBER 5
    // first(fixed): COLUMN_NUM, second: num

    // INDEX NUMBER 1
    // first(fixed): INDEX_NUMBER, second: num

    // PRIMARY KEY NUMBER 1
    // first(fixed): PRIMARY_KEY_NUMBER, second: num
    static unordered_map <string, string> load_tag_num(const string &line, const string tag) {
        unordered_map <string, string> tag_name_num;
        int num_start = 0;
        for (int i = line.size() - 1; i >= 0; i--) {
            if (line[i] == ' ') {
                num_start = i + 1;
                break;
            }
        }
        tag_name_num[tag] = line.substr(num_start);
        //COLUMN_NUM / INDEX_NUM / PRIMARY_KEY_NUM
        // TODO: why these are static variables but not returned in the second element of the returned map (currently return the postion of the num in the line)
        if (tag[0] == 'C') {
            column_num = atoi(line.substr(num_start).c_str());
            state = LOAD_COLUMNS;
        } else if (tag[0] == 'I') {

            index_num = atoi(line.substr(num_start).c_str());
            state = LOAD_INDEXES;
        } else if (tag[0] == 'P') {
            pk_num = atoi(line.substr(num_start).c_str());
            state = LOAD_PKS;
        }
        return tag_name_num;
    }

    /**
     * load json
     * @param line (in) raw line data
     * @param cur_state
     * @return
     */
    // {"Name":"i_id","Ordinal":1,"Unsigned":false,"CharSet":null,"ColumnDef":"int(11)","Length":null,"Precision":10,"Scale":0}
    // first: name, second: value
    static unordered_map <string, string> load_json(string &line, State cur_state) {
        unordered_map <string, string> ret;
        if (line.empty()) {
            return ret;
        }
        switch (cur_state) {
            case LOAD_COLUMNS:
                // When there's only one column, load index instead.
                if (--column_num == 0) {
                    state = LOAD_INDEX_NUM;
                }
                break;
            case LOAD_INDEXES:
                // When there's only one index, ...
                if (--index_num == 0) {
                    state = LOAD_PK_NUM;
                }
                break;
            case LOAD_PKS:
                // When there's only one primary key, ...
                if (--pk_num == 0) {
                    state = LOAD_DB_TB_NAME;
                }
                break;
            default:
                break;
        }
        // add comma for convenience
        // TODO: remove this comma (string copy constructor), use std::string::find
        line += ",\"";
        int comma_position = -1;
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == ',' && (line[i - 1] == '\"' || line[i + 1] == '\"')) {
                string pair_string = line.substr(comma_position + 1, i - 1 - (comma_position + 1) + 1);
                for (int j = 0; j < pair_string.size(); j++) {
                    // TODO: use std::string::find
                    int colon_position = -1;
                    if (pair_string[j] == ':') {
                        colon_position = j;
                        string dirty_first = pair_string.substr(0, colon_position);
                        string dirty_second = pair_string.substr(colon_position + 1);

                        string first = clean_string(dirty_first);
                        string second = clean_string(dirty_second);
                        ret.insert({first, second});
                    }
                }
                comma_position = i;
            }
        }
        return ret;
    }

    /**
     * Split a line in source file to vector of values
     * @param records (out) pair of <tb_name, records of this tb>
     * @param line (in) raw line data read from source file
     */
    // I	tianchi_dts_data	orders	5	1	1	1517	2021-04-11 15:41:55.0	9	6	1
    static void load_source_data(unordered_map <string, set<Record>> &records, const string &line) {
        vector <string> split_line;
        split_line.reserve(line.size());
        int pre_tab_position = -1;
        int size = line.size();
        for (int i = 0; i < size; i++) {
            // TODO: use std::string::find
            if (line[i] == '\t') {
//                cout << line.substr(pre_tab_position + 1, (i - 1 - pre_tab_position)) << ", ";
                split_line.push_back(line.substr(pre_tab_position + 1, (i - 1 - pre_tab_position)));
                pre_tab_position = i;
            }
        }
        split_line.push_back(line.substr(pre_tab_position + 1, (size - 1 - pre_tab_position)));
//        cout << line.substr(pre_tab_position + 1, (size - 1 - pre_tab_position)) << endl;
        handle_split_line(records, split_line);
    }

private:

    static string clean_string_with_char(const string &dirty_string, const char left, const char right) {
        //start == -1 means not meet first wanted character
        int start = -1;
        int end = dirty_string.size() - 1;
        for (int i = 0; i < dirty_string.size(); i++) {
            if (start == -1 && (dirty_string[i] == '\"' || dirty_string[i] == '{' || dirty_string[i] == '['
                                || dirty_string[i] == ' ' || dirty_string[i] == left)) {
                continue;
            } else if (start != -1 && (dirty_string[i] == '\"' || dirty_string[i] == '}' || dirty_string[i] == ']'
                                       || dirty_string[i] == ' ' || dirty_string[i] == right)) {
                end = i - 1;
                break;
            }
            if (start == -1) {
                start = i;
            }
        }
        string ret = dirty_string.substr(start, end - start + 1);
        return ret;
    }

    static string clean_string(const string &dirty_string) {
        return clean_string_with_char(dirty_string, ' ', ' ');
    }

    /**
     * Construct Record instance from a vector of elements.
     * @param records （out) pair of <tb_name, set of Records>
     * @param split_line (in) vectors of split values
     */
    // TODO: Subscription 0 is the operation type (in first stage of contest, it's only 'I')
    static void handle_split_line(unordered_map <string, set<Record>> &records, const vector <string> &split_line) {
        if (split_line.size() <= 3) {
            cout << "Error line." << endl;
        }

        string operation_type = split_line[0];
        string db_name = split_line[1];
        string tb_name = split_line[2];

        Table table = schema.get_table(tb_name);
        // pks: <pk col name, pk ordinal>
        unordered_map<string, int> pks = table.get_pk_names();
        vector <string> pk_values;
        pk_values.resize(pks.size());
        vector <Column> columns = table.get_columns();

        // Create Record instance, set op_type, db_name, tb_name
        Record record(db_name, tb_name);
        record.set_opeartion(operation_type);
        record.setTable(table);

        // Set columns
        for (int i = 0; i < columns.size(); i++) {
            string col_name = columns[i].getColumnName();
            string value = split_line[3 + i];

            // get pks info to sort the record
            //
            if (pks.find(col_name) != pks.end()) {
                int index = pks[col_name];
                pk_values[index] = value;
            }

            // Todo: optimize clean data
            clean_data(columns[i], value);

            // TODO: currently only add to after_img (op_type I)
            record.add_column(col_name, value);
        }

        record.set_pk_values(pk_values);

        // first insert
        if (records.find(tb_name) == records.end()) {
            set <Record> record_set;
            record_set.insert(record);
            records.insert({tb_name, record_set});
        } else {
            records.at(tb_name).insert(record);
        }
    }

    static void
    split_string(std::vector <std::string> &split_strs, std::string &src, char delimiter) {
        int prev_pos = -1;
        int curr_pos = -1;

        while ((curr_pos = src.find(delimiter, prev_pos + 1)) != string::npos) {
            split_strs.emplace_back(src.substr(prev_pos + 1, curr_pos - prev_pos - 1));
            prev_pos = curr_pos;
        }
        split_strs.emplace_back(src.substr(prev_pos + 1, src.size()));
    }

    static bool is_numeric(std::string& src) {
        for (int i = 0; i < src.size(); ++i) {
            if (!isdigit(src[i]))
                return false;
        }
        return true;
    }

    //1) Exceed char length data;
    //2) Error date time type data;
    //3) Error decimal type data;
    //4) Error data type.
    static void clean_data(Column column, string &value) {
        string column_def = column.getColumnDef();
        int char_index = -1;
        int decimal_index = -1;

        if ((char_index = column_def.find(char_tag)) != string::npos) {
            // 1) Exceed char length data;
            clean_char(column_def, char_index, value);
        } else if (strcmp(column_def.c_str(), datetime_tag.c_str()) == 0) {
            // 2) Error date time type data;
            clean_datetime(value);
        } else {
            if ((decimal_index = column_def.find(decimal_tag)) != string::npos) {
                // 3) Error decimal type data;
                clean_decimal(column_def, decimal_index, value);
            }

            if (column_def.find(int_tag) != string::npos || column_def.find(decimal_tag) != string::npos) {
                // 4) Error data type.
                clean_data_type(value);
            }
        }

    }

    static void clean_char(string &column_def, int char_index, string &value) {
        // Get char width from schema
        string char_num_s = clean_string_with_char(column_def.substr(char_index + char_tag.size()), '(', ')');
        int char_num = atoi(char_num_s.c_str());
        if (value.size() > char_num) {
            value = value.substr(0, char_num);
        }
    }

    // TODO: merge some operations together to try to use only one or two iterations. (current ops include: split strings, stoi, stod, check each digit)
    static void clean_datetime(string &value) {
        try {
            // TODO: use functions to convert string to integer without throwing exceptions. (candidates: sstream.good(), strtod)
            int daytime_pos = value.find(' ');
            if (daytime_pos != string::npos) {
                string date_str = value.substr(0, daytime_pos);
                string daytime_str = value.substr(daytime_pos + 1, value.size());

                // Check date
                std::vector <std::string> date_split_strs;
                FileUtil::split_string(date_split_strs, date_str, '-');
                if (date_split_strs.size() != 3) {
                    VLOG(4) << "date component number is wrong, number=" << date_split_strs.size();
                    goto datetime_error;
                } else {
                    std::string year_str = date_split_strs[0];
                    std::string month_str = date_split_strs[1];
                    std::string day_str = date_split_strs[2];

                    // Check year (don't know if negative means B.C.)
                    // TODO: stoi succeed doesn't guarantee that the string consists of pure numerical values, e.g., 1g will be converted to 1 (same for 1e 1f and maybe more combination). Hence, we currently check each bit
                    int year = stoi(year_str);
                    if (year_str.size() != year_digits || !is_numeric(year_str) || year < 0) {
                        VLOG(3) << "year error, year=" << year_str << "\tvalue=" << value;
                        goto datetime_error;
                    }

                    // Check month
                    int month = stoi(month_str);
                    if (month_str.size() != month_digits || !is_numeric(month_str) || month < 0 || month > 12) {
                        VLOG(4) << "month error, month=" << month_str;
                        goto datetime_error;
                    }

                    // Check day
                    int day_of_month = stoi(day_str);
                    if (day_str.size() != day_digits || !is_numeric(day_str) || day_of_month < 0) {
                        VLOG(4) << "day error, day=" << day_str;
                        goto datetime_error;
                    } else {
                        int max_day_of_month;
                        if (month == 2) {
                            if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
                                max_day_of_month = 29;
                            else
                                max_day_of_month = 28;
                        } else {
                            max_day_of_month = days_of_months[month - 1];
                        }

                        if (day_of_month > max_day_of_month) {
                            VLOG(4) << "day error, day=" << day_str << ", month=" << month;
                            goto datetime_error;
                        }
                    }
                }

                // Check time
                std::vector <std::string> daytime_split_strs;
                FileUtil::split_string(daytime_split_strs, daytime_str, ':');
                if (daytime_split_strs.size() != 3) {
                    VLOG(4) << "daytime component number is wrong, number=" << daytime_split_strs.size();
                    goto datetime_error;
                } else {
                    std::string hour_str = daytime_split_strs[0];
                    std::string minute_str = daytime_split_strs[1];
                    std::string second_str = daytime_split_strs[2];

                    // Check hour
                    int hour = stoi(hour_str);
                    if (hour_str.size() != hour_digits || !is_numeric(hour_str) || hour < 0 || hour > 24) {
                        VLOG(4) << "hour error, hour=" << hour_str;
                        goto datetime_error;
                    }

                    // Check minute
                    int minute = stoi(minute_str);
                    if (minute_str.size() != minute_digits || !is_numeric(minute_str) || minute < 0 || minute >= 60) {
                        VLOG(4) << "minute error, minute=" << minute_str;
                        goto datetime_error;
                    }

                    // Check second
                    if (!isdigit(second_str[0]) || !isdigit(second_str[1]) || second_str[2] != '.' || !isdigit(second_str[3]))
                        goto datetime_error;
                    double second = stod(second_str);
                    if (second_str.size() != second_digits || second < 0 || second >= 60.0) {
                        VLOG(4) << "second error, second=" << second_str;
                        goto datetime_error;
                    }
                }
            } else {
                if (value != "null")
                    VLOG(4) << "datetime error, no date and daytime space delimiter";
                goto datetime_error;
            }
        } catch (std::invalid_argument e) {
            VLOG(4) << "Fail to convert datetime string component to numerics, e.what()=" << e.what();
            goto datetime_error;
        }

        return;

        // Wrong datetime format, reset to default datetime
        datetime_error:
        value = correct_datetime;
    }

    static void clean_decimal(string &column_def, int decimal_index, string &value) {
        // Get rid off the brackets '(' and ')' around the precision after type
        string decimal_str = clean_string_with_char(column_def.substr(decimal_index + decimal_tag.size()), '(',
                                                    ')');
        int comma = decimal_str.find(',');
        // Floating point scale (the max number of decimals)
        int f_num = atoi(decimal_str.substr(comma + 1).c_str());
        int period = value.find(".");
        if (period != string::npos) {
            if (value.size() - period - 1 > f_num) {
                // If there are more decimals than the scale
                if (value[period + f_num + 1] < '5') {
                    value = value.substr(0, period + f_num + 1);
                } else {
                    value = value.substr(0, period + f_num + 1);
                    // Convert string to float，and add 1 to the last digit
                    float f = atof(value.c_str());
                    bool negative = f < 0.0f;

                    // Carry
                    float delta = 1.0f;
                    int f_num_ = f_num;
                    while (f_num_--) {
                        delta /= 10.0f;
                    }
                    if (!negative) {
                        f += delta;
                    } else {
                        f -= delta;
                    }
                    ostringstream buffer;
                    buffer << f;
                    value = buffer.str();
                    int count = (period + f_num + 1) - value.size();
                    // Zero padding, (float)3.40 -> (string)3.4 -> (string)3.40(补上)
                    //special case: 34.0 -> 34 -> 34.0
                    if (count > 0) {
                        if (value.find('.') == string::npos) {
                            count--;
                            value += '.';
                        }
                        while (count-- > 0) {
                            value += '0';
                        }
                    }
                }
                return;
            }
        }
    }

    static void clean_data_type(string &value) {
        for (int i = 0; i < value.size(); i++) {
            if (value[i] == '-' || value[i] == ' ' || value[i] == ':' || value[i] == '.' ||
                (value[i] >= '0' && value[i] <= '9')) {
                continue;
            } else {
                value = "0";
                return;
            }
        }
    }

};

#endif //TIANCHI_CPP_FILEUTIL_H
