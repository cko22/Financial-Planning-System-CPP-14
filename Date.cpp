// Student ID 2031611
#include <iostream>
#include <string>

#include "Date.hpp"

DateFormat Date::SetDateFormat(int some_date) {
    // Take the last 2 digits as day.
    date_format_.day = some_date%100;
    some_date /= 100; // remove them.

    //Take the last 2 digits from new some_date as month.
    date_format_.month = some_date%100;
    some_date /= 100; // again remove them.

    // The remaining 4 digits is year.
    date_format_.year = some_date;
    return date_format_;
}

DateFormat Date::GetDateFormat() const {
    return date_format_;
}

int Date::GetStartDate() const {
    return start_date_;
}

int Date::GetMonthlyDays(const int &index) {
    return monthly_days_.at(index);
}

int Date::DaysToDate(const int &num_days) {
    int y = num_days / 365; // year
    int d = num_days % 365; // day
    int m; // month

    Date dateobj;
    int i{0};
    if (0 == d) {
        --y;
        m = 12;
        d = 31;
    } else {
        while (0 < d) {
            d = d - dateobj.GetMonthlyDays (i);
            ++i;
        }
        d = d + dateobj.GetMonthlyDays (i-1);
        m = i;
    }

    // Convert them to string
    std::string year = std::to_string(y);
    std::string month;
    if (10 > m) {
        // make it double digits (consistent)
        month = std::to_string(0) + std::to_string(m);
    } else {
        month = std::to_string(m);
    }
    std::string day;
    if (10 > d) {
        // make it double digits (consistent)
        day = std::to_string(0) + std::to_string(d);
    } else {
        day = std::to_string(d);
    }

    // Concatenate them
    std::string a = year + month + day;

    // Convert to int
    int new_date = std::stoi(a);

    return new_date;
}

int Date::DateToDays(const int &date_a) {
    // Assuming no leap years.
    // Populate date format.
    DateFormat dateobj;
    Date dateobj1;
    dateobj = dateobj1.SetDateFormat(date_a);

    // Safety check
    if (12 < dateobj.month){
        std::cerr << "Month cannot be higher than 12! (Valid range is 1 to 12)" << std::endl;
        exit(13);
    }
    if (31 < dateobj.day ){
        std::cerr << "Day cannot be higher than 31! (Valid range is 1 to 31)" << std::endl;
        exit(32);
    }

    // Calculate number of days
    int d1 = dateobj.year*365 + dateobj.day; // Day count from year and day.
    for (int i{0} ; dateobj.month - 1 > i ; ++i) { // Day count from month.
        d1 += dateobj1.GetMonthlyDays(i);
    }

    return d1;
}

int Date::CompletedAnniversaries(const int &date_1, const int &date_2, const std::string &period){
    // Make sure data_2 is larger than date_1
    if (date_2 <= date_1) {
        std::cerr << "Second date must be larger than first one!!!" << std::endl;
        exit(950705);
    }

    // Decompose the date (YYYYMMDD) into year, month, day format.
    DateFormat dateobj1;
    DateFormat dateobj2;
    dateobj1 = Date::SetDateFormat(date_1);
    dateobj2 = Date::SetDateFormat(date_2);

    // Calculate the number of days apart
    int num_days1 = DateToDays(date_1);
    int num_days2 = DateToDays(date_2);
    int diff_days = num_days2 - num_days1;

    // 3 possible period inputs: A or M or neither.
    if ("A" == period) {
        // Calculate the number of years apart
        int num_years = diff_days/365;
        return num_years;
    } else if ("M" == period) {
        int counter{0};
        int index = dateobj1.month - 1;
        while (0 <= diff_days) {
            diff_days = diff_days - monthly_days_.at(index);
            ++index; // proceed to next month

            // Reset to January (index = 0) if December is reached (index = 11)
            if (12 == index) {
                index = 0;
            }
            ++counter; // Record how many loops
        }
        // If monthly anniversary falls on 29th-31st in Feb or on 31st in months with 30 days)
        if ((28 < dateobj1.day && 02 == dateobj2.month) || (31 ==  dateobj1.day && 30 == monthly_days_.at(dateobj2.month-1))) {
            // +1 counter as it is considered a completed period.
            ++counter;
        }

        // No. of complete months is total counter - 1 (just before diff_days becomes negative)
        return (counter - 1);
    } else {
        std::cerr << "Period must be 'A' or 'M'" << std::endl;
        exit (100); // error
    }
}