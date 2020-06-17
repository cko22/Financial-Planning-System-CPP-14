// Student ID 2031611

#include "TSVInputReader.hpp"

bool TSVInputReader::ProcessChar(const char in_char) {
    // Tab and newline end (and are not part of) fields.
    if (in_char == '\t' || in_char == '\n')
        return true;

    // Not tab or newline, part of the field being read
    current_field_ += in_char;
    return false;
}