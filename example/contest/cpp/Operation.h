//
// Created by dts on 2021/6/6.
//

#ifndef TIANCHI_CPP_OPERATION_H
#define TIANCHI_CPP_OPERATION_H
class Operation {
private:
    string operation_type;
public:
    Operation(string operation_type) {
        this->operation_type = operation_type;
    }

    Operation() {
        this->operation_type = "I";
    }

    void set_operation_type(string& op_type) {
        operation_type = op_type;
    }

    string get_opeartion_type() {
        return operation_type;
    }
};
#endif //TIANCHI_CPP_OPERATION_H
