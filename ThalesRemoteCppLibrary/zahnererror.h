#ifndef ZAHNERERROR_H
#define ZAHNERERROR_H

#include <string>

class ZahnerError : public std::exception {

private:

    std::string message;

public:

    explicit ZahnerError(const std::string& message);

    const char* what() const noexcept override {

        return this->message.c_str();
    }

    const std::string &getMessage() const;
};

#endif // ZAHNERERROR_H
