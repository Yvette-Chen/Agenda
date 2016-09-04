#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <sstream>
#include "AgendaUI.hpp"

using namespace std;
static bool flag = false;

AgendaUI::AgendaUI() {}

void AgendaUI::OperationLoop(void) {
	startAgenda();
}

void AgendaUI::startAgenda(void) {
	while(true) {
		if(!flag) {
			cout << "-----------------------Agenda-----------------------" << endl;
			cout << "Action :                                             " << endl;
			cout << "l:  - log in by Agenda by user name and password    " << endl;
			cout << "r:  - register an Agenda account                    " << endl;
			cout << "q:  - quit Agenda                                   " << endl;
			cout << "----------------------------------------------------" << endl;
			cout << endl;
			cout << "Agenda : ~$ ";
			string ans = getOperation();
			cout << endl;
			if(ans == "q") break;
			executeOperation(ans);
		}
		if(flag) {
			cout << "-----------------------Agenda-----------------------" << endl;
			cout << "Action :                                            " << endl;
			cout << "o   - log out Agenda                                " << endl;
			cout << "dc  - delete Agenda account                         " << endl;
			cout << "lu  - list all Agenda user                          " << endl;
			cout << "cm  - create a meeting                              " << endl;
			cout << "la  - list all meetings                             " << endl;
			cout << "las - list all sponsor meetings                     " << endl;
			cout << "lap - list all participate meetings                 " << endl;
			cout << "qm  - query meeting by title                        " << endl;
			cout << "qt  - query meeting by time interval                " << endl;
			cout << "dm  - delete meeting by title                       " << endl;
			cout << "da  - delete all meetings                           " << endl;
			//cout << "cn  - change the user name                          " << endl;
			cout << "cp  - change the password                           " << endl;
			cout << "ce  - change email                                  " << endl;
			cout << "cph - change the phone                              " << endl;
			cout << "Agenda@" << m_userName << " : # ";
			string ans = getOperation();
			cout << endl;
			if(ans == "o") flag = false;
			 else executeOperation(ans);
		}
	}
}

std::string AgendaUI::getOperation() {
	string temp;
	cin >> temp;
	return temp;
}

bool AgendaUI::executeOperation(std::string t_operation) {
	if(t_operation == "o") 
		userLogOut();
	if(t_operation == "dc") 
		deleteUser();
	if(t_operation == "lu") 
		listAllUsers();
	if(t_operation == "cm") 
		createMeeting();
	if(t_operation == "la") 
		listAllMeetings();
	if(t_operation == "las") 
		listAllSponsorMeetings();
	if(t_operation == "lap") 
		listAllParticipateMeetings();
	if(t_operation == "qm") 
		queryMeetingByTitle();
	if(t_operation == "qt") 
		queryMeetingByTimeInterval();
	if(t_operation == "dm") 
		deleteMeetingByTitle();
	if(t_operation == "da") 
		deleteAllMeetings();
    if(t_operation == "l") 
    	userLogIn();
    if(t_operation == "r") 
    	userRegister();
    if(t_operation == "q") 
    	quitAgenda();
    //if(t_operation == "cn")
    //	changeName();
    if(t_operation == "cp")
    	changePassword();
    if(t_operation == "ce")
    	changeEmail();
    if(t_operation == "cph")
    	changePhone();
    //cout << "exe" << endl;
}

void AgendaUI::userLogIn(void) {
	cout << "[log in] [user name] [password]" << endl;
	cout << "[log in] ";
	cin >> m_userName >> m_userPassword;
	if(m_agendaService.userLogIn(m_userName, m_userPassword)) {
		cout << "[log in] succeed!" << endl;
		flag = true;
	} else {
		cout << "[error] wrong user name or password" << endl;
		cout << "[error] log in fail!" << endl;
	}
	cout << endl;
}

void AgendaUI::userRegister(void) {
	cout << "[register] [user name] [password] [email] [phone]" << endl;
	cout << "[register] ";
	string email, phone;
	cin >> m_userName >> m_userPassword >> email >> phone;
	if(m_agendaService.userRegister(m_userName, m_userPassword, email, phone)) {
		cout << "[register] succeed!" << endl;
		//flag = true;
	} else {
		cout << "the user name you register have been registered by others" << endl;
		cout << "[error] register fail!" << endl;
	}
	cout << endl;
}

void AgendaUI::quitAgenda(void) {}

void AgendaUI::userLogOut(void) {}

void AgendaUI::deleteUser(void) {
	if(m_agendaService.deleteUser(m_userName, m_userPassword)) {
		cout << "[delete Agenda account] succeed!" << endl;
		flag = false;
	} else {
		cout << "[delete Agenda account] fail!" << endl;
	}
	cout << endl;
}

