

#ifndef INC_2031611_INVESTMENT_HPP
#define INC_2031611_INVESTMENT_HPP


#include <vector>

#include "Date.hpp"
#include "InputData.hpp"

struct AvailableProducts {
    std::vector <int> product_index; // product index on product feed
    std::vector <int> interest_rate;
    std::vector <int> tie_in_days; // tie-in_period in days
    std::vector <int> maturity_date;
};

struct InvestedProducts {
    std::vector <int> product_index; // product index on product feed
    std::vector <int> interest_rate;
    std::vector <int> invested_amount; // in pence
    std::vector <int> invested_date;
    std::vector <int> tie_in_end_date;
    std::vector <int> maturity_date;
};

struct SaleableProducts {
    std::vector <int> product_index; // product index on product feed
    std::vector <int> interest_rate;
    std::vector <int> invested_amount; // in pence
    std::vector <int> invested_date;
    std::vector <int> maturity_date;
};

struct RedeemedProducts {
    std::vector <int> product_index; // product index on product feed
    std::vector <int> redeemed_amount; // invested amount + interest in pence
    std::vector <int> redeemed_date;
};

struct SoldProducts {
    std::vector <int> product_index; // product index on product feed
    std::vector <int> sold_amount; // invested amount + interest in pence
    std::vector <int> sold_date;
};

struct TransactionHistory {
    std::vector <int> transaction_date; // in the format of YYYYMMDD
    std::vector <std::string> institution_code;
    std::vector <std::string> product_code;
    std::vector <std::string> activity; // invest, sell or redeem.
    std::vector <int> amount; // in pence
    std::vector <int> current_capital;
};

class Investment {
private:
    AvailableProducts available_products_;
    InvestedProducts invested_products_;
    TransactionHistory transaction_history_;
    RedeemedProducts redeemed_products_;
    SoldProducts sold_products_;
    SaleableProducts saleable_products_;
    int current_date_;
    int current_capital_;
public:
    /*
     * Investment strategy for one client
     *
     * Args:
     * - client index
     * - input data
     *
     * Returns:
     * - client's transaction history
     */
    TransactionHistory Strategy(const int &client_index, const InputData &data);

    /*
     * Calculate return of investment
     *
     * Args:
     * - invested amount (in pence)
     * - AER interest rate (3 digits - for example 522 is 5.22%)
     * - compounding period (A or M)
     * - no of completed period
     *
     * Returns:
     * - return of investment (in pence)
     */
    int InvestmentReturn(const int &invested_amount, const int &rate, const std::string &period, const int &num_period);

    /*
     * Search for a given int element in a vector (int)  and returns its position
     *
     * Args:
     * - vector (int type)
     * - int element to be searched for
     *
     * Returns:
     * - position in the vector if found
     * - return size of vector if not found
     */
    int ElementPosition(const std::vector <int> &vec, const int &element);

    /*
     * Print the transaction history of a client
     *
     * Args:
     * - client index in data file
     *
     * Returns:
     * - NONE
     */
    void PrintTransaction(const int &client_index, const InputData &data);

    /*
     * Write a CSV file containing financial plan for a client
     *
     * Args:
     * - client index
     * - InputData
     *
     * Returns:
     * - NONE
     */
    void CSVOutputFile(const int &client_index, const InputData &data);

    /*
     * Automate investment for all the clients
     *
     * Args:
     * - Client input data and product feed
     *
     * Returns:
     * - NONE
     */
    void AutomateInvestment(const InputData &data);
};


#endif //INC_2031611_INVESTMENT_HPP
