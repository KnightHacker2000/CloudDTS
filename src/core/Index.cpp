//
// Created by Frank Li on 6/22/21.
//

#include "core/Index.h"

Index::Index(const unordered_map <string, string> &map) {
    this->index_name = map.at("IndexName");
    this->index_column.insert(map.at("IndexCols"));
    this->is_pk = map.at("Primary")[0] == 't' ? true : false;
    this->is_uk = map.at("Unique")[0] == 't' ? true : false;
}

const string &Index::getIndexName() const {
    return index_name;
}

void Index::setIndexName(const string &indexName) {
    index_name = indexName;
}

const unordered_set <string> &Index::getIndexColumn() const {
    return index_column;
}

void Index::setIndexColumn(const unordered_set <string> &indexColumn) {
    index_column = indexColumn;
}

bool Index::isPk() const {
    return is_pk;
}

void Index::setIsPk(bool isPk) {
    is_pk = isPk;
}

bool Index::isUk() const {
    return is_uk;
}

void Index::setIsUk(bool isUk) {
    is_uk = isUk;
}

bool Index::operator==(const Index &other) {
    return this->index_column == other.index_column;
}

bool Index::operator<(const Index &other) {
    return this->index_column.size() < other.index_column.size();
}