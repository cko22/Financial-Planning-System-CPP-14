// Student ID 2031611
#include <algorithm>    // std::find
#include <numeric>     // std::iota
#include <cmath>       // std::pow
#include <iostream>    // std::cout
#include <fstream>    // std::ofstream

#include "Investment.hpp"
#include "Date.hpp"
#include "InputData.hpp"

TransactionHistory Investment::Strategy(const int &client_index, const InputData &data) {
    // Initialise current capital to the initial capital.
    current_capital_ = data.GetClientData().capital.at(client_index);

    // Initialise current date to start date.
    Date date1;
    int start_date;
    start_date = date1.GetStartDate();
    current_date_ = start_date;
    int current_days;
    current_days = date1.DateToDays(current_date_);

    // Calculate investment time horizon.
    int n_days;
    int retirement_date;
    retirement_date = data.GetClientData().retirement_date.at(client_index);
    n_days = date1.DateToDays(retirement_date) - date1.DateToDays(start_date);

    //~~~ Loop from start date to retirement date, with increment of +1 day.
    int counter{0};
    for (int d{0} ; n_days > d ; ++d) {

        //~ No. of days to retirement date
        int days_to_retirement = n_days - counter;
        //~ Update new date
        current_date_ = date1.DaysToDate(current_days);

        //~ Add newly available product from product feed to available products vector
        int it1 = ElementPosition(data.GetProductData().available_date,current_date_);
        if (it1 < data.GetProductData().available_date.size()) {
            // If found, add it to available product vector
            int index = it1;
            available_products_.product_index.emplace_back(index);
            available_products_.interest_rate.emplace_back(data.GetProductData().interest_rate.at(index));
            available_products_.tie_in_days.emplace_back(data.GetProductData().tie_in.at(index)*365); // convert to days
            available_products_.maturity_date.emplace_back(data.GetProductData().maturity_date.at(index));
        }

        //~ Remove matured products from available products vector
        int it2 = ElementPosition(available_products_.maturity_date,current_date_);
        if (it2 < available_products_.maturity_date.size()) {
            // If found, remove it from available product vector
            int index = it2;
            available_products_.product_index.erase(available_products_.product_index.begin() + index);
            available_products_.interest_rate.erase(available_products_.interest_rate.begin() + index);
            available_products_.tie_in_days.erase(available_products_.tie_in_days.begin() + index);
            available_products_.maturity_date.erase(available_products_.maturity_date.begin() + index);
        }

        //~ From the AvailableProducts vector, find the product with the highest interest rate.
        std::vector <int> V(available_products_.interest_rate.size()); // a vector of available product vector's index;
        int x{0};
        std::iota(std::begin(V), std::end(V), x++);
        // Sort the vector V in descending order of interest rate.
        std::sort(V.begin(), V.end(), [&](int i,int j){return available_products_.interest_rate.at(i)
                                                              > available_products_.interest_rate.at(j);} );

        //~ Search for saleable products on invested products vector
        for (int i{0} ; invested_products_.product_index.size() > i ; ++i) {
            // If tie-in period ended, add to saleable products vector
            if (current_date_ >= invested_products_.tie_in_end_date.at(i)) {
                saleable_products_.product_index.emplace_back(invested_products_.product_index.at(i));
                saleable_products_.interest_rate.emplace_back(invested_products_.interest_rate.at(i));
                saleable_products_.invested_amount.emplace_back(invested_products_.invested_amount.at(i));
                saleable_products_.invested_date.emplace_back(invested_products_.invested_date.at(i));
                saleable_products_.maturity_date.emplace_back(invested_products_.maturity_date.at(i));

                // Remove these products from invested products vector
                invested_products_.product_index.erase(invested_products_.product_index.begin() + i);
                invested_products_.interest_rate.erase(invested_products_.interest_rate.begin() + i);
                invested_products_.invested_amount.erase(invested_products_.invested_amount.begin() + i);
                invested_products_.invested_date.erase(invested_products_.invested_date.begin() + i);
                invested_products_.tie_in_end_date.erase(invested_products_.tie_in_end_date.begin() + i);
                invested_products_.maturity_date.erase(invested_products_.maturity_date.begin() + i);
            }
        }

        //~ Search for saleable products with the lowest interest rate.
        std::vector <int> W(saleable_products_.interest_rate.size()); // a vector of saleable product vector's index;
        int y{0};
        std::iota(std::begin(W), std::end(W), y++);
        // Sort the vector W in ascending order of interest rate.
        std::sort(W.begin(), W.end(), [&](int i,int j){return saleable_products_.interest_rate.at(i)
                                                              < saleable_products_.interest_rate.at(j);} );

//        // ----------------UNCOMMENT FOR INCLUDING SELLING AS PART OF THE STRATEGY (START)----------------
//        // ---------------------------SELLING STARTS---------------------------
//        // Consider selling products only when capital is zero
//        if ((0 == current_capital_) && (0 != saleable_products_.product_index.size())){
//            int i = W.at(0); // consider the smallest rate, saleable product
//            int j = V.at(0); // consider the highest rate, available product
//            int index = saleable_products_.product_index.at(i); // get product index of the smallest rate, saleable product
//            int index1 = available_products_.product_index.at(j); // get product index of the highest rate, available product
//
//            // Calculate how much this product is worth
//            // Get compounding period
//            std::string period = data.GetProductData().compounding_period.at(index);
//
//            // Calculate no. of completed period
//            Date dateobj;
//            int num_periods = dateobj.CompletedAnniversaries(saleable_products_.invested_date.at(i), current_date_,
//                                                             period);
//            //Calculate investment return on this product
//            int ROI = InvestmentReturn(saleable_products_.invested_amount.at(i),
//                                       saleable_products_.interest_rate.at(i), period,
//                                       num_periods);
//
//            // If this product's interest rate greater than the maximum rate of the available product AND the its worth is at
//            // least the minimum investment of the available product, then SELL
//            if ((data.GetProductData().interest_rate.at(index) > data.GetProductData().interest_rate.at(index1))
//                && (ROI >= data.GetProductData().min_investment.at(index1)*100)){
//                // Update sold products vector
//                sold_products_.product_index.emplace_back(saleable_products_.product_index.at(i));
//                sold_products_.sold_amount.emplace_back(ROI);
//                sold_products_.sold_date.emplace_back(current_date_);
//
//                // Update current capital
//                current_capital_ = current_capital_ + ROI;
//
//                // Update transaction history
//                transaction_history_.transaction_date.emplace_back(current_date_);
//                transaction_history_.institution_code.emplace_back(data.GetProductData().institution_code.at(index));
//                transaction_history_.product_code.emplace_back(data.GetProductData().product_code.at(index));
//                transaction_history_.activity.emplace_back("SELL");
//                transaction_history_.amount.emplace_back(ROI);
//                transaction_history_.current_capital.emplace_back(current_capital_);
//                ++sale;
//            }
//        }
//
//        //~ Remove sold products from saleable products vector
//        // Find the sold product (if there is any)
//        for (int i{0} ; sold_products_.product_index.size() > i ; ++i){
//            int it1 = ElementPosition(saleable_products_.product_index,sold_products_.product_index.at(i));
//            if (it1 < saleable_products_.product_index.size()){
//                // If found, remove it from available product vector
//                int index = it1;
//                saleable_products_.product_index.erase(saleable_products_.product_index.begin() + index);
//                saleable_products_.interest_rate.erase(saleable_products_.interest_rate.begin() + index);
//                saleable_products_.invested_amount.erase(saleable_products_.invested_amount.begin() + index);
//                saleable_products_.invested_date.erase(saleable_products_.invested_date.begin() + index);
//                saleable_products_.maturity_date.erase(saleable_products_.maturity_date.begin() + index);
//            }
//        }
//        // ---------------------------SELLING ENDS---------------------------
//        // ----------------UNCOMMENT FOR INCLUDING SELLING AS PART OF THE STRATEGY (END)----------------

        // ---------------------------INVESTING STARTS---------------------------
        //~ Check highest AER product's minimum investment, tie-in period
        if (0 != current_capital_) {
            for (int i : V) {
                int index = available_products_.product_index.at(i); // get product index
                // Invest if current capital is greater or equal to minimum investment AND (tie-in period ends before retirement's date
                // OR mature before retirement).
                if ((data.GetProductData().min_investment.at(index)*100 <= current_capital_)
                    && ((data.GetProductData().tie_in.at(index)*365 < days_to_retirement)
                        || (data.GetProductData().maturity_date.at(index) < data.GetClientData().retirement_date.at(client_index)))) {
                    // Invest maximum amount or all of the capital, whichever SMALLER.
                    int invested_amount = std::min(data.GetProductData().max_investment.at(index)*100, current_capital_);
                    // Update current_capital_
                    current_capital_ = current_capital_ - invested_amount;

                    // Update invested products vector
                    invested_products_.product_index.emplace_back(index);
                    invested_products_.interest_rate.emplace_back(data.GetProductData().interest_rate.at(index));
                    invested_products_.invested_amount.emplace_back(invested_amount);
                    invested_products_.invested_date.emplace_back(current_date_);
                    invested_products_.tie_in_end_date.emplace_back(current_date_+data.GetProductData().tie_in.at(index)*10000);
                    invested_products_.maturity_date.emplace_back(data.GetProductData().maturity_date.at(index));

                    // Record transaction history for investing
                    transaction_history_.transaction_date.emplace_back(current_date_);
                    transaction_history_.institution_code.emplace_back(data.GetProductData().institution_code.at(index));
                    transaction_history_.product_code.emplace_back(data.GetProductData().product_code.at(index));
                    transaction_history_.activity.emplace_back("Invest");
                    transaction_history_.amount.emplace_back(invested_amount);
                    transaction_history_.current_capital.emplace_back(current_capital_);
                    break; // invest a product at a time ;
                }
            }
        }

        //~ Remove invested products from available products vector
        // Find the invested product (if there is any)
        for (int i : invested_products_.product_index) {
            int it3 = ElementPosition(available_products_.product_index, i);
            if (it3 < available_products_.product_index.size()) {
                // If found, remove it from available product vector
                int index = it3;
                available_products_.product_index.erase(available_products_.product_index.begin() + index);
                available_products_.interest_rate.erase(available_products_.interest_rate.begin() + index);
                available_products_.tie_in_days.erase(available_products_.tie_in_days.begin() + index);
                available_products_.maturity_date.erase(available_products_.maturity_date.begin() + index);
            }
        }
        // ---------------------------INVESTING ENDS---------------------------

        // ---------------------------REDEMPTION STARTS---------------------------
        //~ Redeem matured products from invested product
        int it4 = ElementPosition(invested_products_.maturity_date,current_date_);
        if (it4 < invested_products_.maturity_date.size()) {
            // If found, update redeemed products vector and remove it from invested products vector
            int index = it4;

            // Get compounding period
            std::string period = data.GetProductData().compounding_period.at(invested_products_.product_index.at(index));

            // Calculate no. of completed period
            Date dateobj;
            int num_periods = dateobj.CompletedAnniversaries(invested_products_.invested_date.at(index), current_date_,
                                                             period);
            //Calculate investment return on this product
            int ROI = InvestmentReturn(invested_products_.invested_amount.at(index),
                                       invested_products_.interest_rate.at(index), period,
                                       num_periods);

            // Update redeemed products
            redeemed_products_.product_index.emplace_back(invested_products_.product_index.at(index));
            redeemed_products_.redeemed_amount.emplace_back(ROI);
            redeemed_products_.redeemed_date.emplace_back(current_date_);

            // Update current capital
            current_capital_ = current_capital_ + ROI;

            // Record transaction history for redemption.
            transaction_history_.transaction_date.emplace_back(current_date_);
            transaction_history_.institution_code.emplace_back(data.GetProductData().institution_code.at(invested_products_.product_index.at(index)));
            transaction_history_.product_code.emplace_back(data.GetProductData().product_code.at(invested_products_.product_index.at(index)));
            transaction_history_.activity.emplace_back("Redeem");
            transaction_history_.amount.emplace_back(ROI);
            transaction_history_.current_capital.emplace_back(current_capital_);
        }

        //~ Remove redeemed products from invested products vector
        // Find the redeemed product (if there is any)
        for (int i : redeemed_products_.product_index) {
            int it5 = ElementPosition(invested_products_.product_index, i);
            if (it5 < invested_products_.product_index.size()) {
                // If found, remove it from available product vector
                int index = it5;
                invested_products_.product_index.erase(invested_products_.product_index.begin() + index);
                invested_products_.interest_rate.erase(invested_products_.interest_rate.begin() + index);
                invested_products_.invested_amount.erase(invested_products_.invested_amount.begin() + index);
                invested_products_.invested_date.erase(invested_products_.invested_date.begin() + index);
                invested_products_.tie_in_end_date.erase(invested_products_.tie_in_end_date.begin() + index);
                invested_products_.maturity_date.erase(invested_products_.maturity_date.begin() + index);
            }
        }

        //~ Redeem matured products from saleable product vector
        int it6 = ElementPosition(saleable_products_.maturity_date, current_date_);
        if (it6 < saleable_products_.maturity_date.size()) {
            // If found, update redeemed products vector and remove it from invested products vector
            int index = it6;

            // Get compounding period
            std::string period = data.GetProductData().compounding_period.at(saleable_products_.product_index.at(index));

            // Calculate no. of completed period
            Date dateobj;
            int num_periods = dateobj.CompletedAnniversaries(saleable_products_.invested_date.at(index), current_date_,
                                                             period);
            //Calculate investment return on this product
            int ROI = InvestmentReturn(saleable_products_.invested_amount.at(index),
                                       saleable_products_.interest_rate.at(index), period,
                                       num_periods);

            // Update redeemed products
            redeemed_products_.product_index.emplace_back(saleable_products_.product_index.at(index));
            redeemed_products_.redeemed_amount.emplace_back(ROI);
            redeemed_products_.redeemed_date.emplace_back(current_date_);

            // Update current capital
            current_capital_ = current_capital_ + ROI;

            // Record transaction history for redemption.
            transaction_history_.transaction_date.emplace_back(current_date_);
            transaction_history_.institution_code.emplace_back(data.GetProductData().institution_code.at(saleable_products_.product_index.at(index)));
            transaction_history_.product_code.emplace_back(data.GetProductData().product_code.at(saleable_products_.product_index.at(index)));
            transaction_history_.activity.emplace_back("Redeem");
            transaction_history_.amount.emplace_back(ROI);
            transaction_history_.current_capital.emplace_back(current_capital_);
        }

        //~ Remove redeemed products from saleable products vector
        // Find the most recent sold product (if there is any)
        for (int i : redeemed_products_.product_index) {
            int it7 = ElementPosition(saleable_products_.product_index, i);
            if (it7 < saleable_products_.product_index.size()) {
                // If found, remove it from available product vector
                int index = it7;
                saleable_products_.product_index.erase(saleable_products_.product_index.begin() + index);
                saleable_products_.interest_rate.erase(saleable_products_.interest_rate.begin() + index);
                saleable_products_.invested_amount.erase(saleable_products_.invested_amount.begin() + index);
                saleable_products_.invested_date.erase(saleable_products_.invested_date.begin() + index);
                saleable_products_.maturity_date.erase(saleable_products_.maturity_date.begin() + index);
            }
        }
        // ---------------------------REDEMPTION ENDS---------------------------

        //~ Sell all the remaining  products one day before retirement
        if (1 == days_to_retirement){
            // Tie-in ended but not matured products
            for (int i{0} ; saleable_products_.product_index.size() > i; ++i) {
                int index = saleable_products_.product_index.at(i);
                // Get compounding period
                std::string period = data.GetProductData().compounding_period.at(index);

                // Calculate no. of completed period
                Date dateobj;
                int num_periods = dateobj.CompletedAnniversaries(saleable_products_.invested_date.at(i), current_date_,
                                                                 period);

                //Calculate investment return on this product
                int ROI = InvestmentReturn(saleable_products_.invested_amount.at(i),
                                           saleable_products_.interest_rate.at(i), period,
                                           num_periods);

                // Update current capital
                current_capital_ = current_capital_ + ROI;

                // Record transaction history for sale.
                transaction_history_.transaction_date.emplace_back(current_date_);
                transaction_history_.institution_code.emplace_back(data.GetProductData().institution_code.at(index));
                transaction_history_.product_code.emplace_back(data.GetProductData().product_code.at(index));
                transaction_history_.activity.emplace_back("Sell");
                transaction_history_.amount.emplace_back(ROI);
                transaction_history_.current_capital.emplace_back(current_capital_);
            }
        }
        //~ Advance 1 day each loop
        ++current_days;
        ++counter; // count how many days completed.
    }
    return transaction_history_;
}

