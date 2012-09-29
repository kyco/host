#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <unistd.h>

using namespace std;

int option;
string dir;
string global_host_path;
string header = "#quick_hosts_profile";

void menu();
void menu_option(int option);
string check_host_path();
string view_hosts();
int setup(string switch_host_file);
void switch_profile(string profile);
void choose_profile();
void arguments(string ts, string a, int test);
void add_profile(string add_profile);
void remove_profile(string r_profile);

int main(int argc, char const *argv[]) {
	string the_switch;
	string argument;
	int quick_incr = 0;

	global_host_path = check_host_path();

	if (argc % 2 != 0) {
	   	for(int i = 1; i < argc; i = i + 2) {
	   		quick_incr = i + 1;
	   		the_switch = argv[i];
	   		argument = argv[quick_incr];

	   	}
	    if (the_switch == "")
	    {
	    	menu();
	    }
	    else
	    	arguments(the_switch.c_str(),argument.c_str(),argc);
	}
	else
		exit(1);
	std::cin.ignore(1024, '\n');
  	std::cout << "press enter to continue ";
  	std::cin.get();
	return 0;
}

void menu() {
	cout << "===========================================================" << endl;
	cout << "Options: " << endl;
	cout << "1. Switch profile" << endl;
	cout << "2. Read hosts file" << endl;
	cout << "3. Add a new profile" << endl;
	cout << "4. Remove a profile" << endl;
	cout << "5. Setup hosts file" << endl;
	cout << "Option: ";
	cin >> option;
	menu_option(option);
}

void menu_option(int option) {
	string host_file;
	switch(option) {
		case 1:
			choose_profile();
			break;
		case 2:
			host_file = view_hosts();
			cout << host_file;

			//Used to wait for user input before returning to the main menu
			std::cin.ignore(1024, '\n');
  			std::cout << "press enter to continue ";
  			std::cin.get();
  			menu();
			break;
		case 3:
			add_profile("1");
			break;
		case 4:
			remove_profile("1");
			break;
		case 5:
			setup("1");
			break;
	}
}

string view_hosts() {
	string file;
	string host;
	
	if (global_host_path == "")
	{
		cout << "Please enter the path to your hosts file: " << endl;
		cin >> dir;
		cout << dir << endl;
	}
	else dir = global_host_path;

	ifstream hosts;
	hosts.open (dir.c_str());
        while(!hosts.eof()) // To get you all the lines.
        {
	        getline(hosts,file); // Saves the line in file.
	        host = host+file+"\n";
        }
	hosts.close();
	return host;
}

int setup(string switch_host_file) {
	string file;
	string store_path;
	string partial;
	string folder;
	string get_line;
	bool check = false;

	if (switch_host_file == "1")
	{
		cout << "Please enter the path to your hosts file: " << endl;
		cin >> store_path;
	}
	else
		store_path = switch_host_file;
	ifstream given_host_file;
	given_host_file.open(store_path.c_str());
		while(!given_host_file.eof()){
			getline(given_host_file,get_line);
			if (get_line == header)
				check = true;
		}

	if (check == false){
		ofstream ahf;
		ahf.open(store_path.c_str(),ios::app);
		ahf << "" << endl;
		ahf << header << endl;
		ahf.close();
	}

	given_host_file.close();

	ofstream store_hosts("host_path.ky");
	store_hosts << store_path << endl;
	store_hosts.close();

	cout << "You have selected this to be your hosts file: " << endl;
	ifstream host_file;
	host_file.open ("host_path.ky");
	while(!host_file.eof()) // To get you all the lines.
        {
	        getline(host_file,file); // Saves the line in file.
	        cout << file << endl; // Prints our file.
        }
	host_file.close();
	mkdir("profiles",0777);

	//Used to wait for user input before returning to the main menu
	std::cin.ignore(1024, '\n');
	std::cout << "press enter to continue ";
	std::cin.get();
	menu();

	return 0;
}

