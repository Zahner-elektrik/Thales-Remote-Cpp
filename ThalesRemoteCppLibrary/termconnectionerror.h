#ifndef TERMCONNECTIONERROR_H
#define TERMCONNECTIONERROR_H

#include "zahnererror.h"

/** The TermConnectionError class
 *
 *  This exception is thrown when an error occurs with the term communication,
 *  which has not yet been thrown by a socket exception.
 *
 *  After this error the connection must be completely rebuilt.
 */
class TermConnectionError : public ZahnerError
{
public:
    explicit TermConnectionError(const std::string& message);
};

#endif // TERMCONNECTIONERROR_H