int Investment::InvestmentReturn(const int &invested_amount, const int &rate, const std::string &period, const int &num_period) {
    int return_of_investment; //in pence

    // Convert interest rate into decimal
    double rate_decimal = double(rate)/10000;

    // Calculate returns
    if ("A" == period) {
        return_of_investment = int(invested_amount*std::pow(rate_decimal+1.0,num_period));
        return return_of_investment;
    }
    if ("M" == period) {
        // Convert the AER rate to monthly gross rate
        double monthly_rate = std::pow(rate_decimal+1.0, 1.0/12) - 1;
        return_of_investment = int(invested_amount*std::pow(monthly_rate+1.0, num_period));
        return return_of_investment;
    }

    return 0;
}

int Investment::ElementPosition(const std::vector <int> &vec, const int &element) {
    int index;
    for (int i{0} ; vec.size() > i ; ++i) {
        // If found matching element return the position
        if (element == vec.at(i)) {
            index = i;
            return index;
        }
    }
    // If not found
    return vec.size() + 10;
}

void Investment::PrintTransaction(const int &client_index, const InputData &data) {
    // Start investment
    Investment::Strategy(client_index, data);

    // Print transaction history
    std::cout << "-----------------Transaction History for Client: " << data.GetClientData().name.at(client_index) << "-----------------"<< std::endl;
    for (int i{0} ; transaction_history_.transaction_date.size() > i ; ++i) {
        std::cout << transaction_history_.transaction_date.at(i) << "   " << transaction_history_.institution_code.at(i) << "   "
                  << transaction_history_.product_code.at(i) << "   " << transaction_history_.activity.at(i) << "   "
                  << transaction_history_.amount.at(i) << "   " << transaction_history_.current_capital.at(i)
                  << std::endl;
    }
}

