// Student ID 2031611

#include <iostream>

#include "InputData.hpp"
#include "InputReader.hpp"
#include "FixedWidthInputReader.hpp"

ClientData InputData::SetClientData(const std::string &client_filename) {
    // Test if the both csv and tsv exist
    std::size_t last_index = client_filename.find_last_of('.');
    std::string raw_name = client_filename.substr(0, last_index); // get file name without extension
    std::ifstream in_file {raw_name + ".csv"};
    std::ifstream in_file1 {raw_name + ".tsv"};
    // If both exists
    if (!in_file.fail() && !in_file1.fail()) {
        std::cout << "Both " << raw_name + ".csv" << " and " << raw_name + ".tsv" <<
                  " files found in the directory!" << std::endl;
        in_file.close();
        in_file1.close();
    }

    // Error check to make sure file is opened properly.
    std::ifstream client_file(client_filename);
    int count1{0};
    std::string line1;
    if (!client_file.is_open()) {
        std::cerr<< "Error opening: " << client_filename << std::endl;
        exit (21);
    } else {
        std::cout << client_filename << " file opened successfully!" << std::endl;
        // Count no. of lines in client data file
        while (!client_file.eof()) {
            std::getline(client_file, line1);
            if(line1 == "") continue; // Skip blank line
            ++count1;
        }
        client_file.close();
    }

    // Client data file
    std::unique_ptr<InputReader> Filereader1;
    Filereader1 = InputReaderFactory(client_filename);

    // Store Client data in vectors
    for (int a{0} ; count1 > a ; ++a) {
        if (1 > a) {
            // Do not store first three fields (headers)
            Filereader1->GetNextField();
            Filereader1->GetNextField();
            Filereader1->GetNextField();
        } else {
            client_data_.name.emplace_back(Filereader1->GetNextField());
            client_data_.capital.emplace_back(Filereader1->GetNextField());
            client_data_.retirement_date.emplace_back(Filereader1->GetNextField());
        }
    }
    return client_data_;
}

ClientData InputData::GetClientData() const {
    return client_data_;
}

ProductData InputData::SetProductData(const std::string &product_filename) {

    // Error check to make sure file is opened properly.
    std::ifstream product_file(product_filename);
    int count2{0};
    std::string line1;
    if (!product_file.is_open()) {
        std::cerr<< "Error opening: " << product_filename << std::endl;
        exit (22);
    } else {
        std::cout << product_filename << " file opened successfully!" << std::endl;
        // Count no. of lines in client data file
        while (!product_file.eof()) {
            std::getline(product_file, line1);
            if (line1 == "") continue; // Skip blank line
            ++count2;
        }
        product_file.close();
    }

    // Product Data file
    std::unique_ptr<InputReader> Filereader2;
    Filereader2 = InputReaderFactory<4, 8, 5, 1, 2, 6, 6, 8, 8>(product_filename);

    // Store product data in vectors
    for (int a{0} ; count2 > a ; ++a) {
        product_data_.institution_code.emplace_back(Filereader2->GetNextField());
        product_data_.product_code.emplace_back(Filereader2->GetNextField());
        product_data_.interest_rate.emplace_back(Filereader2->GetNextField());
        product_data_.compounding_period.emplace_back(Filereader2->GetNextField());
        product_data_.tie_in.emplace_back(Filereader2->GetNextField());
        product_data_.min_investment.emplace_back(Filereader2->GetNextField());
        product_data_.max_investment.emplace_back(Filereader2->GetNextField());
        product_data_.available_date.emplace_back(Filereader2->GetNextField());
        product_data_.maturity_date.emplace_back(Filereader2->GetNextField());
    }
    return product_data_;
}

ProductData InputData::GetProductData() const{
    return product_data_;
}

void InputData::PrintData(const std::string &client_filename, const std::string &product_filename) {
    // Populate data
    InputData::SetClientData(client_filename);
    InputData::SetProductData(product_filename);

    // Print client data
    std::cout << "------------Client Data------------" << std::endl;
    for (int b{0} ; client_data_.name.size() > b ; ++b) {
        std::cout << client_data_.name.at(b) << "   " << client_data_.capital.at(b) << "   " << client_data_.retirement_date.at(b) << std::endl;
    }

    // Print product feed
    std::cout << "------------------------Product Feed------------------------" << std::endl;
    for (int b{0} ; product_data_.institution_code.size() > b ; ++b) {
        std::cout << product_data_.institution_code.at(b) << "   " << product_data_.product_code.at(b) << "   " << product_data_.interest_rate.at(b) <<
                  "   " <<product_data_.compounding_period.at(b) << "   " << product_data_.tie_in.at(b) << "   " << product_data_.min_investment.at(b) <<
                  "   " <<  product_data_.max_investment.at(b) << "   " <<  product_data_.available_date.at(b) << "   " <<  product_data_.maturity_date.at(b)
                  << std::endl;
    }
}


