//
// Created by dts on 2021/6/6.
//

#ifndef TIANCHI_CPP_INDEX_H
#define TIANCHI_CPP_INDEX_H

#include <string>
#include <unordered_set>
#include <unordered_map>

using namespace std;

class Index {
private:
    string index_name;
    unordered_set<string> index_column;
    bool is_pk;
    bool is_uk;
public:
    Index(const unordered_map<string, string> &map);

    const string &getIndexName() const;

    void setIndexName(const string &indexName);

    const unordered_set<string> &getIndexColumn() const;

    void setIndexColumn(const unordered_set<string> &indexColumn);

    bool isPk() const;

    void setIsPk(bool isPk);

    bool isUk() const;

    void setIsUk(bool isUk);

    bool operator==(const Index &other);

    bool operator<(const Index &other);
};

#endif //TIANCHI_CPP_INDEX_H
