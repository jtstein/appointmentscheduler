// Jordan Stein
// CSCI 4761
// Mar 28 2017
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include "user.h"
#include "functions.h"

// Adds a unique user to the users vector
void addUser(User user, std::vector<User>& users) {

	bool unique = true; // ensures all users are unique.
	for (int i = 0; i < users.size(); i++) {
		if (user == users[i]) {
			unique = false;
		}
	}
	if(unique)
		users.push_back(user);
}

// Removes a user from the users vector
void deleteUser(User user, std::vector<User>& users) {

	// find index of desired user
	int userIndex = -1;
	for (int i = 0; i < users.size(); i++) {
		if (user == users[i]) {
			userIndex = i;
		}
	}
	if (userIndex != -1) {
		users.erase(users.begin() + userIndex);
	}
}

// Prints all users to the console
void printUsers(std::vector<User> users) {
	for (int i = 0; i < users.size(); i++) {
		std::cout << "==== User " << i + 1 << " ====" << std::endl;
		std::cout << "Name:	" << users[i].getName() << std::endl;
		std::cout << "Phone:	" << users[i].getPhone() << std::endl;
		std::cout << "Email:	" << users[i].getEmail() << std::endl;
		std::cout << std::endl;
	}
}

// Reads all user information from csv file into users vector
// Returns false if file fails to read
bool readUsers(std::vector<User>& users) {
	// ====== Read all user information (Name, Password, Phone, Email) =====
	std::vector <std::vector <std::string> > data;
	std::ifstream in("userData.csv");

	if (!in) {
		std::cerr << "Failed to read file \"userData.csv\"" << std::endl;
		std::cerr << "Enter any key to exit the program.\n";
		char err;
		std::cin >> err;

		return false;
	}

	int i = 0; // index

	while (in) {
		std::string userData;
		if (!getline(in, userData)) 
			break;

		std::istringstream ss(userData);
		std::vector <std::string> record;

		while (ss) {
			std::string userData;
			if (!getline(ss, userData, ',')) 
				break;
			record.push_back(userData);
		}	

		data.push_back(record);
		User newUser(data[i][0], data[i][1], data[i][2], data[i][3], data[i][4]);
		addUser(newUser, users);
		i++;
	}

	if (!in.eof()) {
		std::cerr << "Failed to read data from \"userData.csv\"";
		std::cerr << "\nEnter any key to exit the program.\n";
		char err;
		std::cin >> err;
		return false;
	}

	in.close();


	// ===== Read all user appointment information =====
	std::vector <std::vector <std::string> > data2;
	std::ifstream in2("userAppointments.csv");

	if (!in2) {
		std::cerr << "Failed to read file \"userAppointments.csv\"" << std::endl;
		std::cerr << "Enter any key to exit the program.\n";
		char err;
		std::cin >> err;
		return false;
	}

	i = 0; // index

	while (in2) {
		std::string userData2;
		if (!getline(in2, userData2))
			break;

		std::istringstream ss2(userData2);
		std::vector <std::string> record2;

		while (ss2) {
			std::string userData2;
			if (!getline(ss2, userData2, ','))
				break;
			record2.push_back(userData2);
		}

		data2.push_back(record2);

		for (int j = 0; j < data2[i].size(); j++) {
			if (data2[i][j] != " "){
				users[i].addAppointment(data2[i][j]);
			}
		}


		i++;
	}

	if (!in2.eof()) {
		std::cerr << "Failed to read data from \"userAppointments.csv\"";
		std::cerr << "\nEnter any key to exit the program.\n";
		char err;
		std::cin >> err;
		return false;
	}

	in2.close();
	
	// ===== Read all user descriptions for appointments =====
	std::vector <std::vector <std::string> > data3;
	std::ifstream in3("userDescriptions.csv");

	if (!in3) {
		std::cerr << "Failed to read file \"userDescriptions.csv\"" << std::endl;
		std::cerr << "Enter any key to exit the program.\n";
		char err;
		std::cin >> err;
		return false;
	}

	i = 0; // index

	while (in3) {
		std::string userDesc;
		if (!getline(in3, userDesc))
			break;

		std::istringstream ss2(userDesc);
		std::vector <std::string> record3;

		while (ss2) {
			std::string userData3;
			if (!getline(ss2, userData3, ','))
				break;
			record3.push_back(userData3);
		}

		data3.push_back(record3);
		
		for (int j = 0; j < data3[i].size(); j++) {
			if (data3[i][j] != " "){
				users[i].addDescription(data3[i][j]);
			}
		}

		i++;
	}

	if (!in3.eof()) {
		std::cerr << "Failed to read data from \"userDescriptions.csv\"";
		std::cerr << "\nEnter any key to exit the program.\n";
		char err;
		std::cin >> err;
		return false;
	}

	in3.close();

	// sort all appointments
	for (int i=0; i < users.size(); i++){
		users[i].sortAppointments();
	}

	return true;

}


