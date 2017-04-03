// Jordan Stein
// CSCI 4761
// Mar 28 2017

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <algorithm>
#include <vector>
#include <string.h>
#include "user.h"
#include "functions.h"

#define MYPORT 3594
#define BACKLOG 10

void sigchld_handler(int s){
	while(wait(NULL) > 0);
}

std::vector<User> users;

int main(){
    if (!readUsers(users)){
        return EXIT_FAILURE; // readUsers returns false if the data file fails to read
    }
    
    int sockfd, new_fd;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    int sin_size;
    struct sigaction sa;
    int yes = 1;
    char *recvbuf;
    char *caddr;
    char* recieved;
    long numbytes;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
	sleep(2);
        exit(1);
    }

    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
        perror("setsockopt");
     	sleep(2);
	exit(1);
    }

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MYPORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(my_addr.sin_zero), '\0', 8);

    if(bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1){
        perror("bind");
     	sleep(2);
	exit(1);
    }

    if(listen(sockfd, BACKLOG) == -1){
        perror("listen");
      	sleep(2);
	exit(1);
    }

    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if(sigaction(SIGCHLD, &sa, NULL) == -1){
        perror("sigaction");
     	sleep(2);
	exit(1);
    }

	std::cout << "Server\n\n";

	std::vector<User> users;

	if (!readUsers(users)) { // readUsers returns false if the data file fails to read
		return EXIT_FAILURE;
	}



	while(true){
		sin_size = sizeof(struct sockaddr_in);

        if((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, (socklen_t*) &sin_size)) == -1){
            perror("accept");
            exit(1);
        }
        printf("server: got connection from %s\n",  inet_ntoa(their_addr.sin_addr));
	

	   if (!fork()){
		    close(sockfd);
		    recvbuf = (char*) calloc(128, sizeof(char));

		    std::string send_buf;
            std::string menuChoice = "";
            std::string username;
            std::string password;
            User user; // user that is logging in


            // read all current users into vector
            std::vector<std::string> currentUsers;
            readLogIn(currentUsers);

            // login validation vars
            bool loggedIn = false;
            bool validLogin = false;

            while(!loggedIn){
        	   // send menu
        	   std::string loginMenu = "Enter Selection:\n"
        						"1 ) Login\n"
								"2 ) Create Account\n";
        	   send(new_fd, loginMenu.c_str(), 255, 0);

                // recieve menu choice
                numbytes = recv(new_fd, recvbuf, 127, 0);
                recvbuf[numbytes] = '\0';
                menuChoice = recvbuf;
                std::cout << "[Client]: login menu choice: " << menuChoice << "\n";

                if(!menuChoice.compare("1")){
                    while(!validLogin){
        	            // receive username
        	            numbytes = recv(new_fd, recvbuf, 127, 0);
                        recvbuf[numbytes] = '\0';
                        
                        std::cout << "Client: username: " << recvbuf << "\n";
                        username = recvbuf;

                        // recieve password
                        numbytes = recv(new_fd, recvbuf, 127, 0);
                        recvbuf[numbytes] = '\0';
                        password = recvbuf;

                        // disallow multiple logins with same username simultaneously
                        bool alreadyloggedin = false;
                        for (int i=0; i < currentUsers.size(); i++){
                            if (username == currentUsers[i]){
                                alreadyloggedin = true;
                            }
                        }
                        if(alreadyloggedin){
                            send_buf = "alreadyLoggedIn\n";
                            send(new_fd, send_buf.c_str(), 127, 0);
                            std::cout << "[Server] User already logged in.\n";
                        }
                        // validate username + password combination
                        else if (validateLogin(username, password, users)){
                            // send login validation
                            send_buf = "Success\n";
                            send(new_fd, send_buf.c_str(), 127, 0);
                            //std::cout << "[Server] " << send_buf << "\n";
                            std::cout << "[Server] Login Successful\n";

                            for (int i=0; i < users.size(); i++){ // find user logging in, store them in user variable
                                if (username == users[i].getUsername()){
                                    user = users[i];
                                    break;
                                }
                            }

                            validLogin = true;
                            loggedIn = true;
                        }
                        else {
                            send_buf = "Failure\n";
                            // send login validation
                            send(new_fd, send_buf.c_str(), 127, 0);
                            std::cout << "[Server] Invalid username / password combination.\nRetry login.\n";
                        }
                    }
                }
                else if (!menuChoice.compare("2")){

                    std::string name = "";
                    std::string phone = "";
                    std::string email = "";

                    // ensures the new user isn't already in the database.
                    while(true){
                        // recieve username from client
                        numbytes = recv(new_fd, recvbuf, 127, 0);
                        recvbuf[numbytes] = '\0';
                        std::cout << "Client: desired username: " << recvbuf << '\n';
                        username = recvbuf;

                        bool uniqueUsername = true;
                        // check all users for chosen username
                        for(int i=0; i < users.size(); i++){
                            if (users[i].getUsername() == username){
                                uniqueUsername = false;
                            }
                        }

                        if(uniqueUsername){
                            send_buf = "Success\n";
                            send(new_fd, send_buf.c_str(), 127, 0);
                            break; // exit loop if username is unique
                        }
                        else{
                            send_buf = "Failure\n";
                            send(new_fd, send_buf.c_str(), 127, 0);
                            std::cout << "[Server]: Username taken, retry new username.\n"; 
                        }
                    }

                    // recieve password information from client
                    numbytes = recv(new_fd, recvbuf, 127, 0);
                    recvbuf[numbytes] = '\0';
                    password = recvbuf;
                    std::cout << "Client: password submitted\n";

                    // recieve name information from client
                    numbytes = recv(new_fd, recvbuf, 127, 0);
                    recvbuf[numbytes] = '\0';
                    name = recvbuf;
                    std::cout << "Client: name: " << name << std::endl; 
                
                    // recieve phone information from client
                    numbytes = recv(new_fd, recvbuf, 127, 0);
                    recvbuf[numbytes] = '\0';
                    phone = recvbuf;
                    std::cout << "Client: phone: " << phone << std::endl;

                    // recieve email information from client
                    numbytes = recv(new_fd, recvbuf, 127, 0);
                    recvbuf[numbytes] = '\0';
                    email = recvbuf;
                    std::cout << "Client: email: " << email << std::endl;

                    User newUser = User(username, password, name, phone, email);

                    addUser(newUser, users); // add the new user to users vector
                    writeUsers(users); // write new user to file.

                    user = newUser;
                }
            }


            if (currentUsers.size() == 0){
                currentUsers.push_back(username);   
            }

            writeLogIn(username, currentUsers, true); // add logged in user to file

            while (loggedIn) { // User will now be logged in
                // Create main menu
                std::string mainMenu =  "Enter Selection:\n"
                                    "1 ) Add a new appointment\n"
                                    "2 ) Remove an appointment\n"
                                    "3 ) Update an existing appointment\n"
                                    "4 ) Check appointment time conflicts\n"
                                    "5 ) Display appointments for specific time / time range\n"
                                    "6 ) Modify user information\n"
                                    "7 ) DELETE USER\n"
                                    "e ) exit\n\n";
                // send menu options to client
                send(new_fd, mainMenu.c_str(), 255, 0);

                // recieve menu choice from client
                numbytes = recv(new_fd, recvbuf, 127, 0);
                recvbuf[numbytes] = '\0';
                menuChoice = recvbuf;
                std::cout << "[Client]: main menu choice: " << menuChoice << "\n";

                int intMenuChoice = 0;
                try{
                    intMenuChoice = std::stoi(menuChoice);
                }catch(int n){
                    throw;
                }catch(...){

                }

                if (menuChoice == "e" || menuChoice == "E" || menuChoice == "exit" || menuChoice == "Exit" || menuChoice == "EXIT"){
                    intMenuChoice = 8;
                }

                switch(intMenuChoice){ // main menu driver
                    case 1:
                        createAppointment(user, users, new_fd, recvbuf, numbytes);
                        break;
                    case 2:
                        removeAppointment(user, users, new_fd, recvbuf, numbytes);
                        break;
                    case 3:
                        updateAppointment(user, users, new_fd, recvbuf, numbytes);
                        break;
                    case 4:
                        checkTimeConflicts(user, users, new_fd, recvbuf, numbytes);
                        break;
                    case 5:
                        displayAppointments(user, users, new_fd, recvbuf, numbytes);
                        break;
                    case 6:
                        modifyUser(user, users, new_fd, recvbuf, numbytes);
                        break;
                    case 7:{
                        bool deleted = deleteUser(user, users, new_fd, recvbuf, numbytes);
                        
                        if (deleted){
                            printf("server: User from %s logged out.\n",  inet_ntoa(their_addr.sin_addr));
                            loggedIn = false;
                            writeLogIn(username, currentUsers, false);
                        }

                        break;
                    }
                    case 8:{
                        printf("server: User from %s logged out.\n",  inet_ntoa(their_addr.sin_addr));
                        writeUsers(users);
                        loggedIn = false;
                        writeLogIn(username, currentUsers, false);
                        break;
                    }
                    default:
                        break;
                }
            }
            close(new_fd);
            break;
        }
        close(new_fd);
    }
	return EXIT_SUCCESS;
}
