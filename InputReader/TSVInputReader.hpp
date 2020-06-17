//
// Created by Owner on 8/4/2020.
//

#ifndef INVESTMENT_TSVINPUTREADER_HPP
#define INVESTMENT_TSVINPUTREADER_HPP

#include <istream>
#include <memory>

#include "InputReader.hpp"

// Tab-seperated-values input reader
class TSVInputReader : public InputReader {
protected:
    virtual bool ProcessChar(const char in_char) override;
public:
    // Delegating constructor for InputReader base class
    TSVInputReader(std::shared_ptr<std::istream> input_stream)
            : InputReader(input_stream)
    {}
};

#endif //INVESTMENT_TSVINPUTREADER_HPP
