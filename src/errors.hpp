#ifndef ERRORS_H
#define ERRORS_H

#include <string>

namespace aether {
    class AetherError : public std::exception {
    public:
        virtual const char* what() const noexcept override {
            return "Aether error occurred";
        }
    };

    class ParsingError : public AetherError {
    private:
        std::string message_;
    public:
        explicit ParsingError(const std::string& msg) : message_(msg) {}
        const char* what() const noexcept override {
            return message_.c_str();
        }
    };

    class FileOpenError : public AetherError {
    private:
        std::string message_;
    public:
        explicit FileOpenError(const std::string& msg) : message_(msg) {}
        const char* what() const noexcept override {
            return message_.c_str();
        }
    };
};

#endif // ERRORS_H