// Writes all user information from vector into text file
void writeUsers(std::vector<User> users) {

	// write user data to file
	std::ofstream out;
	out.open("userData.csv");
	for (int i = 0; i < users.size(); i++) {
		out << users[i].getUsername() << ',' << users[i].getPassword() << ',' << users[i].getName() << ',' << users[i].getPhone() << ',' << users[i].getEmail() << std::endl;
	}

	out.close();
	
	// write user appointments to file
	std::ofstream out2;
	out2.open("userAppointments.csv");
	for (int i = 0; i < users.size(); i++) {
		std::vector<std::string> appointments = users[i].getAppointments();
		for (int j = 0; j < appointments.size(); j++) {
			out2 << appointments[j] << ',';
		}
		if (appointments.empty())
			out2 << " "; // push a blank space to output so all users appointments are inline with every user
		if (i < users.size()-1)
			out2 << std::endl;	
	}

	out2.close();

	// write appointment descriptions to file
	std::ofstream out3;
	out3.open("userDescriptions.csv");
	for (int i = 0; i < users.size(); i++) {
		std::vector<std::string> descriptions = users[i].getDescriptions();
		for (int j = 0; j < descriptions.size(); j++) {
			out3 << descriptions[j] << ',';
		}
		if (descriptions.empty())
			out3 << " "; // push a blank space to output so all users descriptions are inline with every user
		if (i < users.size()-1)
			out3 << std::endl;
	}

	out3.close();

}

// reads users who are logged in to file
void readLogIn(std::vector<std::string>& currentUsers){
	std::vector <std::vector <std::string> > data;
	std::ifstream in("loggedIn.csv");

	if (!in) {
		std::cerr << "Failed to read file \"loggedIn.csv\"" << std::endl;
		std::cerr << "Enter any key to exit the program.\n";
		char err;
		std::cin >> err;

	}

	int i = 0; // index

	while (in) {
		std::string userData;
		if (!getline(in, userData)) 
			break;

		std::istringstream ss(userData);
		std::vector <std::string> record;

		while (ss) {
			std::string userData;
			if (!getline(ss, userData, ',')) 
				break;
			record.push_back(userData);
		}	

		data.push_back(record);
		currentUsers.push_back(record[i]);
		i++;
	}

	if (!in.eof()) {
		std::cerr << "Failed to read data from \"userData.csv\"";
		std::cerr << "\nEnter any key to exit the program.\n";
		char err;
		std::cin >> err;
	}

	in.close();

}

// writes user who is logging out to file
void writeLogIn(std::string username, std::vector<std::string>& currentUsers, bool login){
	// remove user from current users vector
	std::ofstream out;
	out.open("loggedIn.csv");
	for (int i = 0; i < currentUsers.size(); i++) {
		if (login){
			out << currentUsers[i] << ',';
		}
		else {
			if (currentUsers[i] != username){
				out << currentUsers[i] << ',';
			}
		}
	}

	out.close();
}

