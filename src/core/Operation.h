//
// Created by dts on 2021/6/6.
//

#ifndef TIANCHI_CPP_OPERATION_H
#define TIANCHI_CPP_OPERATION_H

#include <string>

using namespace std;

class Operation {
private:
    string operation_type;
public:
    Operation(string operation_type);

    Operation();

    void set_operation_type(string &op_type);

    string get_opeartion_type();
};

#endif //TIANCHI_CPP_OPERATION_H
