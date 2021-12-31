/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

#include "UnknownDataTypeException.h"

const char *UnknownDataTypeException::what() const noexcept {
    return errorMessage.c_str();
}