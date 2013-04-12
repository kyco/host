#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <unistd.h>

using namespace std;

void menu(); // Prints out a menu with options
void menu_option(int option); // Handles the options from the menus
string check_host_path(); // Get the location of the host file after it has been setup
string view_hosts(); // Display/Print out the contents of the host file
int setup(string switch_host_file); // Setup all the necessary files and folders
void switch_profile(string profile); // Actually switching the profile
void choose_profile(); // To choose a profile before switching
void arguments(string ts, string a, int test); // When arguments are passed to the program from a cli
void add_profile(string add_profile); // Adding a new profile with constraints
void remove_profile(string r_profile); // Remove one of the profiles in the list
void view_profiles(); // View the profiles
void wait_for_menu(); // Used to remove redundant lines of code when waiting for user input after functions

int main(int argc, char const *argv[])
{
	string the_switch; // Holds the actual switch which determines which function is used
	string argument; // Holds the variable passed to the program
	int quick_incr = 0; // Used to increment within the for loop. Required to store argument as it is switch + 1

	//string host_path = check_host_path(); // Get the location of our hosts file

	if (argc % 2 != 0) // If the argument is an odd number, it is either incomplete or non-existant
	{
		for(int i = 1; i < argc; i = i + 2) // This gets all the switches and arguments (Only 1 of each instance at the moment)
		{
			quick_incr = i + 1;
			the_switch = argv[i]; // The first variable passed is for the switch
			argument = argv[quick_incr]; // The second variable passed is for the argument

		}
		if (the_switch == "") // If there is no switch or argument, proceed straight to the menu
		{
			menu();
		}
		else
			arguments(the_switch.c_str(),argument.c_str(),argc); // Otherwise, direct to the function which deals with arguments
	}
	else
		exit(1); // If nothing here is right, exit to avoid any damage or data loss

	// Wait for user input before doing anything
	std::cin.ignore(1024, '\n');
	std::cout << "press enter to continue ";
	std::cin.get();
	return 0;
}

void arguments(string ts, string a, int test) // Handles which function is called based on the switch value
{
	if (ts == "-p") // -p is to switch the active profile
		switch_profile(a.c_str());
	else if (ts == "-c") // -c is to add a new profile
		add_profile(a);
	else if (ts == "-r") // -r is to remove a profile
		remove_profile(a);
	else if (ts == "-h") // -h is to setup the config files
		setup(a);
	else if (ts != "-h" && ts != "-c" && ts != "-r" && ts != "-p") // If none of these are right, terminate
		exit(1);
}

void menu() // Function to display the menu
{
	int option; // Stores the input of the user

	cout << "===========================================================" << endl;
	cout << "Options: " << endl;
	cout << "1. Switch profile" << endl;
	cout << "2. Read hosts file" << endl;
	cout << "3. View profiles" << endl;
	cout << "4. Add a new profile" << endl;
	cout << "5. Remove a profile" << endl;
	cout << "6. Setup" << endl;
	cout << "Option: ";
	cin >> option; // Gets the input of the user
	menu_option(option); // Calls the function which will direct to the user defined function
}

void menu_option(int option) // Once a choice has een made, this function will call the defined function
{
	switch(option) {
		case 1:
			choose_profile(); // Change active profile
			wait_for_menu(); // Waits for user input before displaying the menu again
			break;
		case 2:
			cout << view_hosts(); // Display contents of hosts file
			wait_for_menu();
			break;
		case 3:
			view_profiles(); // View the available profiles
			wait_for_menu();
			break;
		case 4:
			add_profile("1"); // Add new profile
			wait_for_menu();
			break;
		case 5:
			remove_profile("1"); // Remove a profile
			wait_for_menu();
			break;
		case 6:
			setup("1"); // Setup the software config files
			wait_for_menu();
			break;
	}
}

void wait_for_menu() // Waits for a user to press enter before displaying the menu
{

	//Used to wait for user input before returning to the main menu
	std::cin.ignore(1024, '\n');
	std::cout << "press enter to continue ";
	std::cin.get();
	menu(); // Calls the menu function
}

