
#ifndef INC_2031611_DATE_HPP
#define INC_2031611_DATE_HPP


#include <array>

struct DateFormat {
    int year;
    int month;
    int day;
};

class Date {
private:
    DateFormat date_format_;
    const int start_date_ = 20200405; // start date marked const
    // No. of days in each month from Jan to Dec
    const std::array <int,12> monthly_days_ = {31, 28, 31, 30, 31,
                                               30, 31, 31, 30, 31,
                                               30, 31};
public:
    DateFormat SetDateFormat(int some_date);

    DateFormat GetDateFormat() const;

    int GetStartDate() const;

    int GetMonthlyDays(const int &index);

    /*
    * Returns date from no. of days
    *
    * Args:
    *  Number of days
    *
    * Returns:
    *  Date in the form of YYYYMMDD
    */
    int DaysToDate(const int &num_days);

    /*
    * Returns no. of days of a date
    *
    * Args:
    *  Date in the form of YYYYMMDD
    *
    * Returns:
    *  No. of days
    */
    int DateToDays(const int &date_a);

    /*
    * Returns no. of completed anniversaries between 2 dates
    *
    * Args:
    *  - Date 1 in the form of YYYYMMDD
    *  - Date 2 in the form of YYYYMMDD (larger)
    *  - Period (A or M)
    *
    * Returns:
    *  No. of completed anniversaries.
    */
    int CompletedAnniversaries(const int &date_1, const int &date_2, const std::string &period);
};


#endif //INC_2031611_DATE_HPP