// Creates an appointment for a user, along with a description for the appointment
void createAppointment(User& user, std::vector<User>& users, int new_fd, char* recvbuf, long numbytes) {

	std::string send_buf;
	std::string prompt = ""; // will be used to prompt the user for input info
	std::string month = "  ";
	bool valid = true;
	while (valid) { // prompt user to enter the month of the appointment. Error check for correct formatting.

		while (true){
			prompt = "\nEnter the month of the appointment: Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec\n";
			// prompt the user for month entry
			send(new_fd, prompt.c_str(), 127, 0);

			// recieve month entry from client
			numbytes = recv(new_fd, recvbuf, 127, 0);
        	recvbuf[numbytes] = '\0';
        	month = recvbuf;
        	std::cout << "[Client]: month: " << month << std::endl;

			//std::cin >> month;
			std::transform(month.begin(), month.end(), month.begin(), ::tolower); // ensure month string lowercase
			month[0] = toupper(month[0]); // make first character uppercase
			if (month == "Jan" || month == "Feb" || month == "Mar" || month == "Apr" || month == "May" || month == "Jun" || month == "Jul" || month == "Aug" || month == "Sep" || month == "Oct" || month == "Nov" || month == "Dec") {
				valid = false;
				std::cout << "Valid month entered.\n";
				send_buf = "Success";
				send(new_fd, send_buf.c_str(), 127, 0);
				break;
			}
			else {
				send_buf = "Failure";
				send(new_fd, send_buf.c_str(), 127, 0);
				std::cerr << "\nInvalid month entered.\n";
				month = "";
			}
		}
	}
	valid = true;
	std::string day = "";
	send_buf = "";
	while (valid) { // prompt user to enter the day of the appointment. Error check for correct formatting.
		while (true){

			prompt = "\nEnter the day of the appointment: 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31\n";
			// prompt the user for day entry
			send(new_fd, prompt.c_str(), 127, 0);

			// recieve day entry from client
			numbytes = recv(new_fd, recvbuf, 127, 0);
			recvbuf[numbytes] = '\0';
			day = recvbuf;
			std::cout << "[Client]: day: " << day << std::endl;

			//std::cin >> day;
			int intDay = 0;

			try{
				intDay = std::stoi(day);
			}
			catch(int n){
				throw;
			}catch (...){

			}

			if (intDay >= 1 && intDay <= 31) {
				valid = false;
				send_buf = "Success";
				send(new_fd, send_buf.c_str(), 127, 0);
				break;
			}
			else {
				send_buf = "Failure";
				send(new_fd, send_buf.c_str(), 127, 0);
				std::cerr << "\nInvalid day entered.\n";
				day = "";
			}
		}
	}
	valid = true;
	std::string year = "";
	send_buf = "";
	while (valid) {// prompt user to enter the year of the appointment. Error check for correct formatting.
		while(true){
			prompt = "\nEnter the year of the appointment: 2017 2018 2019 2020 2021 2022 2023 2024 2025 2026 2027 ...\n";
			//std::cin >> year;
			//prompt the user for year entry
			send(new_fd, prompt.c_str(), 127, 0);

			// recieve year entry from client
			numbytes = recv(new_fd, recvbuf, 127, 0);
			recvbuf[numbytes] = '\0';
			year = recvbuf;
			std::cout << "[Client]: year: " << year << std::endl;

			int intYear = 0;

			try{
				intYear = std::stoi(year);
			} 
			catch(int n){
				throw;
			} catch(...){

			}

			if (intYear >= 2017) {
				valid = false;
				send_buf = "Success";
				send(new_fd, send_buf.c_str(), 127, 0);
				break;
			}
			else {
				send_buf = "Failure";
				send(new_fd, send_buf.c_str(), 127, 0);
				std::cerr << "\nInvalid year submitted\n";
				year = "";
			}
		}
	}
	valid = true;
	std::string time = "";
	send_buf = "";
	bool oneDigit = false; // if hour is less than 10
	while(valid){
		while (true) { // prompt user to enter the time of the appointment. Error check for correct formatting.
			prompt =  "\nEnter the time of the appointment: 12:00am - 12:00pm\n";
			// prompt the user for time entry
			send(new_fd, prompt.c_str(), 127, 0);

			// recieve day entry from client
			numbytes = recv(new_fd, recvbuf, 127, 0);
			recvbuf[numbytes] = '\0';
			time = recvbuf;
			std::cout << "[Client]: time: " << time << std::endl;

			int hour = 0;
			int min = 0;

			if (time.size() == 6) { // may happen if user enters 1:00pm etc...
				std::string tempTime = "       ";
				tempTime[1] = time[0];
				tempTime[2] = time[1];
				tempTime[3] = time[2];
				tempTime[4] = time[3];
				tempTime[5] = time[4];
				tempTime[6] = time[5];
				time = tempTime;
				oneDigit = true;
			}

			if (time.size() == 7) {

				try{
					hour = std::stoi(time.substr(0,2)); // check first two charactrs for the hour submitted
					min = std::stoi(time.substr(3, 5)); // check 3rd and 4th characters for the minute submitted

				}
				catch(int n){
					throw;
				}catch(...){

				}

				time[5] = tolower(time[5]); // ensure am / pm is lowercase
				time[6] = tolower(time[6]);
		
				if ((hour >= 0 && hour <= 12) && (min >= 0 && min <= 60)) {
					if ((time[5] == 'a' || time[5] == 'p') && time[6] == 'm') {
						valid = false;
						send_buf = "Success";
						send(new_fd, send_buf.c_str(), 127, 0);
						break;
					}
					else {
						send_buf = "FailureMeridiem";
						send(new_fd, send_buf.c_str(), 127, 0);
						std::cerr << "\nInvalid meridiem submitted\n";
						time = "";
					}
				}
			}
			else {
				send_buf = "FailureHourMin";
				send(new_fd, send_buf.c_str(), 127, 0);
				std::cerr << "\nInvalid hour/min submitted\n";
				time = "";
			}
		}
	}

	if (day.size() == 1){
		std::string tempDay = day;
		day = "0" + tempDay;
	}

	if (time.size() == 6){
		std::string tempTime = time;
		time = "0" + tempTime;
	}


	prompt = "";
	std::string date = "";
	std::string description = "";
	if(oneDigit){ 
		date = month + " " + day + " " + year + time;
		prompt = "Please enter a description for your appointment on " + date + " at" + time + ":\n";
	}
	else{
		date = month + " " + day + " " + year + time;
		prompt = "Please enter a description for your appointment on " + date + " at " + time + ":\n";
	}

	// prompt user for description for appointment
	send(new_fd, prompt.c_str(), 127, 0);
	// recieve description entry from client
	numbytes = recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	description = recvbuf;
	std::cout << "[Client]: description: " << description << std::endl;


	// get position of user in users vector
	int userIndex = -1;
	for(int i=0; i < users.size(); i++){
		if(user == users[i]){
			userIndex = i;
			break;
		}
	}

	// add appointment and description info
	user.addAppointment(date);
	user.addDescription(description);
	user.sortAppointments();

	if(userIndex != -1){ // place updated user back in vector
		users[userIndex] = user;
	}

	if(oneDigit)
		prompt = "\nThe appointment has been added to your schedule for " + date + " at" + time + ":\n";
	else
		prompt = "\nThe appointment has been added to your schedule for " + date + " at " + time + ":\n";

	// tell user the appointment has been created
	send(new_fd, prompt.c_str(), 127, 0);

	writeUsers(users); // rewrite all user data for new appointment.

}

