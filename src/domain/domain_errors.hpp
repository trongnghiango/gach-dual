#pragma once

#include <stdexcept>
#include <string>

namespace gach::domain {

class DomainError : public std::runtime_error {
public:
    explicit DomainError(const std::string& message) : std::runtime_error(message) {}
};

class DomainValidationError : public DomainError {
public:
    explicit DomainValidationError(const std::string& message) : DomainError(message) {}
};

class InvalidStateTransitionError : public DomainError {
public:
    explicit InvalidStateTransitionError(const std::string& message) : DomainError(message) {}
};

} // namespace gach::domain