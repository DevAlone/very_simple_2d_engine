#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

class Exception {
public:
    Exception(const std::string& message = "");
    ~Exception() {}

    const std::string& getMessage() const;

private:
    std::string message;
};

#endif // EXCEPTION_H
