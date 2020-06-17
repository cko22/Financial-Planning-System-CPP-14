//
// Created by Owner on 8/4/2020.
//

#ifndef INVESTMENT_INPUTREADER_HPP
#define INVESTMENT_INPUTREADER_HPP

#include <istream>
#include <memory>
#include <string>

#include "InputField.hpp"

// Abstract base class for all input file readers
class InputReader {
private:
    // Will hold the stream being read from
    std::shared_ptr<std::istream> input_stream_;

protected:
    /* Process next character from file.
     * Must be implemented by all deriving classes - this is how they
     * interpret the content of the file (i.e. provide their
     * specialisation).
     *
     * Args:
     *   char - character read from file.
     * Returns:
     *   true if the last character was the end of the current field, false
     *   otherwise.
     */
    virtual bool ProcessChar(const char) = 0;

    /* Protected member to hold the current field that will be returned.
     * Will be returned when ProcessChar returns true - ProcessChar should
     * populate this.
     */
    std::string current_field_;

public:
    /* Constructor.
     *
     * Args:
     *   std::shared_ptr<std::istream> - shared pointer to the input stream
     *     to read from (so we don't try and duplicate the stream).
     */
    InputReader(std::shared_ptr<std::istream> input_stream)
            : input_stream_{input_stream}, current_field_ {""}
    {}

    /* Method to get next field from the input stream.
     * Returns:
     *   String containing the next field in the stream.
     */
    InputField GetNextField();
    virtual ~InputReader() = default;
};

/*
 * Constructs and returns an appropriate InputReader
 *
 * Args:
 *  filename: path to file to open
 *
 * Returns:
 *  Unique pointer to a constructed InputReader object
 */
std::unique_ptr<InputReader> InputReaderFactory(std::string filename);

#endif //INVESTMENT_INPUTREADER_HPP
