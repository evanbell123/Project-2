#include <fstream>
#include <string>
#include "Library.h"
#include "StringTokenizer.h"
#include "HelperFunctions.h"
using namespace std;

void Library::ReturnToLibrary(Periodical& p, Employee& e, Date currentDate)
{//Jordan
	p.setCheckedBool(false);
	e.removeBookFromList(p.getBarcode());
    UpdateEmployeeReliability(e, p, currentDate);

	if (p.morePeopleInQueue())
	{
		p.passToNextEmployee(currentDate);
	}
    else
    {
		map<int, Periodical>::iterator iter = circulatingPeriodicals.begin();
		archivedPeriodicals.insert(iter, pair <int, Periodical>(p.getBarcode(), p));
		circulatingPeriodicals.erase(p.getBarcode());
    }
	e.removeBookFromList(p.getBarcode());
	UpdateEmployeeReliability(e, p, currentDate);

}

void Library::ExchangePeriodical(Periodical& p, Employee& e1, Employee& e2, Date currentDate)
{

}

void Library::UpdateEmployeeReliability(Employee& e, Periodical& p, Date& currentDate)
{//Jordan
    if (currentDate > p.getReturnDate())
	{
		e.setReliability(e.getReliability() + (currentDate - p.getReturnDate()));
	}
	else
	{
		e.setReliability(e.getReliability() - (currentDate - p.getReturnDate()));
        if (e.getReliability() < 0)
        {
            e.setReliability(0);
        }
	}
}


Library Library::checkoutPeriodical(Periodical& p, Employee& e, Date currentDate) // Jordan
{
	// Evan
	if (p.getCheckOutStatus() == true)
	{
		p.empQueue.push(e);
		return *this;
	}
	if (e.isVacationing(currentDate))
	{
		return *this;
	}
	if (p.empQueue.empty())
	{
		p.setCheckOutDate(currentDate);
		p.setReturnDate();
		p.setCheckedBool(true);
		e.addBookToList(p.getBarcode());

		CirculatePeriodical(p);
		removeArchivedPeriodical(p);
		return *this;
	}


	/* 
	// Jordan
	if (p.getCheckOutStatus() == true)
	{
		p.empQueue.push(e);
		return;
	}
	map<int,Employee> popMap = UpdateQueue(p, currentDate);
    if (!popMap.empty())
    {
        p.empQueue.top().addBookToList(p.getBarcode());
        int dayswaiting = currentDate - p.getReturnDate();
        p.empQueue.top().setWaitingTime(p.empQueue.top().getWaitingTime() + dayswaiting);
        p.empQueue.pop();
        for (int i = 0; i < popMap.size(); i ++)
        {
            p.empQueue.push(popMap.begin()->second);
            popMap.erase(popMap.begin());
        }
        return;
    }
	p.setCheckOutDate(currentDate);
	p.setReturnDate();
	p.setCheckedBool(true);
	e.addBookToList(p.getBarcode());
	removeArchivedPeriodical(p);
	CirculatePeriodical(p);


	if (p.getCheckOutStatus() == true)
		throw::exception("the periodical you want is currently checked out");
    if (p.empQueue.empty())
    {
		if (currentDate > e.getVacationEnd() || currentDate < e.getVacationStart())
		{
			p.setCheckOutDate(currentDate);
			p.setReturnDate();
			p.setCheckedBool(true);
			e.addBookToList(p.getBarcode());
			Periodical temp = Periodical(p);
			removeArchivedPeriodical(p);
		}
		else
		{
			p.empQueue.push(e);
		}
    }
	*/
}


void Library::ReadPeriodicalsFromFile()
{//Evan w/ Brenton debug

	ifstream fin("Periodicals.txt");
	if (fin)
	{
		string line, aName;
		int aBarCode;
		map<int, Periodical>::iterator iter = archivedPeriodicals.begin();

		while (getline(fin, line))
		{
			String_Tokenizer st(line, ",");
			aName = trim(st.next_token());
			aBarCode = stoi(trim(st.next_token()));
            //circulatingPeriodicals[aBarCode] = Periodical(aName, aBarCode);
			ArchivePeriodical(Periodical(aName, aBarCode));
		}
	}
	fin.close();
}

