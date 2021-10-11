/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

#include "UnexpectedDataTypeException.h"

const char *UnexpectedDataTypeException::what() const noexcept {
    return errorMessage.c_str();
}