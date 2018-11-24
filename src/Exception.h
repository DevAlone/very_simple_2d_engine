#pragma once

#include <iostream>
#include <string>

class Exception {
public:
    Exception(const std::string& message = "");
    ~Exception() {}

    const std::string& getMessage() const;

private:
    std::string message;
};

inline std::ostream& operator<<(
    std::ostream& stream,
    const Exception& exc)
{
    return stream << "Exception { message: \"" << exc.getMessage() << "\" }";
}
