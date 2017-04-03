#pragma once

// Jordan Stein
// CSCI 4761
// Mar 28 2017

#ifndef _functions
#define _functions

#include <vector>
#include "user.h"

// Adds a unique user to the users vector
void addUser(User, std::vector<User>&);

// Removes a user from the users vector
void deleteUser(User, std::vector<User>&);

// Prints all users to the console
void printUsers(std::vector<User>);

// Reads all user information from csv file into users vector. Returns false if file fails to read
bool readUsers(std::vector<User>&);

// Writes all user information from vector into text file
void writeUsers(std::vector<User>);

// recieves a date/time from client with correct formatting.
std::string receiveDateTime(int new_fd, char* recvbuf, long numbytes);

// converts a date/time string to a value
long dateToValue(std::string);

// Creates an appointment for a user, along with a description for the appointment
void createAppointment(User&, std::vector<User>&, int new_fd, char* recvbuf, long numbytes);

// Removes an appointment for a user, along with the description for the appointment
void removeAppointment(User& user, std::vector<User>&, int new_fd, char* recvbuf, long numbytes);

// Updates information about an appointment for a user
void updateAppointment(User& user, std::vector<User>& users, int new_fd, char* recvbuf, long numbytes);

// Displays all appointments for the user
void displayAppointments(User user, std::vector<User> users, int new_fd, char* recvbuf, long numbytes);

// Compares username and password for given user.
bool validateLogin(std::string username, std::string password, std::vector<User>);

// Modifies user information
void modifyUser(User& user, std::vector<User>& users, int new_fd, char* recvbuf, long numbytes);

// Checks all user appointments for time conflicts, sends them to client.
void checkTimeConflicts(User user, std::vector<User> users, int new_fd, char* recvbuf, long numbytes);

// Deletes the current user that is logged in. Returns true if user agrees deletion.
bool deleteUser(User& user, std::vector<User>& users, int new_fd, char* recvbuf, long numbytes);

// reads all users who are logged in from a file.
void readLogIn(std::vector<std::string>& currentUsers);

// writes new logged out person to file. boolean determins log in or log out
void writeLogIn(std::string username, std::vector<std::string>& currentUsers, bool login);

#endif