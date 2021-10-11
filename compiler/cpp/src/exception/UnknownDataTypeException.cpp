/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

#include "UnknownDataTypeException.h"

const char *UnknownDataTypeException::what() const noexcept {
    return errorMessage.c_str();
}