#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
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

template<class T>
class WSAExceptionT : public ExceptionT<struct WSAExceptionTag> {
    public:
        WSAExceptionT() = default;
        explicit WSAExceptionT(const char *msg) : ExceptionT(msg) {}
        explicit WSAExceptionT(const std::string &msg) : ExceptionT(msg) {}
        ~WSAExceptionT() override = default;
};

using WSAException = ExceptionT<WSAExceptionTag>;
using WSANotReadyException = WSAExceptionT<struct WSANotReadyTag>;
using WSAUnsupportedVersionException = WSAExceptionT<struct WSAUnsupportedVersionTag>;
using WSAInProgressException = WSAExceptionT<struct WSAInProgressTag>;
using WSAProcessLimitException = WSAExceptionT<struct WSAProcessLimitTag>;
using WSAInvalidDataException = WSAExceptionT<struct WSAInvalidDataTag>;
using WSAEnetDownException = WSAExceptionT<struct WSAEnetDownTag>;
using WSANotInitializedException = WSAExceptionT<struct WSANotInitializedTag>;

} // namespace CynNet

#endif // EXCEPTION_HPP