void AgendaUI::listAllUsers(void) {
	cout << "[list all users]" << endl;
	cout << "name\temail\t\tphone" << endl;
	list<User> res = m_agendaService.listAllUsers();
	for (auto p = res.begin(); p != res.end(); p++) {
		cout << p -> getName() << "\t" << p -> getEmail() << "\t" << p -> getPhone() << endl;
	}
	cout << endl;
}

void AgendaUI::createMeeting(void) {
	vector<string> par;
	string totol1;
	string title, start, end;
	cout << "[create meeting] [the number of participators]" << endl;
	cout << "[create meeting] ";
	cin >> totol1;
	for (int i = 0; i < totol1.size(); i++) {
		if(totol1[1] < '0' || totol1[i] > '9') {
			cout << "[create meeting] error, your input is not a interger" << endl << endl;
			return;
		}
	}
	int totol;
	stringstream ss;
	ss << totol1;
	ss >> totol;
	for(int i = 0; i < totol; i++) {
		string temp;
		cout << "[create meeting] [please enter the participator " << i + 1 << " ]" << endl;
		cout << "[create meeting] ";
		cin >> temp;
		par.push_back(temp);
	}
	cout << "[create meeting] [title][start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
	cin >> title >> start >> end;
	int error = m_agendaService.createMeeting(m_userName, title, start,end, par);
	if(error == 10) {
		cout << "[create meeting] succeed!" << endl;
	} else {
		cout << "[create meeting] error!" << endl;
		cout << "[create meeting] ";
		switch(error) {
			case 1: cout << "participator is 0" << endl; break;
			case 2: cout << "the sponsor is in the participator" << endl; break;
			case 3: cout << "the participator repeat" << endl; break;
			case 4: cout << "the date is not valid or reasonable" << endl; break;
			case 5: cout << "the title repeat" << endl; break;
			case 6: cout << "the sponsor is not a user of Agenda" << endl; break;
			case 7: cout << "one of the participator is not a user of Agenda" << endl; break;
			case 8: cout << "the time of the sponsor overlap" << endl; break;
			case 9: cout << "the time of one of the participator overlap" << endl; break;
		}
	}
	cout << endl;
}

void AgendaUI::listAllMeetings(void) {
	list<Meeting> res =  m_agendaService.listAllMeetings(m_userName);
	//cout << res.size() << endl;
	cout << "[list all meetings]" << endl;
	if(res.size() == 0) {
		cout << "the user did not take part in any meetings" << endl << endl;
		return;
	}
	cout <<"title\tsponsor\tstart time\t\tend time\t\tparticipators" << endl;
	if(res.size() == 0) return;
	for(auto p = res.begin(); p != res.end(); p++) {
		cout << p -> getTitle() << "\t";
		cout << p -> getSponsor() << "\t" ;
		cout << Date::dateToString(p -> getStartDate()) << "\t";
		cout << Date::dateToString(p -> getEndDate()) << "\t";
		vector<string> par = p -> getParticipator();
		cout << par[0];
		if(par.size() > 1) {
			for(auto q = par.begin() + 1; q != par.end(); q++) {
			    cout << "," << *q;
		    }
		}
		cout << endl;
	}
	cout << endl;
}

void AgendaUI::listAllSponsorMeetings(void) {
	list<Meeting> res = m_agendaService.listAllSponsorMeetings(m_userName);
	cout << "[list all sponsor meetings]" << endl;
	if(res.size() == 0) {
		cout << "the user did not sponsor any meetings" << endl << endl;
		return;
	}
	cout <<"title\tsponsor\tstart time\t\tend time\t\tparticipators" << endl;
	if(res.size() == 0) return;
	for(auto p = res.begin(); p != res.end(); p++) {
		cout << p -> getTitle() << "\t" << p -> getSponsor() << "\t" << Date::dateToString(p -> getStartDate()) << "\t"
		     << Date::dateToString(p -> getEndDate()) << "\t";
		vector<string> par = p -> getParticipator();
		cout << par[0];
		for(auto q = par.begin() + 1; q != par.end(); q++) {
			cout << "," << *q;
		}
		cout << endl;
	}
	cout << endl;
}

