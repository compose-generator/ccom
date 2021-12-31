/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

#include "MaxLookaheadException.h"

const char *MaxLookaheadException::what() const noexcept {
    return errorMessage.c_str();
}