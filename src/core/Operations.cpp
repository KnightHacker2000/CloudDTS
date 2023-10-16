//
// Created by Frank Li on 6/22/21.
//

#include "core/Operation.h"

Operation::Operation(string operation_type) {
    this->operation_type = operation_type;
}

Operation::Operation() {
    this->operation_type = "I";
}

void Operation::set_operation_type(string &op_type) {
    operation_type = op_type;
}

string Operation::get_opeartion_type() {
    return operation_type;
}