void switch_profile(string profile) {
	//Load the profile directory
	chdir("profiles");

	//Load the host file into host
	string working_dir = getcwd(NULL,0);
	string line;
	string host;
	string profile_replace;
	string new_line = "\n";
	host = view_hosts();
	//cout << working_dir << endl;

	//Get the contents of the new profile
	ifstream new_profile;
	new_profile.open(profile.c_str());
	while(!new_profile.eof()){
		getline(new_profile,line);
		profile_replace = profile_replace+new_line+line;
	}
	new_profile.close();
	chdir("..");
	working_dir = getcwd(NULL,0);
	//cout << working_dir << endl;
	profile_replace = header+new_line+profile_replace;

	// let's replace the first needle:
	host.replace(host.find(header),host.length(),profile_replace);
	ofstream replace;
	replace.open(global_host_path.c_str());
	replace << host << endl;
	replace.close();
	cout << host << endl;
}

string check_host_path() {
	string get_path;
	string path;

	ifstream host_path;
	host_path.open ("host_path.ky");
	if (!host_path.fail())
	{
		while(!host_path.eof()){
			getline(host_path,get_path);
				if (get_path != "")
				{
					path = get_path;
				}
		}
	}
	return path;
}

void choose_profile() {
	string contents[50];
	int control = 0;
	string control_unit;
	int host = 0;
	string get;

	DIR *dir;
	struct dirent *ent;
	dir = opendir ("profiles");
	if (dir != NULL) {

		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {
			control_unit = ent->d_name;
			if (control_unit == "." || control_unit == "..")
				continue;
			else {
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

		std::cin.ignore(1024, '\n');
		std::cout << "press enter to continue ";
		std::cin.get();
		menu();
	}
	else {
		setup("");
	}
}

void arguments(string ts, string a, int test) {
	if (ts == "-p")
		switch_profile(a.c_str());
	else if (ts == "-c")
		add_profile(a);
	else if (ts == "-r")
		remove_profile(a);
	else if (ts == "-h")
		setup(a);
	else if (ts != "-h" && ts != "-c" && ts != "-r" && ts != "-p")
		exit(1);
}

void add_profile(string add_profile) {
	string profile;
	string p_name;
	string view_line;

	cout << "Please enter a name for your new profile: " << endl;
	cin >> p_name;
	if (add_profile != "1")
		profile = add_profile;
	else {
		cout << "Please enter the profile constraints: " << endl;
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
	while(!view.eof()){
		getline(view,view_line);
		cout << view_line+"\n";
	}
	view.close();
	chdir("..");

	//Used to wait for user input before returning to the main menu
	std::cin.ignore(1024, '\n');
  	std::cout << "press enter to continue ";
  	std::cin.get();
  	menu();
}

void remove_profile(string r_profile) {
	string profile_remove;
	string contents[50];
	string remove_this_one;
	int control = 0;
	int option;
	int chosen;
	string control_unit;

	if (r_profile == "1"){
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
				else {
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
			if (remove(remove_this_one.c_str()) != 0) {
				cout << "Remove failed. Please try again" << endl;
				chdir("..");
				remove_profile("1");
			}
			else {
				cout << "Remove successful" << endl;
				chdir("..");
			}
		}
		else if (option == 1)
		{
			cout << "Please enter the name of the profile you would like to remove (case sensitive): ";
			cin >> remove_this_one;
			chdir("profiles");
			if (remove(remove_this_one.c_str()) != 0) {
				cout << "Remove failed. Please try again" << endl;
				chdir("..");
				remove_profile("1");
			}
			else {
				cout << "Remove successful" << endl;
				chdir("..");
			}
		}
	}
	else {
		chdir("profiles");
		if (remove(r_profile.c_str()) != 0) {
			cout << "Remove failed. Please try again" << endl;
			chdir("..");
			remove_profile("1");
		}
		else {
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
