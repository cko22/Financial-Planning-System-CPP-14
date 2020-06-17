
#ifndef INC_2031611_INPUTDATA_HPP
#define INC_2031611_INPUTDATA_HPP


#include <vector>
#include <string>

#include "InputReader.hpp"

struct ClientData {
    std::vector <std::string> name;
    std::vector <int> capital; // in pence
    std::vector <int> retirement_date;
};

struct ProductData {
    std::vector <std::string> institution_code;
    std::vector <std::string> product_code;
    std::vector <int> interest_rate; // AER rate
    std::vector <std::string> compounding_period; // A or M
    std::vector <int> tie_in; // in years
    std::vector <int> min_investment; // in pounds
    std::vector <int> max_investment; // in pounds
    std::vector <int> available_date; // YYYYMMDD
    std::vector <int> maturity_date; // YYYYMMDD
};

class InputData {
private:
    ClientData client_data_;
    ProductData product_data_;
public:
    ClientData SetClientData(const std::string &client_filename);

    ClientData GetClientData() const;

    ProductData SetProductData(const std::string &product_filename);

    ProductData GetProductData() const;

    /*
    * Print input data given file names
    *
    * Args:
    *  - client input data file name
    *  - product input data file name
    *
    */
    void PrintData(const std::string &client_filename, const std::string &product_filename);
};



#endif //INC_2031611_INPUTDATA_HPP
