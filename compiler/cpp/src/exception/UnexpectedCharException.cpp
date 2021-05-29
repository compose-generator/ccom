//
// Created by Marc on 23.05.2021.
//

#include "UnexpectedCharException.h"

const char *UnexpectedCharException::what() const noexcept {
    return errorMessage.c_str();
}
