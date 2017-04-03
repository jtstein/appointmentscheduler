#pragma once

// Jordan Stein
// CSCI 4761
// Mar 28 2017

#ifndef _users
#define _users

#include <string>
#include <vector>

class User {

private:
	std::string username = "";
	std::string name = "";
	std::string password = "";
	std::string phone = "";
	std::string email = "";
	std::vector<std::string> appointments; // stores all appointment times
	std::vector<std::string> descriptions; // stores descriptions of appointments

public:
	User(std::string, std::string, std::string, std::string, std::string); // constructor
	User(); // default constructor
	~User(); // destructor

	// Getters / Setters
	void setUsername(std::string);
	std::string getUsername();
	void setName(std::string);
	std::string getName();
	void setPassword(std::string);
	std::string getPassword();
	void setPhone(std::string);
	std::string getPhone();
	void setEmail(std::string);
	std::string getEmail();
	void setAppointments(std::vector<std::string>);
	std::vector<std::string> getAppointments();
	void setDescriptions(std::vector<std::string>);
	std::vector<std::string> getDescriptions();

	// Adds a created appointment to the users appointment and description lists
	void addAppointment(std::string);
	void addDescription(std::string);

	// sorts all appointments chronologically
	void sortAppointments();

	// deletes all appointments / descriptions for the user
	void deleteAllAppointments();

	// returns a string of all appointments/descriptions for the user
	std::string returnAppointments();
	// returns a string of all user data
	std::string returnUserData();

	// deletes appointment at the index given.
	void deleteAppointment(int apptIndex);

	// updates appointment. first entry is the appointment number, second is new data, third boolean is date/time(true) or description(false) update
	void updateAppointment(int, std::string, bool);

	bool operator==(const User& user);  // overload == operator to compare two user objects
};

#endif