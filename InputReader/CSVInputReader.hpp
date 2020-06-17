//
// Created by Owner on 8/4/2020.
//

#ifndef INVESTMENT_CSVINPUTREADER_HPP
#define INVESTMENT_CSVINPUTREADER_HPP

#include "InputReader.hpp"

// comma-seperated-values input stream reader
class CSVInputReader : public InputReader {
private:
    /*
     * Flag to hold if the reader instance is currently parsing characters
     * within a quoted string.
     */
    bool inside_double_quote_;
    /*
     * Flag to hold if the previous character was a double quote within a
     * quote string (as it may be either the end of the quoted string or the
     * start of a pair indicating a literal double quote.
     */
    bool last_char_double_quote_;
protected:
    virtual bool ProcessChar(const char in_char) override;
public:
    // Constructor - delegates handling the stream to InputReader base class
    CSVInputReader(std::shared_ptr<std::istream> input_stream)
            : InputReader(input_stream), inside_double_quote_{false},
              last_char_double_quote_{false}
    {}
};

#endif //INVESTMENT_CSVINPUTREADER_HPP
