#include "Date.hpp"
#include <sstream>
#include <iterator>
#include <iostream>
using namespace std;

bool isleap(int year) {

	if(year % 400 == 0) return true;
	if(year % 4 == 0 && year % 100 != 0) return true;
	return false;
}

Date::Date() {
	m_year = 0;
	m_month = 0;
	m_day = 0;
	m_hour = 0;
	m_minute  = 0;
}

Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) {
	m_year = t_year;
	m_month = t_month;
	m_day = t_day;
	m_hour = t_hour;
	m_minute = t_minute;
}

Date::Date(std::string dateString) {
	Date temp = Date::stringToDate(dateString);
	m_year = temp.m_year;
	m_month = temp.m_month;
	m_day = temp.m_day;
	m_hour = temp.m_hour;
	m_minute = temp.m_minute;
}

int Date::getYear(void) const {
	return m_year;
}

void Date::setYear(const int t_year) {
	m_year = t_year;
}

int Date::getMonth(void) const {
	return m_month;
}

void Date::setMonth(const int t_month) {
	m_month = t_month;
}

int Date::getDay(void) const {
	return m_day;
}

void Date::setDay(const int t_day) {
	m_day = t_day;
}

int Date::getHour(void) const {
	return m_hour;
}

void Date::setHour(const int t_hour) {
	m_hour = t_hour;
}

int Date::getMinute(void) const {
	return m_minute;
}

void Date::setMinute(const int t_minute) {
	m_minute = t_minute;
}

bool Date::isValid(const Date t_date) {
	if(t_date.m_year < 1000 || t_date.m_year >9999) return false;
	//cout << "1" << endl;
	if(t_date.m_month < 1 || t_date.m_month > 12) return false;
	//cout << "2" << endl;
	if(t_date.m_minute < 0 || t_date.m_minute > 59) return false;
	//cout << "3" << endl;
	if(t_date.m_hour < 0 || t_date.m_hour > 23 || t_date.m_day < 1 || t_date.m_day > 31) return false;
	//cout << "4" << endl;
	if((t_date.m_month == 4 || t_date.m_month == 6 || t_date.m_month == 9 || t_date.m_month == 11)
		&& t_date.m_day > 30) 
		return false;
	//cout << "6" << endl;
	if(t_date.m_month == 2) {
		//cout << "leap" << isleap(t_date.m_month) << endl;
	    if(isleap(t_date.m_year) && t_date.m_day >29) return false;
	    if(!isleap(t_date.m_year) && t_date.m_day > 28) { return false;}
	}
	//cout << "7" << endl;
	return true;
}

Date Date::stringToDate(const std::string t_dateString) {
	//cout << "1" << endl;
	const std::string da = t_dateString;
	int totol = 0;
	for (int i = 0; i < da.size(); i++) {
		if(i != 4 && i != 7 && i != 10 && i != 13) {
			if(da[i] < '0' || da[i] > '9') {
				totol++;
				break;
			}
		}
	}
	if(t_dateString.size() != 16 || da[4] != '-' || da[7] != '-' || da[10] != '/' || da[13] != ':' || totol != 0) {
		//cout << "fail" << endl;
	    Date temp(0, 0, 0, 0, 0);
	    return temp;
	} else {
		//cout <<  da << endl;
		int temp11, temp22, temp33, temp44, temp55;
	    temp11 = (da[0] - '0') * 1000 + (da[1] - '0') * 100 +(da[2] - '0') * 10 + (da[3] - '0');
	    temp22 = (da[5] - '0') * 10 + da[6] - '0';
	    temp33 = (da[8] - '0') * 10 + da[9] - '0';
	    temp44 = (da[11] - '0') * 10 + da[12] - '0';
	    temp55 = (da[14] - '0') * 10 + da[15] - '0';
	    Date temp(temp11, temp22, temp33, temp44, temp55);
	    return temp;
	}
}