void Library::ReadEmployeesFromFile()
{//Evan w/ Brenton debug
	ifstream fin("Employees.txt");
	if (fin)
	{
		string line, empName, startVacation, endVacation;
		int theReliability, theWaitingTime;

		while (getline(fin, line))
		{
			String_Tokenizer st(line, ",");
			empName = trim(st.next_token());
			theReliability = stoi(trim(st.next_token()));
			theWaitingTime = stoi(trim(st.next_token()));
			startVacation = trim(st.next_token());
			endVacation = trim(st.next_token());
			employees[empName] = Employee(theReliability, empName, Date(startVacation, DateFormat::US), Date(endVacation, DateFormat::US), theWaitingTime);
		}
	}
	fin.close();
}
// Returns Date of next batch of actions
// if end of file has been reached return empty Date
void Library::ReadActionsFromFile() // Evan
{
	ifstream actionsFile("Actions.txt");
	if (actionsFile.fail()){
		cout << "Couldn't open actions.txt\n";
		system("pause");
		exit(1);
	}

	while (!actionsFile.eof())
	{
		string line, name;//, action
		int aBarcode, action;
		Date currentDate;

		getline(actionsFile, line);

		if (line == ""){
			continue;
		}
		else if (isDate(line))
		{
			currentDate = Date(line);
		}
		else {
			String_Tokenizer st(line, ",");

			name = trim(st.next_token());
			action = stoi(trim(st.next_token()));
			aBarcode = stoi(trim(st.next_token()));
			
			switch (action)
			{
			case 1:
				checkoutPeriodical(archivedPeriodicals[aBarcode], employees[name], currentDate);
				break;
			case 2:
				ReturnToLibrary(circulatingPeriodicals[aBarcode], employees[name], currentDate);
				break;
			case 3:
			{
				string name2 = trim(st.next_token());
				ExchangePeriodical(circulatingPeriodicals[aBarcode], employees[name], employees[name2], currentDate);
			}
				break;
			default:
				throw::exception("Invalid action call from file");
				break;
			}
			
		}
    }
}

void Library::buildPriorityQueues(Date currentDate){
	//Brenton
	for (map<int, Periodical>::iterator itr = circulatingPeriodicals.begin(); itr != circulatingPeriodicals.end(); itr++){
		itr->second.generateEmpQueue(employees);

		Employee firstEmployee = itr->second.passToNextEmployee(currentDate);
		firstEmployee.addBookToList(itr->first);
	}
   
}

void Library::ArchivePeriodical(Periodical& p) // Evan
{
	map<int, Periodical>::iterator iter = archivedPeriodicals.begin();
	archivedPeriodicals.insert(iter, pair <int, Periodical>(p.getBarcode(), p));
}


void Library::removeArchivedPeriodical(Periodical& p) // Evan
{
	if (archivedPeriodicals.empty())
		throw::exception("Can't remove an element from an empty");
	/* this wasn't working right
	map<int, Periodical>::iterator iter = archivedPeriodicals.find(p.getBarcode());
	archivedPeriodicals.erase(iter);
	*/
	archivedPeriodicals.erase(next(archivedPeriodicals.find(p.getBarcode())));
}

void Library::CirculatePeriodical(Periodical& p) // Evan
{
	map<int, Periodical>::iterator iter = circulatingPeriodicals.begin();
	circulatingPeriodicals.insert(iter, pair <int, Periodical>(p.getBarcode(), p));
}

void Library::removeCirculatingPeriodical(Periodical& p) // Evan
{
	if (circulatingPeriodicals.empty())
		throw::exception("Can't remove an element from an empty");
	/* this wasn't working right
	map<int, Periodical>::iterator iter = circulatingPeriodicals.find(p.getBarcode());
    circulatingPeriodicals.erase(iter);
	*/
	circulatingPeriodicals.erase(next(circulatingPeriodicals.find(p.getBarcode())));
}

void Library::displayLibraryInfo()
{
	cout << "Archived Periodicals" << endl;
	displayPeriodicalMap(archivedPeriodicals);
	cout << endl << "Circulating Periodicals" << endl;
	displayPeriodicalMap(circulatingPeriodicals);
	cout << endl << "Employees" << endl;
	displayEmployeeMap(employees);
}

void Library::displayPeriodicalMap(const map<int, Periodical> periodicalMap)
{
	map<int, Periodical>::const_iterator constIter = periodicalMap.begin();
	for (constIter; constIter != periodicalMap.end(); constIter++)
	{
		cout << constIter->first << "\t" << constIter->second.getName() << endl;
	}
}

void Library::displayEmployeeMap(const map<string, Employee> employeeMap)
{
	map<string, Employee>::const_iterator constIter = employeeMap.begin();
	for (constIter; constIter != employeeMap.end(); constIter++)
	{
		cout << constIter->first << endl;
	}
}