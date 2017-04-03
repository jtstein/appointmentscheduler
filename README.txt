Created by: Jordan Stein

This program is an online appointment scheduling database via TCP. The two folders listed include a client and a server that operate all functionalities. Makefiles are included for both the client in the server that create executable images for runServer and runClient.

This program was tested and build on Ubuntu 14.04. It should operate properly on any recent distribution of linux.

The server must be ran before the client. 
In order to run the server:
1 ) Go to the server directory and enter 'make' to compile all files
2 ) enter './runServer'
**If there are object files, enter 'make clean' before step 1**

In order to run the client:
1 ) Go to the client directory and enter 'make' to compile all files
2 ) Enter './runClient {ip address server is hosted on}'
* note, the ip address of the machine must be passed as an argument for runClient.
**If there are object files, enter 'make clean' before step 1**

*Warning: (username already logged in)
Do not close the client with ctrl+c.
The functionality of disallowing multiple log-ins logs the user out when they properly exit the program (typing e during the main menu screen.)
Closing the program with ctrl+c will keep the user logged in, disallowing them to log back in with the same username.
If for any reason you run into this issue, you can fix it by opening the loggedIn csv file in the server directory and removing the username from the file / saving it.
