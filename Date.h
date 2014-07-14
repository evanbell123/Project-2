
#ifndef __DATE_H__
#define __DATE_H__

#include <stdexcept> 
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include "StringTokenizer.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

const int DAYS[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

enum DateFormat{Standard, US}; //Standard format starts with year, the default is a standard format

class Date {

public:

	Date(DateFormat format = DateFormat::US) :format(format){}

	Date(string date,DateFormat format = DateFormat::US) :format(format){
		year = 1; month = 1; day = 1; 
		*this=parseDate(date,format);
	}


	Date(int year, int month, int day, DateFormat format = DateFormat::US) : year(year), month(month), day(day), format(format){
		if (!valid_date(year, month, day))
			throw std::exception(" The date is not valid");
	}

	string toString() const {
		switch (format){
		case DateFormat::US:
			return to_string(month) + "/" + to_string(day) + "/" + to_string(year);
		case DateFormat::Standard:
			return to_string(year) + "/" + to_string(month) + "/" + to_string(day);
		}
	}
	
	 static Date parseDate(string date, DateFormat format){
		
		String_Tokenizer st(date, "-/,");
		int year = 1, month = 1, day = 1;

		int index = 0;
		while (st.has_more_tokens()){
			switch (format){
			case DateFormat::Standard: //standard format: year month day
				switch (index){
				case 0:
					std::istringstream(st.next_token()) >> year;
					break;
				case 1:
					std::istringstream(st.next_token()) >> month;
					break;
				case 2:
					std::istringstream(st.next_token()) >> day;
					break;
				}
				break;

			case DateFormat::US: //US format: month day year
				switch (index){
				case 0:
					std::istringstream(st.next_token()) >> month;
					break;
				case 1:
					std::istringstream(st.next_token()) >> day;
					break;
				case 2:
					std::istringstream(st.next_token()) >> year;
					break;
				}
			}
			++index;
		}

		if (!valid_date(year, month, day)) 
			throw std::exception(" The date is not valid");

		return Date(year, month, day, format);

	}

	friend istream& operator >> (istream& in, Date& d) {
		string date;
		in >> date;
		d=parseDate(date,d.format);
		
		return in;
	}


	bool operator <(Date other){
		if (year != other.year)
			return year < other.year;

		if (month != other.month)
			return month < other.month;

		return day < other.day;
	}

	void check_valid(){
		if (!valid_date(year, month, day))
			throw std::exception("The date is invalid");
	}

	static bool valid_date(int year, int month, int day){

		if (year>0 && month >= 1 && month <= 12 && day >= 1 && day <= 31){

			if (is_leap_year(year) && month == 2) // check for the leap year case
				return day <= 29;

			return day <= DAYS[month - 1];
		}
	}

	static bool is_leap_year(int year){

		//Algorithm is taken from Wikipedia: http://en.wikipedia.org/wiki/Leap_year
		if (year % 4 != 0)
			return false;
		else if (year % 100 != 0)
			return true;
		else if (year % 400 != 0)
			return false;
		else
			return true;
	}

	void add_days(int days){
		int reminder = days;

		while (reminder > 0){

			int limit = is_leap_year(year) && month == 2 ? 29 : DAYS[month - 1];

			if (reminder + day <= limit){ // applies to all months
				day += reminder;
				reminder = 0;
			}
			else if (month == 12){ //treat December differently
				year++;
				month = 1;
				reminder -= limit - day;
				day = 0;
			}
			else { //applies to all other months
				month++;
				reminder -= limit - day;
				day = 0;
			}
		}
	}

	int operator - (Date other){ //difference in number of days
		int year_r=0, month_r=0, day_r=0;

		if (this->operator<(other))
			throw std::invalid_argument("This date is < the given data");
		
		year_r = (year - other.year) * 365;
		int larger_month = month>other.month ? month : other.month;
		int smaller_month = month<other.month ? month : other.month;

		while (larger_month - smaller_month> 0){
			month_r += DAYS[larger_month - 1];
			larger_month--;
		}

		if (other.month > month)
			month_r *= -1;

		day_r = day - other.day;

		return day_r + month_r + year_r;

	}

	bool operator >(Date other){
		if (year != other.year)
			return year > other.year;

		if (month != other.month)
			return month > other.month;

		return day > other.day;
	}

	bool operator ==(Date other) {
		return year == other.year && month == other.month && day == other.day;
	}

	bool operator !=(const Date other) {
		return !(*this == other);
	}

	bool operator ==(Date other) const{
		return year == other.year && month == other.month && day == other.day;
	}

	bool operator !=(const Date other) const{
		return !(*this == other);
	}

	bool operator <=(Date other){
		return this->operator<(other) || this->operator==(other);
	}

	bool operator >=(Date other){
		return this->operator>(other) || this->operator==(other);
	}

	Date& operator =(const Date &rhs)
	{
		if (*this == rhs)
			return *this;

		setYear(rhs.year);
		setMonth(rhs.month);
		setDay(rhs.day);

		return *this;
	}

	int getYear() { return year; }
	int getMonth() { return month; }
	int getDay() { return day; }

	void setYear(int theYear) { year = theYear; check_valid(); }
	void setMonth(int theMonth) { month = theMonth; check_valid(); }
	void setDay(int theDay) { day = theDay; check_valid(); }


private:
	int year;
	int month;
	int day;
	DateFormat format;

};

#endif 