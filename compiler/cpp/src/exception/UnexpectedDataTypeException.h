/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

#pragma once

#include <string>

class UnexpectedDataTypeException : public std::exception {
public:
    // Constructors
    explicit UnexpectedDataTypeException(const std::string& dataDump, const std::string& expectedDataTypeName) {
        errorMessage = "Unexpected data '" + dataDump + "'. Expected was data of type " + expectedDataTypeName;
    }

    // Public methods
    const char * what() const noexcept override;
private:
    // Members
    std::string errorMessage {};
};