string view_hosts() // Used to view the hosts files
{
	string file; // Used as a container to save each line
	string host; // Used as a temporary storage variable for the host file information
	string dir; // Stores the location of the host file

	dir = check_host_path(); // Gets the string location of the hosts file

	ifstream hosts; // Initializes a variable for opening a file
	hosts.open (dir.c_str()); // Opens the path given by the variable dir
	while(!hosts.eof()) // Loops until the end of the file
	{
		getline(hosts,file); // Saves each line to the variable called file.
		host = host+file+"\n"; // Adds a new line after each line so that the output is readable
	}
	hosts.close(); // Closes the file
	return host; // Returns the entire output of the file
}

int setup(string switch_host_file) // Sets up the config files for the software
{
	string file; // Stores each line of the hosts file
	string store_path; // Stores the path of the hosts file
	string get_line; // Temporary storage for each line of the hosts file
	string header = "#quick_hosts_profile"; // A header which defines the beginning of the host_switch configs in the hosts fole
	bool check = false; // False until the header is found in the hosts file

	if (switch_host_file == "1") // If the hosts file needs to be setup, run this.
	{
		cout << "Please enter the path to your hosts file: " << endl; // Requests path to hosts file
		cin >> store_path; // Stores the path of the hosts file
	}
	else // Otherwise it has been setup already and will get the path from the variable passed to the function
		store_path = switch_host_file; // Stores the variable in store_path
	ifstream given_host_file; // Initializes a variable to open files
	given_host_file.open(store_path.c_str()); // Opens the file in the path indicated by store_path
	while(!given_host_file.eof()) // Loops until the end of the file
	{
		getline(given_host_file,get_line); // Reads each line individually
		if (get_line == header) // Checks to see if the header line is already present
			check = true; // Sets check to true so that it doesn't add the header again later
	}

	if (check == false) // If the header wasn't found
	{
		ofstream ahf; // Opens the variable ahf to append the file
		ahf.open(store_path.c_str(),ios::app); // Open the actual file indicated by the path in store_path
		ahf << "" << endl; // Creates a new line at the end of the file
		ahf << header << endl; // Adds the header at the end of the file
		ahf.close(); // Closes the variable
	}

	given_host_file.close(); // Closes the variable for the hosts file

	ofstream store_hosts("host_path.ky"); // Opens the file host_path.ky for editing
	store_hosts << store_path << endl; // Adds the path to the config file
	store_hosts.close(); // closes store_hosts

	cout << "You have selected this to be your hosts file: " << endl;
	ifstream host_file; // Initializes the variable for display
	host_file.open ("host_path.ky"); // Opens the config file
	while(!host_file.eof()) // Loops until the end of the file
	{
		getline(host_file,file); // Saves the each line in file.
		cout << file << endl; // Prints out file.
	}
	host_file.close(); // Closes the config file
	mkdir("profiles",0777); // Creates a folder with permissions for everyone to read and write to later save profiles in

	return 0;
}

void switch_profile(string profile)
{
	string line;
	string host = view_hosts();
	string profile_replace;
	string new_line = "\n";
	string host_path = check_host_path();
	string header = "#quick_hosts_profile";

	//Load the profile directory
	chdir("profiles");
	string working_dir = getcwd(NULL,0);

	//Get the contents of the new profile
	ifstream new_profile;
	new_profile.open(profile.c_str());
	while(!new_profile.eof())
	{
		getline(new_profile,line);
		profile_replace = profile_replace+new_line+line;
	}
	new_profile.close();
	chdir("..");
	working_dir = getcwd(NULL,0);
	profile_replace = header+new_line+profile_replace;

	// let's replace the first needle:
	host.replace(host.find(header),host.length(),profile_replace);
	ofstream replace;
	replace.open(host_path.c_str());
	replace << host << endl;
	replace.close();
	cout << host << endl;
}

string check_host_path()
{
	string get_path;
	string path;

	cout << getcwd(NULL,0) << endl;
	ifstream host_path;
	host_path.open ("host_path.ky");
	if (!host_path.fail())
	{
		while(!host_path.eof())
		{
			getline(host_path,get_path);
			if (get_path != "")
			{
				path = get_path;
			}
		}
	}
	else
		cout << "Can't open config file file" << endl;
	return path;
}

