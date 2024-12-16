#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP
#include <exception>
#include <stdexcept>
#include <string>
#include <utility>

namespace CynNet {

template<class T>
class ExceptionT : public std::exception {
    private:
        std::string msg;

    public:
        ExceptionT() = default;
        explicit ExceptionT(std::string msg) : msg(std::move(msg)) {}
        explicit ExceptionT(const char *msg) : msg(msg) {}
        ~ExceptionT() override = default;
        [[nodiscard]] const char *what() const noexcept override { return msg.c_str(); }
};

template<class T = auto>
class WSAExceptionT : public ExceptionT<struct WSAException> {
    public:
        WSAExceptionT() = default;
        explicit WSAExceptionT(const char *msg) : ExceptionT(msg) {}
        explicit WSAExceptionT(const std::string &msg) : ExceptionT(msg) {}
        ~WSAExceptionT() override = default;
};

using WSANotReadyException = WSAExceptionT<struct WSANotReady>;
using WSAUnsupportedVersionException = WSAExceptionT<struct WSAUnsupportedVersion>;
using WSAInProgressException = WSAExceptionT<struct WSAInProgress>;
using WSAProcessLimitException = WSAExceptionT<struct WSAProcessLimit>;
using WSAInvalidDataException = WSAExceptionT<struct WSAInvalidData>;
using WSAEnetDownException = WSAExceptionT<struct WSAEnetDown>;
using WSANotInitializedException = WSAExceptionT<struct WSANotInitialized>;

} // namespace CynNet

#endif // EXCEPTION_HPP
