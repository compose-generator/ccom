// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#include "IncompatibleTypesException.h"

const char *IncompatibleTypesException::what() const noexcept {
    return errorMessage.c_str();
}