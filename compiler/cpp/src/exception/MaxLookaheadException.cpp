//
// Created by Marc on 27.05.2021.
//

#include "MaxLookaheadException.h"

const char *MaxLookaheadException::what() const noexcept {
    return errorMessage.c_str();
}