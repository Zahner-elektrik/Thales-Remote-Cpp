#ifndef THALESREMOTEERROR_H
#define THALESREMOTEERROR_H

#include <string>
#include "zahnererror.h"

/** The ThalesRemoteError class
 *
 *  This exception is thrown when an error is reported in the remote protocol,
 *  for example, when a parameter is out of range.
 *  In the following document the errors are explained.
 *  http://zahner.de/pdf/Remote2.pdf
 */
class ThalesRemoteError : public ZahnerError
{
public:
    explicit ThalesRemoteError(const std::string& message);
};

#endif // THALESREMOTEERROR_H