void AgendaUI::listAllParticipateMeetings(void) {
	list<Meeting> res = m_agendaService.listAllParticipateMeetings(m_userName);
	cout << "[list all participator meetings]" << endl;
	if(res.size() == 0) {
		cout << "the user is not a participator of any meetings" << endl << endl;
	}
	cout <<"title\tsponsor\tstart time\t\tend time\t\tparticipators" << endl;
	for(auto p = res.begin(); p != res.end(); p++) {
		cout << p -> getTitle() << "\t" << p -> getSponsor() << "\t" << Date::dateToString(p -> getStartDate()) << "\t"
		     << Date::dateToString(p -> getEndDate()) << "\t";
		vector<string> par = p -> getParticipator();
		cout << par[0];
		for(auto q = par.begin() + 1; q != par.end(); q++) {
			cout << "," << *q;
		}
		cout << endl;
	}
	cout << endl;
}

void AgendaUI::queryMeetingByTitle(void) {
	cout << "[query meeting] [title]" << endl;
	cout << "[query meeting] ";
	string title;
	cin >> title;
	list<Meeting> res = m_agendaService.meetingQuery(m_userName, title);
	if(res.size() == 0) {
		cout << "the wrong title or other reasons" << endl << endl;
		return;
	}
	cout <<"title\tsponsor\tstart time\t\tend time\t\tparticipators" << endl;
	auto p = res.begin();
	cout << p -> getSponsor() << "\t" << title << "\t" << Date::dateToString(p -> getStartDate()) << "\t"
	     << Date::dateToString(p -> getEndDate()) << "\t";
	vector<string> par = p -> getParticipator();
	cout << par[0];
	for(auto p = par.begin() + 1; p != par.end(); p++) {
		cout << "," << *p;
	}
	cout << endl;
}

void AgendaUI::queryMeetingByTimeInterval(void) {
	cout << "[query meetings] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
	cout << "[query meetings] ";
	string start, end;
	cin >> start >> end;
	list<Meeting> res = m_agendaService.meetingQuery(m_userName, start, end);
	if(start >= end) {
		cout << "the date is not reasonable" << endl << endl;
		return;
	}
    if(!Date::isValid(Date::stringToDate(start)) || !Date::isValid(Date::stringToDate(end))) {
    	cout << "the date is not valid" << endl << endl;
    	return;
    }  
	cout << "[query meetings] " << endl << endl;
	cout <<"title\tsponsor\tstart time\t\tend time\t\tparticipators" << endl;
	for(auto p = res.begin(); p!= res.end(); p++) {
		cout << p -> getTitle() << "\t" << p -> getSponsor() << "\t" << Date::dateToString(p -> getStartDate()) << "\t"
		     << Date::dateToString(p -> getEndDate()) << "\t";
		vector<string> par = p -> getParticipator();
		cout << par[0];
		for(auto q = par.begin() + 1; q != par.end(); q++) {
			cout << "," << *q;
		}
		cout << endl;
	}
	cout << endl;
}

void AgendaUI::deleteMeetingByTitle(void) {
	cout << "[delete meeting] [title]" << endl;
	cout << "[delete meeting] ";
	string title;
	cin >> title;
	if(m_agendaService.deleteMeeting(m_userName, title)) {
		cout << "[delete meeting by title] succeed!" << endl;
	} else {
		cout << "[error] the wrong title or other reasons" << endl;
		cout << "[error] delete meeting fail!" << endl;
	}
	cout << endl;
}

void AgendaUI::deleteAllMeetings(void) {
	if(m_agendaService.deleteAllMeetings(m_userName)) {
		cout << "[delete all meetings] succeed!" << endl;
	}
}

void AgendaUI::printMeetings(std::list<Meeting> t_meetings) {}

void AgendaUI::changePassword(void) {
	cout << "[change password] [password]" << endl;
	cout << "[change password] ";
	string password;
	cin >> password;
	m_userPassword = password;
	m_agendaService.changePassword(m_userName, password);
	cout << "password change succeed!" << endl << endl;
} 

void AgendaUI::changeEmail(void) {
	cout << "[change email] [email]" << endl;
	cout << "[change email] ";
	string email;
	cin >> email;
	m_agendaService.changeEmail(m_userName, email);
	cout << "email change succeed!" << endl << endl;
} 

void AgendaUI::changePhone(void) {
	cout << "[change phone] [phone]" << endl;
	cout << "[change phone] ";
	string phone;
	cin >> phone;
	m_agendaService.changePhone(m_userName, phone);
	cout << "phone change succeed!" << endl << endl;
} 

void AgendaUI::changeName(void) {
	cout << "[change name] [name]" << endl;
	cout << "[change name] ";
	string name;
	cin >> name;
	if (m_agendaService.changeName(m_userName, name)) {
		m_userName = name;
		cout << "change name succeed!" << endl;
	} else {
		cout << "the name you chose have been registered by others" << endl;
		cout << "[error] name change fail!" << endl;
	}

}