// Removes an appointment for a user, along with a description for the appointment
void removeAppointment(User& user, std::vector<User>& users, int new_fd, char* recvbuf, long numbytes) {
	
	int userIndex = -1;
	for(int i=0; i< users.size(); i++){
		if (user == users[i]){
			userIndex = i;
			break;
		}
	}

	std::string prompt = ""; // used to prompt client with info.
	prompt = users[userIndex].returnAppointments(); // fill prompt with all appointments for given user
	// sends the user a list of all appoints so they can choose which one to delete.
	send(new_fd, prompt.c_str(), 1023, 0);

	std::string appt = ""; // appointment to remove
	int intAppt = 0;
	// recieve appointment to remove entry from client
	numbytes = recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	appt = recvbuf;
	std::cout << "[Client]: Appointment to remove: " << appt << std::endl;

	std::string apptLower = appt;
	std::transform(apptLower.begin(), apptLower.end(), apptLower.begin(), ::tolower);

	if (apptLower != "exit"){
		try{
			intAppt = std::stoi(appt) - 1; // off by one
		}catch(int n){
			throw n;
		}catch (...){

		}

		bool removed = false;
		if (userIndex != -1){
			try{
				if (appt != " "){
					std::vector<std::string> appts = users[userIndex].getAppointments();
					if(intAppt >=0 && intAppt <= appts.size()){
						users[userIndex].deleteAppointment(intAppt); // deletes the appointment at index
						user = users[userIndex];
						removed = true;
					}
				}
			}catch (int n){
				throw n;
			}
			catch(...){

			}
		}

		prompt = "Appointment " + appt + " has been removed successfully.\n";
		// prompt client that the appointment was removed
		send(new_fd, prompt.c_str(), 127, 0);

		std::cout << prompt;
		if (removed){
			writeUsers(users); // rewrite all user data for removed appointment.
		}
	}
}

