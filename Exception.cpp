#include "Exception.h"

Exception::Exception(const std::string& message)
    : message(message)
{
}

const std::string& Exception::getMessage() const
{
    return message;
}