void Investment::CSVOutputFile(const int &client_index, const InputData &data) {
    // Execute investment strategy given product feed and client.
    Investment::Strategy(client_index, data);

    // Create a file
    std::string filename = data.GetClientData().name.at(client_index) + ".csv";
    std::ofstream File;

    // Test if the file exists
    std::ifstream in_file {filename};
    if (in_file.fail()) {
        // file does not exist
        // Clear the error so we can reuse the stream object
        in_file.clear();
    } else {
        // file exists
        in_file.close();
    }

    File.open(filename);

    for (int i{0} ; transaction_history_.transaction_date.size() > i ; ++i) {
        File << transaction_history_.transaction_date.at(i) << "," << transaction_history_.institution_code.at(i) << ","
             << transaction_history_.product_code.at(i) << "," << transaction_history_.activity.at(i) << ","
             << transaction_history_.amount.at(i) << std::endl;
    }

    File.close(); // close file after writing
}

void Investment::AutomateInvestment(const InputData &data) {
    for (int i{0} ; data.GetClientData().name.size() > i ; ++i) {
        Investment plan; // create new object for new client
        plan.CSVOutputFile(i, data);
        std::cout << "Financial plan for client " << i + 1 << ", " <<data.GetClientData().name.at(i)
                  << " is ready."<< std::endl;
    }
}