// allows a user to update appointment information
void updateAppointment(User& user, std::vector<User>& users, int new_fd, char* recvbuf, long numbytes){
	int userIndex = -1;
	for(int i=0; i< users.size(); i++){
		if (user == users[i]){
			userIndex = i;
			break;
		}
	}

	std::string prompt = ""; // used to prompt client with info.
	prompt = users[userIndex].returnAppointments(); // fill prompt with all appointments for given user
	// sends the user a list of all appoints so they can choose which one to update.
	send(new_fd, prompt.c_str(), 1023, 0);

	std::string appt = ""; // appointment to update
	int intAppt = 0;
	// recieve appointment to update entry from client
	numbytes = recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	appt = recvbuf;
	std::cout << "[Client]: Appointment to update: " << appt << std::endl;

	std::string apptLower = appt;
	std::transform(apptLower.begin(), apptLower.end(), apptLower.begin(), ::tolower);

	if (apptLower != "exit"){
		try{
			intAppt = std::stoi(appt) - 1; // off by one
		}catch(int n){
			throw n;
		}catch (...){

		}

		bool updated = false;
		if (userIndex != -1){
			try{
				if (appt != " "){
					std::vector<std::string> appts = users[userIndex].getAppointments();
					if(intAppt >=0 && intAppt <= appts.size()){

						std::string apptEntry = "";
						// recieve appointment entry to update from client
						numbytes = recv(new_fd, recvbuf, 127, 0);
						recvbuf[numbytes] = '\0';
						apptEntry = recvbuf;
						std::cout << "[Client]: Appointment entry to update: " << apptEntry << std::endl;						


						if (apptEntry == "1"){ // update date/time
							std::string newDateTime = "";
							std::string newTime = "";
							std::string newDay = "";
							std::string newMonth = "";
							std::string newYear = "";

							// recieve new day
							numbytes = recv(new_fd, recvbuf, 127, 0);
							recvbuf[numbytes] = '\0';
							newDay = recvbuf;
							std::cout << "[Client]: New date/time to update: " << newDay << std::endl;

							// recieve new month
							numbytes = recv(new_fd, recvbuf, 127, 0);
							recvbuf[numbytes] = '\0';
							newMonth = recvbuf;
							std::cout << "[Client]: New month to update: " << newMonth << std::endl;

							// recieve new year
							numbytes = recv(new_fd, recvbuf, 127, 0);
							recvbuf[numbytes] = '\0';
							newYear = recvbuf;
							std::cout << "[Client]: New year to update: " << newYear << std::endl;
							
							// recieve new time
							numbytes = recv(new_fd, recvbuf, 127, 0);
							recvbuf[numbytes] = '\0';
							newTime = recvbuf;
							std::cout << "[Client]: New time to update: " << newTime << std::endl;

							if (newDay.size() == 1){
								std::string tempDay = newDay;
								newDay = "0" + tempDay;
							}

							if (newTime.size() == 6){
								std::string tempTime = newTime;
								newTime = "0" + tempTime;

							}
							if (newTime[0] == ' '){
								newDateTime = newDay + " " + newMonth + " " + newYear + newTime;
							}
							else{
								newDateTime = newDay + " " + newMonth + " " + newYear + " " + newTime;
							}

							users[userIndex].updateAppointment(intAppt, newDateTime, true);
							user = users[userIndex];
							updated = true;
	
						}
						else if (apptEntry == "2"){ // update description
							std::string newDesc = "";
							// recieve new description
							numbytes = recv(new_fd, recvbuf, 127, 0);
							recvbuf[numbytes] = '\0';
							newDesc = recvbuf;

							std::cout << "[Client]: New description to update: " << newDesc << std::endl;

							users[userIndex].updateAppointment(intAppt, newDesc, false);
							user = users[userIndex];
							updated = true;

						}else{

						}	
					}
				}
			}catch (int n){
				throw n;
			}
			catch(...){

			}
		}


		if (updated){
			writeUsers(users); // rewrite all user data for removed appointment.
		}

	}

}