void choose_profile()
{
	string contents[50];
	int control = 0;
	string control_unit;
	int host = 0;
	string get;

	DIR *dir;
	struct dirent *ent;
	dir = opendir ("profiles");
	if (dir != NULL)
	{

		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL)
		{
			control_unit = ent->d_name;
			if (control_unit == "." || control_unit == "..")
				continue;
			else
			{
				contents[control] = ent->d_name;
				control++;
			}
		}
		for (int i = 0; i < control; ++i)
		{
			cout << i << ". " << contents[i] << endl;
		}
		cout << "Option: ";
		cin >> host;
		closedir (dir);
		switch_profile(contents[host].c_str());
	}
	else
	{
		setup("");
	}
}

void add_profile(string add_profile)
{
	string profile;
	string p_name;
	string view_line;

	cout << "Please enter a name for your new profile: " << endl;
	cin >> p_name;
	if (add_profile != "1")
		profile = add_profile;
	else
	{
		cout << "Please enter the profile constraints (end with % and press enter to finish): " << endl;
		char new_input[1024];
		cin.getline(new_input, 1024, '%');
		profile=new_input;//cast char array to string
	}
	chdir("profiles");
	ofstream create_profile(p_name.c_str());
	create_profile << profile << endl;
	create_profile.close();
	cout << "\n";
	cout << "Your new profile: " << endl;
	cout << "Name: " << p_name << endl;
	cout << "Contents: ";

	ifstream view;
	view.open(p_name.c_str());
	while(!view.eof())
	{
		getline(view,view_line);
		cout << view_line+"\n";
	}
	view.close();
	chdir("..");
}

void remove_profile(string r_profile)
{
	string profile_remove;
	string contents[50];
	string remove_this_one;
	int control = 0;
	int option;
	int chosen;
	string control_unit;

	if (r_profile == "1")
	{
		cout << "1. Remove profile" << endl;
		cout << "2. View all profiles" << endl;
		cout << "Option: ";
		cin >> option;
		cout << "\n";
		if (option == 2)
		{
			cout << "Please pick a profile to remove" << endl;
			DIR *dir;
			struct dirent *ent;
			dir = opendir ("profiles");
			while ((ent = readdir (dir)) != NULL) {
				control_unit = ent->d_name;
				if (control_unit == "." || control_unit == "..")
					continue;
				else
				{
					contents[control] = ent->d_name;
					control++;
				}
			}
			for (int i = 0; i < control; ++i)
			{
				cout << i << ". " << contents[i] << endl;
			}
			cout << "Option: ";
			cin >> chosen;
			chdir("profiles");
			remove_this_one = contents[chosen];
			if (remove(remove_this_one.c_str()) != 0)
			{
				cout << "Remove failed. Please try again" << endl;
				chdir("..");
				remove_profile("1");
			}
			else
			{
				cout << "Remove successful" << endl;
				chdir("..");
			}
		}
		else if (option == 1)
		{
			cout << "Please enter the name of the profile you would like to remove (case sensitive): ";
			cin >> remove_this_one;
			chdir("profiles");
			if (remove(remove_this_one.c_str()) != 0)
			{
				cout << "Remove failed. Please try again" << endl;
				chdir("..");
				remove_profile("1");
			}
			else
			{
				cout << "Remove successful" << endl;
				chdir("..");
			}
		}
	}
	else
	{
		chdir("profiles");
		if (remove(r_profile.c_str()) != 0)
		{
			cout << "Remove failed. Please try again" << endl;
			chdir("..");
			remove_profile("1");
		}
		else
		{
			cout << "Remove successful" << endl;
			chdir("..");
		}
	}
	//remove(profile_remove.c_str());

	//Used to wait for user input before returning to the main menu
	std::cin.ignore(1024, '\n');
	std::cout << "press enter to continue ";
	std::cin.get();
	menu();
}


void view_profiles()
{
	string contents[50];
	int control = 0;
	string control_unit;

	DIR *dir;
	struct dirent *ent;
	dir = opendir ("profiles");
	while ((ent = readdir (dir)) != NULL)
	{
		control_unit = ent->d_name;
		if (control_unit == "." || control_unit == "..")
			continue;
		else
		{
			contents[control] = ent->d_name;
			control++;
		}
	}
	if (control == 0)
	{
		cout << "You currently have no profiles" << endl;
	}
	for (int i = 0; i < control; ++i)
	{
		cout << i << ". " << contents[i] << endl;
	}
	chdir("..");
}
