/*
Employee class header
*/

#ifndef __EMPLOYEE_H__
#define __EMPLOYEE_H__
#include <string>
#include <iostream>
#include <vector>
#include "Periodical.h"
#include "Date.h"
using namespace std;

class Employee { //implemented by Jordan
public:
	//default constructor
	Employee() : reliability(), empname(), vacationStart(Date()), vacationEnd(Date()), waiting_time() {}

	/*
	//full-argument constructor
	Employee(int aPriority, int aReliability, string aName, Date aVacStart, Date aVacEnd, int aWaitTime) :
	priority(aPriority), reliability(aReliability), empname(aName), vacationStart(aVacStart),
	vacationEnd(aVacEnd), waiting_time(aWaitTime) {}
	*/

	Employee(int aReliability, string aName, Date aVacStart, Date aVacEnd, int aWaitTime)
		: reliability(aReliability), empname(aName), vacationStart(aVacStart), vacationEnd(aVacEnd), waiting_time(aWaitTime){}

	//copy constructor
	Employee(const Employee& e) : reliability(e.reliability), empname(e.empname),
		vacationStart(e.vacationStart), vacationEnd(e.vacationEnd), waiting_time(e.waiting_time) {}


	void setReliability(const int& r) { reliability = r; }
	void setVacationStart(const Date& d) { vacationStart = d; }
	void setVacationEnd(const Date& d) { vacationEnd = d; }
	void setWaitingTime(const int& t) { waiting_time = t; }

	void addBookToList(const int& b) { BookListByID.push_back(b); }
	void removeBookFromList(const int& b) { //Jordan w/ evan debug

		if (BookListByID.empty())
			throw::exception("Can't remove an item from an empty vector");
		
		vector<int>::iterator iter;
		for (iter = BookListByID.begin(); iter != BookListByID.end(); iter++)
		{
			if (*iter == b)
			{
				BookListByID.erase(iter);
				return;
			}
		}
		throw::exception("The periodical is not in this employees periodical list.\n");
		
	}

	//getters
	Date getVacationStart() const { return vacationStart; }
	Date getVacationEnd() const { return vacationEnd; }
	int getReliability() const { return reliability; }
	string getEmpname() const { return empname; }
	int getWaitingTime() const { return waiting_time; }
	int getLateDays() { return 0; }

	bool isVacationing(Date currentDate){
		return currentDate >= vacationStart && currentDate <= vacationEnd;
	}

private:
	string empname;
	int reliability;
	Date vacationStart;
	Date vacationEnd;
	vector<int> BookListByID;
	int waiting_time; // # of days
};

#endif