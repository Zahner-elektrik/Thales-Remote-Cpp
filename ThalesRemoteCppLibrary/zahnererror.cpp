#include "zahnererror.h"

ZahnerError::ZahnerError(const std::string &message)  : message(message)
{

}

const std::string &ZahnerError::getMessage() const
{
    return this->message;
}