std::string Date::dateToString(Date t_date) {
	if(!isValid(t_date)) { return "0000-00-00/00:00";}
	string year = "year", month = "mm", day = "dd", hour = "hh", minute = "mm";
	year[0] = (t_date.m_year / 1000) + '0';
	year[1] = (t_date.m_year / 100 - t_date.m_year / 1000 * 10) + '0';
	year[2] = (t_date.m_year / 10 - t_date.m_year / 100 * 10) + '0';
	year[3] = (t_date.m_year - t_date.m_year / 10 * 10) + '0';
	month[0] = t_date.m_month / 10 + '0';
	month[1] = t_date.m_month % 10 +'0';
	day[0] = t_date.m_day / 10 + '0';
	day[1] = t_date.m_day % 10 +'0';
	hour[0] = t_date.m_hour / 10 + '0';
	hour[1] = t_date.m_hour % 10 + '0';
	minute[0] = t_date.m_minute / 10 + '0';
	minute[1] = t_date.m_minute % 10 + '0';
	std::string all = year + "-" + month + "-" + day + "/" + hour + ":" + minute;
	return all;
}

Date &Date::operator=(const Date &t_date) {
	m_year = t_date.m_year;
	m_month = t_date.m_month;
	m_day = t_date.m_day;
	m_hour = t_date.m_hour;
	m_minute = t_date.m_minute;
	return *this;
}

bool Date::operator==(const Date &t_date) const {
    if(m_year == t_date.m_year && m_month == t_date.m_month && m_day == t_date.m_day
    	&& m_hour == t_date.m_hour && m_minute == t_date.m_minute) {
    	return true;
    } else {
    	return false;
    }
}

bool Date::operator>(const Date &t_date) const {
	if(m_year > t_date.m_year) return true;
	if(m_year == t_date.m_year && m_month > t_date.m_month) return true;
	if(m_year == t_date.m_year && m_month == t_date.m_month && m_day > t_date.m_day) return true;
	if(m_year == t_date.m_year && m_month == t_date.m_month && m_day == t_date.m_day
		&& m_hour > t_date.m_hour) return true;
	if(m_year == t_date.m_year && m_month == t_date.m_month && m_day == t_date.m_day
		&& m_hour == t_date.m_hour && m_minute > t_date.m_minute) return true;
	return false;
}

bool Date::operator<(const Date &t_date) const {
	if(m_year < t_date.m_year) return true;
	if(m_year == t_date.m_year && m_month < t_date.m_month) return true;
	if(m_year == t_date.m_year && m_month == t_date.m_month && m_day < t_date.m_day) return true;
	if(m_year == t_date.m_year && m_month == t_date.m_month && m_day == t_date.m_day
		&& m_hour < t_date.m_hour) return true;
	if(m_year == t_date.m_year && m_month == t_date.m_month && m_day == t_date.m_day
		&& m_hour == t_date.m_hour && m_minute < t_date.m_minute) return true;
	return false;
}

bool Date::operator>=(const Date &t_date) const {
	if(m_year == t_date.m_year && m_month == t_date.m_month && m_day == t_date.m_day
    	&& m_hour == t_date.m_hour && m_minute == t_date.m_minute) {
    	return true;
    }
	if(m_year > t_date.m_year) return true;
	if(m_year == t_date.m_year && m_month > t_date.m_month) return true;
	if(m_year == t_date.m_year && m_month == t_date.m_month && m_day > t_date.m_day) return true;
	if(m_year == t_date.m_year && m_month == t_date.m_month && m_day == t_date.m_day
		&& m_hour > t_date.m_hour) return true;
	if(m_year == t_date.m_year && m_month == t_date.m_month && m_day == t_date.m_day
		&& m_hour == t_date.m_hour && m_minute > t_date.m_minute) return true;
	return false;
}

bool Date::operator<=(const Date &t_date) const {
	if(m_year == t_date.m_year && m_month == t_date.m_month && m_day == t_date.m_day
    	&& m_hour == t_date.m_hour && m_minute == t_date.m_minute) {
    	return true;
    }
    if(m_year < t_date.m_year) return true;
	if(m_year == t_date.m_year && m_month < t_date.m_month) return true;
	if(m_year == t_date.m_year && m_month == t_date.m_month && m_day < t_date.m_day) return true;
	if(m_year == t_date.m_year && m_month == t_date.m_month && m_day == t_date.m_day
		&& m_hour < t_date.m_hour) return true;
	if(m_year == t_date.m_year && m_month == t_date.m_month && m_day == t_date.m_day
		&& m_hour == t_date.m_hour && m_minute < t_date.m_minute) return true;
	return false;
}
