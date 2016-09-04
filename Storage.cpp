#include <vector>
#include <sstream>
#include <fstream>
#include <iterator>
#include <iostream>
#include <string>
#include "Storage.hpp"
#include "Path.hpp" 
using namespace std;

std::shared_ptr<Storage> Storage::m_instance = NULL;

vector<string> divide(string s) {
	vector<string> res;
	int last = 0;
	for (int i = 1; i < s.size(); i++) {
		if(s[i] == '"') {
			string temp;
			temp.assign(s, last + 1, i - last - 1);
			res.push_back(temp);
			last = i + 2;
			i += 2;
		}
	}
	return res;
}

vector<string> divide2(string s) {
	int last = 0;
	vector<string> res;
	string aa = "&";
	if(s.find(aa) == string::npos) {
		res.push_back(s);
		return res;
	}
	for(int i = 1; i < s.size(); i++) {
		if(s[i] == '&') {
			string temp;
			temp.assign(s, last, i - last);
			res.push_back(temp);
			last = i + 1;
		}
		if(i == s.size() - 1) {
			string temp;
			temp.assign(s, last, i - last + 1);
			res.push_back(temp);
			break;
		}
	}
	return res;
}

Storage::Storage() {
	this -> readFromFile(); 
}

bool Storage::readFromFile(void) {
	std::ifstream file, file1;
	file.open(Path::userPath);
	if(!file) return false;
	string temp;
	//std::getline(file, temp);
	while(true) {
		std::getline(file, temp);
		if(temp == "") break;
		vector<string> res = divide(temp);
		User usertemp(res[0], res[1], res[2], res[3]);
		m_userList.push_back(usertemp);
	}
	file.close();

    file1.open(Path::meetingPath);
    if(!file1) return false;
    //std::getline(file1, temp);
    while(true) {
    	std::getline(file1, temp);
    	if(temp == "") break;
    	vector<string> res = divide(temp);
    	vector<string> par = divide2(res[1]);
    	//cout << "res " << res[2] << " " << res[3] << endl; 
    	Date date1 = Date::stringToDate(res[2]);
    	Date date2 = Date::stringToDate(res[3]);
    	Meeting meettemp(res[0], par, date1, date2, res[4]);
    	m_meetingList.push_back(meettemp);
    }
    file1.close();
    return true;
}

bool Storage::writeToFile(void) {
	std::ofstream file;
	file.open(Path::userPath);
	if(!file) return false;
	//file << "\"name\", \"password\", \"email\",\"phone\"" << endl;
	for(auto p = m_userList.begin(); p != m_userList.end(); p++) {
		string n = (*p).getName();
		string pass = (*p).getPassword();
		string e = (*p).getEmail();
		string ph = (*p).getPhone();
		string all = "\"" + n + "\",\"" + pass + "\",\"" + e + "\",\"" + ph + "\"";
		file << all << endl;
	}
	file.close();

	file.open(Path::meetingPath);
	if(!file) return false;
	//file << "\"sponsor\",\"participators\",\"sdate\",\"edate\",\"title\"" << endl;
	for(auto p = m_meetingList.begin(); p != m_meetingList.end(); p++) {
		string sp = (*p).getSponsor();
		vector<string> par = (*p).getParticipator();
		string temp = par[0];
		for(int i = 1; i < par.size(); i++) {
			temp += "&" + par[i];
     	}
     	string st = Date::dateToString((*p).getStartDate());
     	string en = Date::dateToString((*p).getEndDate());
     	string ti = (*p).getTitle();
     	string all = "\"" + sp + "\",\"" + temp + "\",\"" + st + "\",\"" + en + "\",\"" + ti + "\"";
     	file << all << endl;
    }	
    file.close();
    return true;
}


shared_ptr<Storage> Storage::getInstance(void) {
	if(m_instance == NULL) {
		m_instance = shared_ptr<Storage>(new Storage());
	}
	return m_instance;
}

Storage::~Storage() {
	writeToFile();
}

void Storage::createUser(const User & t_user) {
	m_userList.push_back(t_user);
	m_dirty = true;
}

std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const {
	std::list<User> res;
	for(auto p = m_userList.begin(); p != m_userList.end(); p++) {
		if(filter(*p) == true) {
			res.push_back(*p);
		}
	}
	return res;
}

int Storage::updateUser(std::function<bool(const User &)> filter,
                 std::function<void(User &)> switcher) {
	int all = 0;
	for(auto p = m_userList.begin(); p != m_userList.end(); p++) {
		if(filter(*p) == true) {
			switcher(*p);
			m_dirty = true;
			all++;
		}
	}
	//sync();
	return all;
}

int Storage::deleteUser(std::function<bool(const User &)> filter) {
	int res = 0;
	for(auto p = m_userList.begin(); p != m_userList.end();) {
		if(filter(*p) == true) {
			p = m_userList.erase(p);
			res++;
			m_dirty = true;
		} else {
			p++;
		}
	}
	//sync();
	return res;
}

void Storage::createMeeting(const Meeting & t_meeting) {
	m_meetingList.push_back(t_meeting);
	m_dirty = true;
	//sync();
}

std::list<Meeting> Storage::queryMeeting(std::function<bool(const Meeting &)> filter) const {
	std::list<Meeting> res;
	for(auto p = m_meetingList.begin(); p != m_meetingList.end(); p++) {
		if(filter(*p)) {
			res.push_back(*p);
		}
	}
	return res;
}

int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
                    std::function<void(Meeting &)> switcher) {
	int all = 0;
	for (auto p = m_meetingList.begin(); p != m_meetingList.end(); p++) {
		if(filter(*p) == true) {
			switcher(*p);
			m_dirty = true;
			all++;
		}
	}
	//sync();
	return all;
}

int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
	int all = 0;
	for (auto p = m_meetingList.begin(); p != m_meetingList.end(); ) {
		if(filter(*p)) {
			p = m_meetingList.erase(p);
			all++;
			m_dirty = true;
		} else {
			p++;
		}
	}
	//sync();
	return all;
}

bool Storage::sync(void) {
	bool flag = writeToFile();
	if(flag) {
		m_dirty = false;
		return true;
	} else {
		return false;
	}
}
