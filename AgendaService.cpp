#include "AgendaService.hpp"
#include <iterator>
#include <iostream>
#include <string>
using namespace std;

AgendaService::AgendaService() {
	startAgenda();
}

AgendaService::~AgendaService() {
	quitAgenda();
}

bool AgendaService::userLogIn(const std::string userName, const std::string password) {
	list<User> usertemp = m_storage -> queryUser([userName, password](const User &temp) -> bool {
		                                            if(userName == temp.getName() && password == temp.getPassword())
		                                                return true;
		                                            else return false;
	                                            });
	if(usertemp.empty()) {
		//cout << "wrong user name or password" << endl;
		return false;
	}
	return true;
}

bool AgendaService::userRegister(const std::string userName, const std::string password,
                      const std::string email, const std::string phone) {
	list<User> usertemp = m_storage -> queryUser([userName, password](const User &temp) -> bool {
		                                            if(userName == temp.getName())
		                                                return true;
		                                            else return false;
	                                            });
	if(!usertemp.empty()) {
		//cout << "the user name you register have been registered by others" << endl;
		return false;
	}
	User temp(userName, password, email, phone);
	m_storage -> createUser(temp);
	m_storage -> sync();
	return true;
}

bool AgendaService::deleteUser(const std::string userName, const std::string password) {
	int flag;
	flag = m_storage -> deleteUser([userName, password](const User &temp) -> bool {
		if(userName == temp.getName() && password == temp.getPassword()) 
			return true;
		else
			return false;
	});
	if(flag) {
		m_storage -> deleteMeeting([userName](const Meeting &temp) -> bool {
			if(userName == temp.getSponsor() || temp.isParticipator(userName)) return true;
			else return false;
		});
		m_storage -> sync();
		return true;
	}
	return false;
}

std::list<User> AgendaService::listAllUsers(void) const {
	list<User> usertemp = m_storage -> queryUser([](const User &temp) -> bool { 
		                                            return true;});
	return usertemp;
}

int AgendaService::createMeeting(const std::string userName, const std::string title,
                       const std::string startDate, const std::string endDate,
                       const std::vector<std::string> participator) {
	list<Meeting> meettemp = m_storage -> queryMeeting([userName](const Meeting &temp) -> bool {
                                                        if(userName == temp.getSponsor() || temp.isParticipator(userName)) 
                                                        	return true;
                                                        else 
                                                        	return false;
	                                                });
	list<User> usertemp = m_storage -> queryUser([](const User &temp) -> bool { 
		                                            return true;});
	Date start = Date::stringToDate(startDate);
    Date end = Date::stringToDate(endDate);
    if(participator.size() == 0) return 1;
    /* check sponsor if_is participator */
    for (int i = 0; i < participator.size(); i++) {
    	if(userName == participator[i]) return 2;
    }
    /* check if the same participator */
    for(int i = 0; i < participator.size(); i++) {
    	for (int j = 0; j < participator.size(); j++) {
    		if(participator[i] == participator[j] && i != j) return 3;
    	}
    }
	/* check Date */
	//cout << "0" << endl;
	if(!Date::isValid(startDate) || !Date::isValid(endDate) || startDate >= endDate) return 4;
	//cout << "1" << endl;
	/* check title*/
	list<Meeting> meet = m_storage -> queryMeeting([title](const Meeting &temp) -> bool {
                                                        if(title == temp.getTitle()) return true;
                                                        else return false;
	                                                });
	if(!meet.empty()) return 5;
	//cout << "2" << endl;
	/* check user is_user */
	bool flag = false;
	for (auto p = usertemp.begin(); p != usertemp.end(); p++) {
		if(userName == p -> getName()) {
			flag = true;
			break;
		}
	}
	if(!flag) return 6;
	//cout << "3" << endl;
	/* check participator is_user */
	for(int p = 0; p < participator.size(); p++) {
		string pp = participator[p];
		list<User> parti = m_storage -> queryUser( [pp](const User &temp) -> bool {
			if(pp == temp.getName()) return true;
			else return false;
		});
		if(parti.empty()) return 7;
	}
	//cout << "4" << endl;
	/* check sponsor overlap */
	for(auto i = meettemp.begin(); i != meettemp.end(); i++) {
		if (!(start >= i -> getEndDate() || end <= i -> getStartDate())) return 8;
	}
	//cout << "5" << endl;
	/* check participator overlap */
	for (int p = 0; p < participator.size(); p++) {
		string name = participator[p];
		list<Meeting> parmeet = listAllMeetings(name);
		for (auto m = parmeet.begin(); m != parmeet.end(); m++) {
			if(!(start >= m -> getEndDate() || end <= m -> getStartDate())) return 9;
		}
	}
	//cout << "6" << endl;
	Meeting temp(userName, participator, start, end, title);
	m_storage -> createMeeting(temp);
	m_storage -> sync();
	return 10;
}

