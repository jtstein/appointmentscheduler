// Jordan Stein
// CSCI 4761
// Mar 28 2017
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <algorithm>
#include <iostream>
#include <string>

#define PORT 3594
#define MAXDATASIZE 512

#define clear() printf("\033[H\033[J") // escape code to clear the screen

void sendDateTime(int); // sends a date/time in correct format to server


int main(int argc, char *argv[]){
    clear();
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    char sendbuf[MAXDATASIZE];
    struct hostent *he;
    struct sockaddr_in their_addr;

    //Check for command line arguments
    if(argc != 2){
        fprintf(stderr, "usage: client hostname\n");
        sleep(2);
	exit(1);
    }
    //assign the hostname to the first command line argument
    if((he=gethostbyname(argv[1])) == NULL){
        perror("gethostbyname");
     	sleep(2);
	exit(1);
    }
    // TCP socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
     	sleep(2);
	exit(1);
    }

    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(PORT);
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(&(their_addr.sin_zero), '\0', 8);

    if(connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1){
        perror("connect");
        sleep(2);
	exit(1);
    }

    bool loggedIn = false;
    std::string input;
    while(!loggedIn){
        // recieve menu option
        numbytes = recv(sockfd, buf, 255, 0);
        buf[numbytes] = '\0';
        std::cout << buf;

        //Send menu option
        std::getline(std::cin, input);
        send(sockfd, input.c_str(), 127, 0);
        std::string loginChoice = input;
        bool validLogin = false;

        if (!loginChoice.compare("1")){ // login existing user
        	while(!validLogin){
                // send username
        		std::cout << "Enter username:\n";
        		std::getline(std::cin, input);
        		send(sockfd,input.c_str(), 127, 0);
                // send password
                std::cout << "Enter password:\n";
                std::getline(std::cin, input);
                send(sockfd, input.c_str(), 127,0);

                // recieve login validation
                numbytes = recv(sockfd, buf, 127, 0);
                buf[numbytes] = '\0';
                std::string success = buf;

                if(!success.compare("Success\n")){ // response from server for valid login
                    validLogin = true;
                    loggedIn = true;
                    std::cout << "Log in successful\n";
                }
                else if(!success.compare("alreadyLoggedIn\n")){
                    std::cout << "User is already logged in\n";
                    std::cout << "Accessed Denied.\n";
                    sleep(1.5);
                    return EXIT_SUCCESS;
                }
                else{
                    std::cout << "Invalid username / password combination.\nRetry login.\n";
                }
        	}
        }
        else if (!loginChoice.compare("2")){ // create new user
            
            while (true){ // check if new username is unique
                std::cout << "Enter username:\n";
                std::getline(std::cin, input);
                // send desired "new" username 
                send(sockfd, input.c_str(), 127, 0);

                numbytes = recv(sockfd, buf, 127, 0);
                buf[numbytes] = '\0';
                std::string success = buf;
                bool uniqueUsername = false;
                if (!success.compare("Success\n")){ // response from server for unique username
                    uniqueUsername = true;
                    break; // unique username breaks loop
                }
                else{
                    std::cout << "Username taken, retry new username.\n";
                }
            }

            // validate password on creation
            while (true){
                std::cout << "Enter password:\n";
                std::getline(std::cin, input);
                std::cout << "Re-enter password: (verification)\n";
                std::string pw2 = "";
                std::getline(std::cin, pw2);

                if (input == pw2){
                    break;
                }
                else{
                    std::cout << "Password validation not equal. Re-enter password:\n";
                }
            }

            // send password
            send(sockfd, input.c_str(), 127, 0);

            std::cout << "Enter full name (First Last)\n";
            std::getline(std::cin, input);
            // send name
            send(sockfd, input.c_str(), 127, 0);

            std::cout << "Enter phone number:\n";
            std::getline(std::cin, input);
            // send phone number
            send(sockfd, input.c_str(), 127, 0);

            std::cout << "Enter email:\n";
            std::getline(std::cin, input);
            // send email
            send(sockfd, input.c_str(), 127, 0);

            validLogin = true;
            loggedIn = true;

        }
        else{
            std::cout << "Invalid option selected.\n";
        }
    }
    // now the user is logged in

    // delay for 1 second
    sleep(1);
    // clears the screen
    clear();

    std::string menuChoice = "";
    while(menuChoice != "e"){
    menuChoice = "";

    // clears the screen
    clear();
    // recieve menu option
    numbytes = recv(sockfd, buf, 255, 0);
    buf[numbytes] = '\0';
    std::cout << buf << std::endl;

    //Send menu option
    std::getline(std::cin, input);
    send(sockfd, input.c_str(), 127, 0);

    menuChoice = input;

    // clears the screen
    clear();

    int menuChoiceInt = 0;
    try{
        menuChoiceInt = std::stoi(menuChoice);
    }catch(int n){
        throw n;
    }catch(...){

    }

    bool valid = false;
    switch(menuChoiceInt){
        case 1:{ // add a new appointment

            while(true){ // loop until valid month is submitted
                // prompts use to input month of appointment.
                numbytes = recv(sockfd, buf, 127, 0);
                buf[numbytes] = '\0';
                std::cout << buf;

                // send month info
                std::getline(std::cin, input);
                send(sockfd, input.c_str(), 127, 0);

                // recieve whether month entry is valid from server
                numbytes = recv(sockfd, buf, 127, 0);
                buf[numbytes] = '\0';
                std::string success = buf;
                if(!success.compare("Success")){
                    break;
                }
                else{
                    std::cout << "Invalid month entered. Retry.\n";
                }
            }

            while(true){ // loop until valid day is submitted
                // prompts user to input day of appointment.
                numbytes = recv(sockfd, buf, 127, 0);
                buf[numbytes] = '\0';
                std::cout << buf;

                // send day info
                std::getline(std::cin, input);
                send(sockfd, input.c_str(), 127, 0);

                // recieve whether day entry is valid from server
                numbytes = recv(sockfd, buf, 127, 0);
                buf[numbytes] = '\0';
                std::string success = buf;
                if(!success.compare("Success")){
                    break;
                }
                else{
                    std::cout << "Invalid day entered. Retry.\n";
                }
            }

            while(true){ // loop until valid year is submitted
                // prompts user to input year of appointment.
                numbytes = recv(sockfd, buf, 127, 0);
                buf[numbytes] = '\0';
                std::cout << buf;

                //send year info
                std::getline(std::cin, input);
                send(sockfd, input.c_str(), 127, 0);

                // recieve whether year entry is valid from server
                numbytes = recv(sockfd, buf, 127, 0);
                buf[numbytes] = '\0';
                std::string success = buf;
                if(!success.compare("Success")){
                    break;
                }
                else{
                    std::cout << "Invalid year entered. Retry.\n";
                }
            }

            while(true){ // loop until valid time is submitted
                // prompts user to input year of appointment.
                numbytes = recv(sockfd, buf, 127, 0);
                buf[numbytes] = '\0';
                std::cout << buf;

                //send year info
                std::getline(std::cin, input);
                send(sockfd, input.c_str(), 127, 0);

                // recieve whether time entry is valid from server
                numbytes = recv(sockfd, buf, 127, 0);
                buf[numbytes] = '\0';
                std::string success = buf;
                if(!success.compare("Success")){
                    break;
                }
                else if(!success.compare("FailureMeridiem")){
                    std::cout << "Invalid meridiem entered. Retry.\n";
                }
                else{
                    std::cout << "Invalid hour/min entered. Retry.\n";
                }
            }

            // prompts user to input description of appointment.
            numbytes = recv(sockfd, buf, 127, 0);
            buf[numbytes] = '\0';
            std::cout << buf;

            // send description info
            std::getline(std::cin, input);
            send(sockfd, input.c_str(), 127, 0);

            // tells user appointment has been created.
            numbytes = recv(sockfd, buf, 127, 0);
            buf[numbytes] = '\0';
            std::cout << buf;

           break;
        }
        case 2: { // remove an existing appointment
            std::string appts = "";

            std::cout << "All of your appointments are listed below:\n";
            // recieve all appointments for user
            numbytes = recv(sockfd,buf,1023,0);
            buf[numbytes] = '\0';
            std::cout << buf;

            std::string apptNum = "";

            std::cout << "\nSelect appointment # to remove:     (Enter 'exit' to return to main menu)\n";
            // user inputs the appointment they want to remove, send to server
            std::getline(std::cin, input);
            send(sockfd, input.c_str(), 127, 0);

            std::transform(input.begin(), input.end(), input.begin(), ::tolower); // make input lowercase
            if (input != "exit"){
                // recieve success/failure from server for removing appointment
                numbytes = recv(sockfd, buf, 127, 0);
                buf[numbytes] = '\0';
                std::cout << buf;
            }

            break;
        }

        case 3: { // update an existing appointment
            std::string appts = "";
            std::cout << "All of your appointments are listed below:\n";
            // recieve all appoints for user
            numbytes = recv(sockfd, buf, 1023, 0);
            buf[numbytes] = '\0';
            std::cout << buf;

            std::string apptNum = "";

            std::cout <<"\nSelect appointment # to update:      (Enter 'exit' to return to main menu)\n";
            // user inputs the appoint they want to update, send to server
            std::getline(std::cin, input);
            send(sockfd, input.c_str(), 127, 0);

            std::transform(input.begin(), input.end(), input.begin(), ::tolower); // make input lowercase
            
            apptNum = input;
            if (input != "exit"){
                
                // ask client which part of appointment to update
                std::cout << "Select the entry to update for appointment " << apptNum  << "   (Enter 'exit' to return to main menu)"<< std::endl;
                std::cout << "\n1 ) Date/Time\n2 ) Description\n\n";
                
                std::getline(std::cin, input);
                std::transform(input.begin(), input.end(), input.begin(), ::tolower); // make input lowercase
                // tell server which entry to update
                send(sockfd, input.c_str(), 127, 0);


                if(input == "1"){ // update date/time

                    std::string month = "";
                    while(true){ // prompt user to enter month of appointment. Error check for correct formatting.
                        std::cout <<"\nEnter the month of the appointment: Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec\n";
                        std::cin >> month;
                        std::transform(month.begin(), month.end(), month.begin(), ::tolower); // make month string lowercase
                        month[0] = toupper(month[0]); // make first character uppercase.
                        if (month == "Jan" || month == "Feb" || month == "Mar" || month == "Apr" || month == "May" || month == "Jun" || month == "Jul" || month == "Aug" || month == "Sep" || month == "Oct" || month == "Nov" || month == "Dec"){
                            send(sockfd, month.c_str(), 127, 0);
                            break;
                        }
                        else{
                            std::cerr << "\nInvalid month entered.\n";
                        }
                    }

                    std::string day = "";
                    while(true){ // prompt user to enter the day of the appointment. Error check for correct formatting.
                        std::cout << "\nEnter the day of the appointment: 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31\n";
                        std::cin >> day;

                        int intDay = 0;
                        try{
                            intDay = std::stoi(day);
                        }catch(int n){
                            throw n;
                        }catch(...){

                        }

                        if (day.size() == 1){
                            std::string tempDay = day;
                            day = "0" + tempDay;
                        }

                        if (intDay >= 1 && intDay <= 31){
                            send(sockfd, day.c_str(), 127, 0);
                            break;
                        }
                        else{
                            std::cerr <"\nInvalid date entered.\n";
                        }
                    }

                    std::string year = "";
                    while(true) { // prompt use to enter the year of the appointment. Error check for correct formatting.
                        std::cout << "\nEnter the year of the appointment: 2017 2018 2019 2020 2021 2022 2023 2024 2025 2026 2027 ...\n";
                        std::cin >> year;

                        int intYear = 0;

                        try{    
                            intYear = std::stoi(year);
                        }catch(int n){
                            throw n;
                        }catch(...){

                        }

                        if((intYear) >= 2017){
                            send(sockfd, year.c_str(), 127, 0);
                            break;
                        }
                        else{
                            std::cerr<"\nInvalid year submitted\n";
                        }
                    }

                    std::string timet = "";
                    bool oneDigit = false; // if hour is less than 10

                    while(true){ // prompt user to enter the time of the appointment. Error check for correct formatting.
                        timet = "";
                        //std::cout << year;
                        std::cout << "\nEnter the time of the appointment:  12:00am - 12:00pm\n";
                        std::cin >> timet;
                        int hour = 0;
                        int min = 0;

                       if (timet.size() == 6){ // may happen if user enters 1:00pm etc...
                            std::string tempTime = "       ";
                            tempTime[1] = timet[0];
                            tempTime[2] = timet[1];
                            tempTime[3] = timet[2];
                            tempTime[4] = timet[3];
                            tempTime[5] = timet[4];
                            tempTime[6] = timet[5];
                            timet = tempTime;
                            oneDigit = true;
                        }

                        if (timet.size() == 6){
                            std::string tempTime = timet;
                            timet = "0" + tempTime;
                        }
                    
                        if (timet.size() == 7){
                            try{
                                 hour = std::stoi(timet.substr(0,2)); // check first two charactrs for the hour submitted
                                 min = std::stoi(timet.substr(3, 5)); // check 3rd and 4th characters for the minute submitted
                            }catch(int n){
                                throw n;
                            }catch(...){

                            }

                            timet[5] = tolower(timet[5]); // ensure am / pm is lowercase
                            timet[6] = tolower(timet[6]);

                            if ((hour >= 0 && hour <= 12) && (min >= 0 && min <= 60)) {
                                if ((timet[5] == 'a' || timet[5] == 'p') && timet[6] == 'm') {
                                    send(sockfd, timet.c_str(), 127, 0);
                                    break;
                                }
                                else {
                                    std::cerr << "\nInvalid meridiem submitted\n";
                                    timet = "";
                                }
                            }
                        }
                        else{
                            std::cerr << "\nInvalid hour/min submitted\n";
                            timet = "";
                        }
                    }
                }
                else if(input == "2"){ // update description

                    std::cout << "\nEnter the new description for appointment " + apptNum + ":" << std::endl;
                    std::getline(std::cin, input);

                    // send new description to server
                    send(sockfd, input.c_str(), 127, 0);

                    break;

                }
                else{
                   // break;
                }

            }

            break;
        }

        case 4: { // check appointment time conflicts
            // recieve all time conflicting appointments from server
            numbytes = recv(sockfd, buf, 511, 0);
            buf[numbytes] = '\0';
            std::cout << buf;

            // prompts user to return back to main menu
            std::cout << "\n\nEnter any key to return to main menu.\n";
            std::getline(std::cin, input);

            // send request to return to main menu
            send(sockfd, input.c_str(), 127, 0);

            break;
        }
        case 5:{ // display appointments for specific time / time range
            int intInput = 0;

            // recieve menu options
            numbytes = recv(sockfd, buf, 255, 0);
            buf[numbytes] = '\0';

            bool valid = false;
            while(!valid){
                std::cout << buf;
                std::getline(std::cin, input);

                intInput = 0;
                try{
                    intInput = std::stoi(input);
                }catch(int n){
                    throw n;
                }catch(...){

                }

                if (intInput >= 1 && intInput <= 3){
                    valid = true;
                }else{
                    std::cout << "Invalid input. Retry.\n";
                }
            }

            send(sockfd, input.c_str(), 127, 0);

            if (intInput == 2){ // specific time
                sendDateTime(sockfd);
            }
            else if(intInput == 3){ // specific time range
                std::cout << "Enter the start date/time range:\n";
                sendDateTime(sockfd);
                std::cout << "\nEnter the end date/time range:\n";
                sendDateTime(sockfd);

            }

            // recieve appointment info from server.
            numbytes = recv(sockfd, buf, 1023, 0);
            buf[numbytes] = '\0';
            std::cout << buf;

            // prompts user to return back to main menu
            std::cout << "\n\nEnter any key to return to main menu.\n";
            std::cin >> input;
            // send request to return to main menu
            send(sockfd, input.c_str(), 127, 0);

            break;
        }
        case 6: { //modify user information

            // prompts current user data
            numbytes = recv(sockfd, buf, 127, 0);
            buf[numbytes] = '\0';
            std::cout << buf;

            // prompts use to input choice
            numbytes = recv(sockfd, buf, 127, 0);
            buf[numbytes] = '\0';

            std::string strChoice = "";
            int choice = 0;
            bool valid = false;
            while(!valid){
                std::cout << buf;
                std::cin >> strChoice;

                try{
                    choice = std::stoi(strChoice);
                }catch(int n){
                    throw n;
                }catch(...){

                }

                if (choice >= 1 && choice <= 5){
                    valid = true;
                }else{
                    std::cerr << "\nInvalid Choice, try again.\n";
                }
            }
            //strChoice = std::to_string(choice);
            // send choice to server
            send(sockfd, strChoice.c_str(), 127, 0);

            if(choice == 1){
                std::cout <<"\nInput new name:\n";
                // input new data
                std::cin >> input;
                //send it to server
                send(sockfd, input.c_str(), 127, 0);
            }
            else if(choice == 2){
                std::cout << "\nInput new phone number:\n";
                // input new data
                std::cin >> input;
                //send it to server
                send(sockfd, input.c_str(), 127, 0);
            }
            else if(choice == 3){
                std::cout << "\nInput new email:\n";
                // input new data
                std::cin >> input;
                //send it to server
                send(sockfd, input.c_str(), 127, 0);
            }
            else if(choice == 4){
                std::cout <<"\nInput new password:\n";
                // input new data
                std::cin >> input;
                //send it to server
                send(sockfd,input.c_str(), 127, 0);
            }

            if(choice >= 1 && choice <= 4){
                // recieve success/failure message
                numbytes = recv(sockfd, buf, 127, 0);
                buf[numbytes] = '\0';
                std::cout << buf;
            }

            break;
        }
        case 7:{ // DELETE user
            // recieve request for deletion
            numbytes = recv(sockfd, buf, 127, 0);
            buf[numbytes] = '\0';
            std::cout << buf;

            // user confirms their password to approve deletion.
            std::getline(std::cin, input);
            send(sockfd, input.c_str(), 127, 0);

            // recieve request for deletion
            numbytes = recv(sockfd, buf, 127, 0);
            buf[numbytes] = '\0';
            std::cout << buf;
            std::string success = "";
            success = buf;

            if (!success.compare("Success")){
                std::cout << "\nYour account has been deleted.\n";
                menuChoice = "e"; // exits menu
            }else{
                std::cout << "\nPassword entered did not match.\n";
            }

            break;
        }
        case 8:{ // exit program
            menuChoice = "e"; // exits menu
            break;
        }

        default:
            break;
        }
    } // while menuchoice != 'e'