// Checks all user appointments for time conflicts, sends them to client.
void checkTimeConflicts(User user, std::vector<User> users, int new_fd, char* recvbuf, long numbytes){
	int userIndex = -1;
	for(int i=0; i< users.size(); i++){
		if (user == users[i]){
			userIndex = i;
			break;
		}
	}

	std::string prompt = "";
	std::vector<std::string> appointments = users[userIndex].getAppointments();
	std::vector<std::string> descriptions = users[userIndex].getDescriptions();
	std::vector<std::string> conflicts;
	std::vector<std::string> conflictsDesc;

	for (int i=0; i < appointments.size(); i++){
		for (int j=i+1; j < appointments.size(); j++){
			if (appointments[i] == appointments[j]){ // if two appointments share a time
				bool newConflict = true;

				if (conflicts.size() == 0){
					conflicts.push_back(appointments[i]); // first conflict, push back
					conflictsDesc.push_back(descriptions[i]);
				}

				for (int k = 0; k < conflicts.size(); k++){ // check if its already in conflicts
					if (conflicts[k] == appointments[i]){
						newConflict = false;
						break;
					}
				}


				if (newConflict){
					conflicts.push_back(appointments[i]); // if its a new conflict, push [i] (first) back
					conflictsDesc.push_back(descriptions[i]);
				}
				else{
					conflicts.push_back(appointments[j]); // not new conflict, push [j] (the latter) back
					conflictsDesc.push_back(descriptions[j]);
				}
				

				break;
			}
		}
	}

	// now all conflicted times are stored in the conflicts vector
	prompt += "Below is a list of all appointments with conflicting times:\n";
	
	for(int i=0; i < conflicts.size(); i++){
		for (int j=1; j < appointments.size(); j++){
			if(conflicts[i] == appointments[j]){
				if (conflictsDesc[i] != descriptions[j]){
					prompt += conflicts[i] + '\n' + conflictsDesc[i] + '\n';
					break;
				}

			}
		}
	}

	if (conflicts.size() == 0){
		prompt = "There are no appointment time conflicts.\n\n";
	}

	// send all time conflicts to client
	send(new_fd, prompt.c_str(), 511, 0);
	std::cout << "[Server] Conflict information sent to client.\n";

	// recieve request to return to main menu
	std::string returnToMenu = "";
	numbytes = recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	returnToMenu = recvbuf;

}


void displayAppointments(User user, std::vector<User> users, int new_fd, char* recvbuf, long numbytes){
	int userIndex = -1;
	for(int i=0; i< users.size(); i++){
		if (user == users[i]){
			userIndex = i;
			break;
		}
	}

	std::string prompt = "";
	prompt = "Select an option:\n"
			 "1 ) Display all appointments\n"
			 "2 ) Display appointments for a specific time\n"
			 "3 ) Display appointments in a time range\n\n";

	// send menu to client
	send(new_fd, prompt.c_str(), 255, 0);

	// recieve menu choice from client
	std::string displayChoice = "";
	numbytes = recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	displayChoice = recvbuf;

	std::cout << "[Client]: display menu choice: " << displayChoice << std::endl;

	std::vector<std::string> appointments = users[userIndex].getAppointments();
	std::vector<std::string> descriptions = users[userIndex].getDescriptions();

	if (displayChoice == "1"){ // view all appointments
		prompt = "\n\nBelow are all of your appointments:\n\n";
		for (int i=0; i < appointments.size(); i++){
			prompt += appointments[i] + '\n' + descriptions[i] + '\n';
		}

		if (prompt == "\n\nBelow are all of your appointments:\n\n"){
			prompt += "\n\nNo appointments found\n";
		}			

	}
	else if (displayChoice == "2"){ // view appointments for specific time
		std::string dateTime = receiveDateTime(new_fd, recvbuf, numbytes);
		std::cout << "[Client]: Requests to view all appointments for " << dateTime << std::endl;

		prompt = "\n\nBelow are you appointment(s) on " + dateTime + ":\n\n";
		
		bool isData = false;
		for (int i=0; i < appointments.size(); i++){
			if (dateTime == appointments[i]){
				isData = true;
				prompt += appointments[i] + '\n' + descriptions[i] + '\n';
			}
		}
		if(!isData){
			prompt += "\n\nNo appointments found on " + dateTime + '\n';
		}
	}
	else if (displayChoice == "3"){ // view all appointments in time range
		std::string startDateTime = receiveDateTime(new_fd, recvbuf, numbytes);
		std::string endDateTime = receiveDateTime(new_fd, recvbuf, numbytes);

		std::cout << "[Client]: Start Date: " << startDateTime << " End Date: " << endDateTime << std::endl; 

		long startDayVal = dateToValue(startDateTime);
		long endDayVal = dateToValue(endDateTime);


		prompt = "\n\nBelow are you appointment(s) between " + startDateTime + " and " + endDateTime + ":\n\n";
		bool isData = false;
		// prompt all appointments whose dateValue is between startDayVal and endDayVal
		for (int i=0; i < appointments.size(); i++){
			long apptVal = dateToValue(appointments[i]);
			if (apptVal >= startDayVal && apptVal <= endDayVal){
				prompt += appointments[i] + '\n' + descriptions[i] + '\n';
				isData = true;
			}
		}

		if(startDayVal > endDayVal) {
			prompt = "\nError, start date entered is later than end date\n";
		}else if (!isData){
			prompt += "\n\nNo appointments found between " + startDateTime + " and " + endDateTime + '\n';
		}
	}

	// send appointments to user
	send(new_fd, prompt.c_str(), 1023, 0);

	// recieve request to return to main menu
	std::string returnToMenu = "";
	numbytes = recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	returnToMenu = recvbuf;
}