std::list<Meeting> AgendaService::meetingQuery(const std::string userName,
                                    const std::string title) const {
	list<Meeting> res = m_storage -> queryMeeting([userName, title](const Meeting &temp) -> bool {
		if(userName == temp.getSponsor() || temp.isParticipator(userName)) {
			if(title == temp.getTitle())
				return true;
			else
				return false;
		} else {
			return false;
		}
	});
	return res;
}

std::list<Meeting> AgendaService::meetingQuery(const std::string userName,
                                const std::string startDate,
                                const std::string endDate) const {
	if(!Date::isValid(startDate) || !Date::isValid(endDate) || startDate > endDate) {
		list<Meeting> temp;
		return temp;
	}
	list<Meeting> res = m_storage -> queryMeeting([userName, startDate, endDate](const Meeting &temp) -> bool {
		if(userName == temp.getSponsor() || temp.isParticipator(userName)) 
			if((Date::stringToDate(startDate) <= temp.getStartDate() && Date::stringToDate(endDate) >= temp.getEndDate())
				|| (Date::stringToDate(startDate) < temp.getStartDate() && Date::stringToDate(endDate) >= temp.getStartDate()) 
				|| (Date::stringToDate(startDate) <= temp.getEndDate() && Date::stringToDate(endDate) > temp.getEndDate())
				|| (Date::stringToDate(startDate) >= temp.getStartDate() && Date::stringToDate(endDate) <= temp.getEndDate())
				|| (Date::stringToDate(startDate) == temp.getStartDate() && Date::stringToDate(endDate) == temp.getEndDate())
				) {
				return true;}
			else { 
				return false;} 
		else {
			return false;
		}
	});
	return res;
}

std::list<Meeting> AgendaService::listAllMeetings(const std::string userName) const {
	string name = userName;
	list<Meeting> meettemp = m_storage -> queryMeeting([name](const Meeting& temp) -> bool {
		if(temp.getSponsor() == name || temp.isParticipator(name)) {
			return true;}
		else {  return false;}
	});
	return meettemp; 
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string userName) const {
	list<Meeting> res = m_storage -> queryMeeting([userName](const Meeting& temp) -> bool {
		if(temp.getSponsor() == userName) 
			return true;
		else 
			return false;
	});
	return res; 
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(
        const std::string userName) const {
	list<Meeting> res = m_storage -> queryMeeting([userName](const Meeting& temp) -> bool {
		if(temp.isParticipator(userName) && temp.getSponsor() != userName) return true;
		else return false;
	});
	return res;
}

bool AgendaService::deleteMeeting(const std::string userName, const std::string title) {
	int totol = m_storage -> deleteMeeting( [userName, title](const Meeting &temp) -> bool {
											if(temp.getSponsor() == userName && temp.getTitle() == title) 
												return true;
											else return false;
										} );
    if(totol) {
    	m_storage -> sync();
    	return true;
    }
    else return false;
}

bool AgendaService::deleteAllMeetings(const std::string userName) {
	int totol = m_storage -> deleteMeeting([userName](const Meeting &temp) -> bool { 
		                                    if(temp.getSponsor() == userName) 
		                                    	return true;
		                                    else
		                                        return false;
		                                });
    if(totol) { 
    	m_storage -> sync();
    	return true;
    }
    else return false;
}

void AgendaService::startAgenda(void) {
	m_storage = Storage::getInstance();
}

void AgendaService::quitAgenda(void) {
	m_storage = NULL;
}

void AgendaService::changePassword(string name, string password) {
	m_storage -> updateUser([name](const User& temp) -> bool {
		                    if(temp.getName() == name) return true;
		                    else return false;
	                    } ,
	    [password](User &temp) -> void{
	    	temp.setPassword(password);
	    }
	);
	m_storage -> sync();
}

void AgendaService::changeEmail(string name, string email) {
	m_storage -> updateUser([name](const User& temp) -> bool {
		                    if(temp.getName() == name) return true;
		                    else return false;
	                    } ,
	    [email](User &temp) {
	    	temp.setEmail(email);
	    }
	);
	m_storage -> sync();
}

void AgendaService::changePhone(string name, string phone) {
	m_storage -> updateUser([name](const User& temp) -> bool {
		                    if(temp.getName() == name) return true;
		                    else return false;
	                    } ,
	    [phone](User &temp) {
	    	temp.setPhone(phone);
	    }
	);
	m_storage -> sync();
}

bool AgendaService::changeName(string userName, string name) {
	list<User> usertemp = m_storage -> queryUser([name](const User &temp) -> bool {
		if(name == temp.getName()) return true;
		else return false;
	});
	if(!usertemp.empty()) return false;
	m_storage -> updateUser([userName](const User& temp) -> bool {
		                    if(temp.getName() == userName) return true;
		                    else return false;
	                    } ,
	                    [name](User &temp) {
	                    	temp.setName(name);
	                    }
	);
	return true;
}