return EXIT_SUCCESS;
}


void sendDateTime(int sockfd) { // creates a date/time in correct format, sends to server
    std::string month = "";
    while(true){ // prompt user to enter month of appointment. Error check for correct formatting.
        std::cout <<"\nEnter the month of the appointment: Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec\n";
        std::cin >> month;
        std::transform(month.begin(), month.end(), month.begin(), ::tolower); // make month string lowercase
        month[0] = toupper(month[0]); // make first character uppercase.
        if (month == "Jan" || month == "Feb" || month == "Mar" || month == "Apr" || month == "May" || month == "Jun" || month == "Jul" || month == "Aug" || month == "Sep" || month == "Oct" || month == "Nov" || month == "Dec"){
            send(sockfd, month.c_str(), 127, 0);
            break;
        }
        else{
            std::cerr << "\nInvalid month entered.\n";
        }
    }

    std::string day = "";
    while(true){ // prompt user to enter the day of the appointment. Error check for correct formatting.
        std::cout << "\nEnter the day of the appointment: 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31\n";
        std::cin >> day;

        int intDay = 0;
        try{
            intDay = std::stoi(day);
        }catch(int n){
            throw n;
        }catch(...){

        }

        if (day.size() == 1){
            std::string tempDay = day;
            day = "0" + tempDay;
        }

        if (intDay >= 1 && intDay <= 31){
            send(sockfd, day.c_str(), 127, 0);
            break;
        }
        else{
            std::cerr <"\nInvalid date entered.\n";
        }
    }

    std::string year = "";
    while(true) { // prompt use to enter the year of the appointment. Error check for correct formatting.
        std::cout << "\nEnter the year of the appointment: 2017 2018 2019 2020 2021 2022 2023 2024 2025 2026 2027 ...\n";
        std::cin >> year;

        int intYear = 0;

        try{    
            intYear = std::stoi(year);
        }catch(int n){
            throw n;
        }catch(...){

        }

        if((intYear) >= 2017){
            send(sockfd, year.c_str(), 127, 0);
            break;
        }
        else{
            std::cerr<"\nInvalid year submitted\n";
        }
    }

    std::string timet = "";
    bool oneDigit = false; // if hour is less than 10

    while(true){ // prompt user to enter the time of the appointment. Error check for correct formatting.
        timet = "";
        //std::cout << year;
        std::cout << "\nEnter the time of the appointment:  12:00am - 12:00pm\n";
        std::cin >> timet;
        int hour = 0;
        int min = 0;

       if (timet.size() == 6){ // may happen if user enters 1:00pm etc...
            std::string tempTime = "       ";
            tempTime[1] = timet[0];
            tempTime[2] = timet[1];
            tempTime[3] = timet[2];
            tempTime[4] = timet[3];
            tempTime[5] = timet[4];
            tempTime[6] = timet[5];
            timet = tempTime;
            oneDigit = true;
        }

        if (timet.size() == 7){
            try{
                 hour = std::stoi(timet.substr(0,2)); // check first two charactrs for the hour submitted
                 min = std::stoi(timet.substr(3, 5)); // check 3rd and 4th characters for the minute submitted
            }catch(int n){
                throw n;
            }catch(...){

            }

            timet[5] = tolower(timet[5]); // ensure am / pm is lowercase
            timet[6] = tolower(timet[6]);

            if ((hour >= 0 && hour <= 12) && (min >= 0 && min <= 60)) {
                if ((timet[5] == 'a' || timet[5] == 'p') && timet[6] == 'm') {
                    send(sockfd, timet.c_str(), 127, 0);
                    return;
                }
                else {
                    std::cerr << "\nInvalid meridiem submitted\n";
                    timet = "";
                }
            }
        }
        else{
            std::cerr << "\nInvalid hour/min submitted\n";
            timet = "";
        }
    }
}
