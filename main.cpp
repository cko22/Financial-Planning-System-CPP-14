// Student ID 2031611
#include <iostream>
#include <string>
#include <ctime>

#include "InputData.hpp"
#include "Investment.hpp"

// Program entry point
int main() {
    clock_t start{clock()}; // clock starts

    const std::string client_filename {"../People.csv"}; // Client input filename
    const std::string product_filename {"../Feed.dat"}; // Product input filename

    // Read input data, store them in vectors and print them out.
    InputData Data1;
    Data1.PrintData(client_filename, product_filename);

    // Invest for all the clients and output their financial plans as CSV files.
    Investment test;
    test.AutomateInvestment(Data1);
//    test.PrintTransaction(0,Data1);
    clock_t end{clock()}; // clock ends
    clock_t elapsed = end - start; // time difference
    std::cout << "Time elapsed: " << elapsed/CLOCKS_PER_SEC
              << ' ' <<"s" << std::endl;
    return 0;
}
