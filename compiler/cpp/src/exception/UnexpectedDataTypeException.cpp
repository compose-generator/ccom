/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

#include "UnexpectedDataTypeException.h"

const char *UnexpectedDataTypeException::what() const noexcept {
    return errorMessage.c_str();
}