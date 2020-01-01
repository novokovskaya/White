#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <iomanip>
#include <exception>
//#include <algorithm>
using namespace std;
class Date {
public:
	Date(int Y, int M, int D){
		if (M < 1 || M > 12) {
			throw invalid_argument("Month value is invalid: " + to_string(M));
		}
		if (D < 1 || D > 31) {
			throw invalid_argument("Day value is invalid: " + to_string(D));
		}
		year = Y;
		month = M;
		day = D;
	}
	int GetYear() const { return year; }
	int GetMonth() const { return month; }
	int GetDay() const { return day; }
private:
	int year, month, day;
};
bool operator<(const Date& lhs, const Date& rhs) {
	if (lhs.GetYear() < rhs.GetYear())return true;
	else if (lhs.GetYear() == rhs.GetYear()) {
		if (lhs.GetMonth() < rhs.GetMonth())return true;
		else if (lhs.GetMonth() == rhs.GetMonth()) {
			if (lhs.GetDay() < rhs.GetDay())return true;
		}
	}
	return false;
}
ostream& operator<<(ostream& stream, const Date& r) {
	stream << setfill('0') 
		<< setw(4) << r.GetYear() << '-'
		<< setw(2) << r.GetMonth() << '-'
		<< setw(2) << r.GetDay();
	return stream;
}
class Database {
public:
	void AddEvent(const Date& date, const string& event) {
		Base[date].insert(event);
	}
	bool DeleteEvent(const Date& date, const string& event1) {
		if (Base[date].count(event1) > 0) {
			Base[date].erase(event1);
			return true;
		}
		return false;
	}
	int  DeleteDate(const Date& date) {
		int c = 0;
		if (Base.count(date) != 0) {
			c = Base[date].size();
			Base.erase(date);
		}
		return c;
	}
	void Print() const {
		for (auto& item : Base) {
			for (auto& item1 : item.second) {
				cout << item.first << " " << item1 << endl;
			}
		}
	}
	void Find(const Date& date) const {
		if (Base.count(date) != 0) {
			for (auto& item : Base.at(date)) {
				cout << item << endl;
			}
		}
	}
private:
	map<Date, set<string>> Base;
};
void EnsureNextSymbolAndSkip(stringstream& stream, string& date) {
	if (stream.peek() != '-') {
		throw invalid_argument("Wrong date format: " + date);
	}
	stream.ignore(1);
}
Date DatePars(string& date) {
	stringstream ss(date);
	int Year, Month, Day;
	string Tail;
	if (!(ss >> Year)) { throw invalid_argument("Wrong date format: " + date); }
	EnsureNextSymbolAndSkip(ss, date);
	if (!(ss >> Month)){ throw invalid_argument("Wrong date format: " + date); }
	EnsureNextSymbolAndSkip(ss, date);
	if(!(ss >> Day)) {throw invalid_argument("Wrong date format: " + date);}
	ss >> Tail;
	if (Tail.size() != 0) {throw invalid_argument("Wrong date format: " + date);
	}
	return Date(Year, Month, Day);
}
int main() {
	Database dbase;
	string command;
	while (getline(cin, command)) {
		if (command.size() == 0)continue;
		stringstream ss(command);
		string act;
		ss >> act;
		try {
			if (act == "Add") {
				string date, event;
				ss >> date;
				ss >> event;
				Date tmp = DatePars(date);
				dbase.AddEvent(tmp, event);

			}
			else if (act == "Print") {
				dbase.Print();
			}
			else if (act == "Del") {
				string date, event;
				ss >> date;
				ss >> event;
				Date tmp = DatePars(date);
				if (event.size() != 0) {
					if (dbase.DeleteEvent(tmp, event)) { cout << "Deleted successfully" << endl; }
					else { cout << "Event not found" << endl; }
				}
				else {
					cout << "Deleted " << dbase.DeleteDate(tmp) << " events" << endl;
				}
			}
			else if (act == "Find") {
				string date;
				ss >> date;
				Date tmp = DatePars(date);
				dbase.Find(tmp);
			}
			else { throw invalid_argument("Unknown command: " + act); }
		}
		catch (exception& ex) { cout << ex.what() << endl; }
	}
	cin.get(); cin.get();
	return 0;
}