// converts a date/time string to a value
long dateToValue(std::string date){
	std::string month = date.substr(0,3);
	std::string day = "";
	std::string year = "";
	std::string hour = "";
	std::string min = "";
	int minInc = 0;
	int merInc = 0;
	char meridiem = ' ';
	if(date[5] == ' '){ // single digit day
		std::string tempday = "";
		tempday = date.substr(4,1);
		day = "0" + tempday;
		year = date.substr(6,4);
		hour = date.substr(11,2);
		minInc = 14;
		min = date.substr(minInc,2);
		merInc = 16;
		meridiem = date[merInc];
	}else{
		day = date.substr(4,2); // double digit day
		year = date.substr(7,4);
		hour = date.substr(12,2);
		minInc = 15;
		min = date.substr(minInc,2);
		merInc = 17;
		meridiem = date[merInc];
	}

	if (hour[0] == ' ' || hour.size() == 1){ // single digit hour
		hour[0] = '0';
	}
	if (hour[1] == ':'){
		char temphour = hour[0];
		hour = "0"; 
		hour += temphour;
	}

	if (min[1] == 'a' || min[1] == 'p'){
		min = date.substr(minInc-1, 2);
		meridiem = date[merInc-1];
	}

	// convert month to a number
	if (month == "Jan")
		month = "01";
	if (month == "Feb")
		month = "02";
	if (month == "Mar")
		month = "03";
	if (month == "Apr")
		month = "04";
	if (month == "May")
		month = "05";
	if (month == "Jun")
		month = "06";
	if (month == "Jul")
		month = "07";
	if (month == "Aug")
		month = "08";
	if (month == "Sep")
		month = "09";
	if (month == "Oct")
		month = "10";
	if (month == "Nov")
		month = "11";
	if (month == "Dec")
		month = "12";

	// add 12 to hour if meridiem is pm
	if (meridiem == 'p') {
		int intHour = 0;
		try{
			intHour = std::stoi(hour);
		}catch(int n){
			throw n;
		}catch(...){

		}
		intHour += 12;
		// convert back to string
		hour = std::to_string(intHour);
	}

	// string value representation of date / time
	std::string strVal = year + month + day + hour + min;

	long value = 0;
	// convert strval to integer
	try{
		value = std::stol(strVal);
	}catch(int n){
		throw n;
	}catch(...){

	}

	return value;
}

