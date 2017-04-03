// Jordan Stein
// CSCI 4761
// Mar 28 2017
#include <iostream>
#include "user.h"
#include "functions.h"

// Overload == operator to compare two User objects
bool User::operator==(const User& user) {
	return ((name == user.name && phone == user.phone && email == user.email));
}

// Constructor
User::User(std::string username, std::string password, std::string name, std::string phone, std::string email) {
	this->username = username;
	this->password = password;
	this->name = name;
	this->phone = phone;
	this->email = email;
}

// Default Constructor
User::User() {
	this->name = "";
	this->password = "";
	this->phone = "";
	this->email = "";
}

// Destructor
User::~User() {

}

// Getters / Setters
void User::setUsername(std::string username) {
	this->username = username;
}

std::string User::getUsername() {
	return this->username;
}
void User::setName(std::string name) {
	this->name = name;
}

std::string User::getName() {
	return this->name;
}

void User::setPassword(std::string password) {
	this->password = password;
}

std::string User::getPassword() {
	return this->password;
}

void User::setPhone(std::string phone) {
	this->phone = phone;
}

std::string User::getPhone() {
	return this->phone;
}

void User::setEmail(std::string email) {
	this->email = email;
}

std::string User::getEmail() {
	return this->email;
}

void User::setAppointments(std::vector<std::string> appointments) {
	this->appointments = appointments;
}

std::vector<std::string> User::getAppointments() {
	return this->appointments;
}

void User::setDescriptions(std::vector<std::string> description) {
	this->descriptions = description;
}

std::vector<std::string> User::getDescriptions() {
	return this->descriptions;
}

// Adds a created appointment to the users appointment and description lists
void User::addAppointment(std::string date) {
	this->appointments.push_back(date);
}

void User::addDescription(std::string description) {
	this->descriptions.push_back(description);
}

// sorts all appointments chronologically
void User::sortAppointments(){

	for (int i=0; i < this->appointments.size(); i++){
		for (int j=i+1; j < this->appointments.size(); j++){
			if (dateToValue(this->appointments[i]) > dateToValue(this->appointments[j])){
				std::string tempAppt = this->appointments[i];
				this->appointments[i] = this->appointments[j];
				this->appointments[j] = tempAppt;

				std::string tempDesc = this->descriptions[i];
				this->descriptions[i] = this->descriptions[j];
				this->descriptions[j] = tempDesc;
			}
		}
	}
}

// Returns string of all appointments/descriptions for the user
std::string User::returnAppointments() {
	std::string appts = "";
	for (int i = 0; i < this->appointments.size(); i++) {
		appts += "Appointment " + std::to_string(i + 1) + '\n';
		appts += "Date / Time:	" + this->appointments[i] + '\n';
		appts += "Description:	 " + this->descriptions[i] + '\n';
	}

	return appts;
}
// Returns a string of all user data
std::string User::returnUserData(){
	std::string data = "";
	data += "Your user information is listed below:\n";
	data += "name: " + this->name + "\n";
	data += "phone: " + this->phone + "\n";
	data += "email: " + this->email + "\n";

	return data;
}


// deletes an appointment at the index given.
void User::deleteAppointment(int apptIndex){
	this->appointments.erase(this->appointments.begin() + apptIndex);
	this->descriptions.erase(this->descriptions.begin() + apptIndex);
}

// deletes all appointments / descriptions for the user
void User::deleteAllAppointments(){
	this->appointments.clear();
	this->descriptions.clear();
}

// updates appointment. first entry is the appointment number, second is new data, third boolean is date/time(true) or description(false) update
void User::updateAppointment(int apptNum, std::string data, bool time){

	if(time == true){ // date/time update
		this->appointments[apptNum] = data;
	}
	else { // description update
		this->descriptions[apptNum] = data;
	}


}