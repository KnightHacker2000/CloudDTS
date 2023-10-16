//
// Created by Frank Li on 6/22/21.
//

#include <string>

#include "core/Column.h"

using namespace std;

Column::Column(string column_name, int ordinal, bool is_unsigned, string charset, string column_def, int length,
               int precision, int scale) {
    this->column_name = column_name;
    this->ordinal = ordinal;
    this->is_unsigned = is_unsigned;
    this->charset = charset;
    this->column_def = column_def;
    this->length = length;
    this->precision = precision;
    this->scale = scale;
}

Column::Column(const unordered_map<string, string> &map) {
    this->column_name = map.at("Name");
    this->ordinal = atoi(map.at("Ordinal").c_str());
    this->is_unsigned = (map.at("Unsigned")[0] == 't') ? true : false;
    this->charset = map.at("CharSet");
    this->column_def = map.at("ColumnDef");
    this->length = atoi(map.at("Length").c_str());
    this->precision = atoi(map.at("Precision").c_str());
    this->scale = atoi(map.at("Scale").c_str());
}

bool Column::operator==(const Column &other) const {
    return this->column_name == other.column_name;
}

bool Column::operator<(const Column &other) const {
    return this->column_name < other.column_name;
}

size_t Column::operator()(const Column &column) const {
    return hash<string>()(column.column_name);
}

const string &Column::getColumnName() const {
    return column_name;
}

void Column::setColumnName(const string &columnName) {
    column_name = columnName;
}

int Column::getOrdinal() const {
    return ordinal;
}

void Column::setOrdinal(int ordinal) {
    Column::ordinal = ordinal;
}

bool Column::isUnsigned() const {
    return is_unsigned;
}

void Column::setIsUnsigned(bool isUnsigned) {
    is_unsigned = isUnsigned;
}

const string &Column::getCharset() const {
    return charset;
}

void Column::setCharset(const string &charset) {
    Column::charset = charset;
}

const string &Column::getColumnDef() const {
    return column_def;
}

void Column::setColumnDef(const string &columnDef) {
    column_def = columnDef;
}

int Column::getLength() const {
    return length;
}

void Column::setLength(int length) {
    Column::length = length;
}

int Column::getPrecision() const {
    return precision;
}

void Column::setPrecision(int precision) {
    Column::precision = precision;
}

int Column::getScale() const {
    return scale;
}

void Column::setScale(int scale) {
    Column::scale = scale;
}