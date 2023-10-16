//
// Created by dts on 2021/6/6.
//

#ifndef TIANCHI_CPP_COLUMN_H
#define TIANCHI_CPP_COLUMN_H

#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class Column {
public:

private:
    string column_name;
    int ordinal;
    bool is_unsigned;
    string charset;
    string column_def;
    int length;
    int precision;
    int scale;
public:
    Column(string column_name, int ordinal, bool is_unsigned, string charset, string column_def, int length,
           int precision, int scale);

    Column(const unordered_map<string, string> &map);

    bool operator==(const Column &other) const;

    bool operator<(const Column &other) const;

    size_t operator()(const Column &column) const;

    const string &getColumnName() const;

    void setColumnName(const string &columnName);

    int getOrdinal() const;

    void setOrdinal(int ordinal);

    bool isUnsigned() const;

    void setIsUnsigned(bool isUnsigned);

    const string &getCharset() const;

    void setCharset(const string &charset);

    const string &getColumnDef() const;

    void setColumnDef(const string &columnDef);

    int getLength() const;

    void setLength(int length);

    int getPrecision() const;

    void setPrecision(int precision);

    int getScale() const;

    void setScale(int scale);
};

#endif //TIANCHI_CPP_COLUMN_H