// Modifies user information
void modifyUser(User& user, std::vector<User>& users, int new_fd, char* recvbuf, long numbytes){
	int userIndex = -1;
	for(int i=0; i< users.size(); i++){
		if (user == users[i]){
			userIndex = i;
			break;
		}
	}

	std::string currentData = user.returnUserData();
	send(new_fd, currentData.c_str(), 127, 0);

	bool updated = false;
	std::string prompt = "";
	std::string updateData = "";
	prompt = "Which user information would you like to modify?\n1 ) name\n2 ) phone\n3 ) email\n4 ) password\n5 ) exit\n";

	send(new_fd, prompt.c_str(), 127, 0);

	std::string choice = "";
	// recieve choice from client
	numbytes = recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	choice = recvbuf;
	std::cout << "[Client]: update choice: " << choice << std::endl;


	if(choice == "1" || choice == "2" || choice == "3" || choice == "4"){
		// recieve update data from client
		numbytes = recv(new_fd, recvbuf, 127, 0);
		recvbuf[numbytes] = '\0';
		updateData = recvbuf;
	}

	std::string choiceStr = "";

	if(choice == "1"){
		users[userIndex].setName(updateData);
		choiceStr = "name";
		updated = true;
	}
	else if(choice == "2"){
		users[userIndex].setPhone(updateData);
		choiceStr = "phone";
		updated = true;
	}
	else if(choice == "3"){
		users[userIndex].setEmail(updateData);
		choiceStr = "email";
		updated = true;

	}
	else if(choice == "4"){
		users[userIndex].setPassword(updateData);
		choiceStr = "password";
		updated = true;
	}
	//else if(choice == "5"){
	//	return;
	//}

	if(choice == "1" || choice == "2" || choice == "3" || choice == "4"){
		prompt = "User " + choiceStr + " information has been updated.\n";
		send(new_fd, prompt.c_str(), 127, 0);
		std::cout << prompt;
	}

	user = users[userIndex];
	if (updated)
		writeUsers(users);

}

// Deletes the current user that is logged in. Returns true if user confirms deletion.
bool deleteUser(User& user, std::vector<User>& users, int new_fd, char* recvbuf, long numbytes){

	int userIndex = -1;
	for(int i=0; i< users.size(); i++){
		if (user == users[i]){
			userIndex = i;
			break;
		}
	}

	std::string prompt = "You have selected to permanently DELETE your account.\nPlease confirm your password to approve deletion.\n";
	send(new_fd, prompt.c_str(), 127, 0);

	std::string pw = "";
	// recieve password from client
	numbytes = recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	pw = recvbuf;

	if (pw == users[userIndex].getPassword()){
		// DANGER ZONE ===============================
		users[userIndex].deleteAllAppointments(); // deletes all appointments/descriptions for user
		users.erase(users.begin() + userIndex); // removes user from user vector
		// DANGER ZONE ===============================

		writeUsers(users); // update user files for removed user.

		prompt = "Success";
		send(new_fd, prompt.c_str(), 127, 0);
		return true;
	}
	else{
		prompt = "Failure";
		send(new_fd, prompt.c_str(), 127, 0);
		return false;
	}

}

// Compares username and password for given user.
bool validateLogin(std::string username, std::string password, std::vector<User> users){
	for (int i=0; i < users.size(); i++){
		if(users[i].getUsername() == username){
			if(users[i].getPassword() == password){
				return true;
			}
		}
	}
	return false;
}


// recieves a date/time from client with correct formatting.
std::string receiveDateTime(int new_fd, char* recvbuf, long numbytes){
		std::string dateTime = "";
		std::string time = "";
		std::string day = "";
		std::string month = "";
		std::string year = "";

		// recieve day
		numbytes = recv(new_fd, recvbuf, 127, 0);
		recvbuf[numbytes] = '\0';
		day = recvbuf;
		std::cout << "[Client]: day: " << day << std::endl;

		// recieve month
		numbytes = recv(new_fd, recvbuf, 127, 0);
		recvbuf[numbytes] = '\0';
		month = recvbuf;
		std::cout << "[Client]: month: " << month << std::endl;

		// recieve year
		numbytes = recv(new_fd, recvbuf, 127, 0);
		recvbuf[numbytes] = '\0';
		year = recvbuf;
		std::cout << "[Client]: year: " << year << std::endl;
		
		// recieve time
		numbytes = recv(new_fd, recvbuf, 127, 0);
		recvbuf[numbytes] = '\0';
		time = recvbuf;
		std::cout << "[Client]: time: " << time << std::endl;

		if (time[0] == ' '){
			dateTime = day + " " + month + " " + year + time;
		}
		else{
			dateTime = day + " " + month + " " + year + " " + time;

		}

		return dateTime;
}