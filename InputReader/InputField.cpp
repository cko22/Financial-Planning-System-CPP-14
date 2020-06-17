// Student ID 2031611

#include <string>


#include "InputField.hpp"

std::ostream& operator<<(std::ostream& lhs, const InputField & rhs) {
    lhs << static_cast<std::string>(rhs);
    return lhs;
}