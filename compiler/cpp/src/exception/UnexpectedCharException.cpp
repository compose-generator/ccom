/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#include "UnexpectedCharException.h"

const char *UnexpectedCharException::what() const noexcept {
    return errorMessage.c_str();
}
