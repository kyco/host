/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 * Name: qs
 *
 * Description: quickly change entries in your hosts file
 *              using profiles.
 *
 * Author: Kyluke McDougall
 *
 */

#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <fstream>
#include <windows.h>

using namespace std;

// Menus
void menu(); // Prints out a menu with options
void menu_option(int option); // Handles the options from the menus
void arguments(string ts, string a, int test); // When arguments are passed to the program from a cli
void wait_for_menu(); // Used to remove redundant lines of code when waiting for user input after functions

// Hosts file
string check_host_path(); // Get the location of the host file after it has been setup
string view_hosts(); // Display/Print out the contents of the host file
int setup(string switch_host_file); // Setup all the necessary files and folders

// Profiles
void switch_profile(string profile); // Actually switching the profile
void choose_profile(); // To choose a profile before switching
void view_profiles(); // View the profiles
void remove_profile(string r_profile); // Remove one of the profiles in the list
void add_profile(string add_profile); // Adding a new profile with constraints

// Misc
int first_run();
void no_profiles();

// Start of program
int main(int argc, char const *argv[])
{
    string the_switch; // Holds the actual switch which determines which function is used
    string argument; // Holds the variable passed to the program
    int quick_incr = 0; // Used to increment within the for loop. Required to store argument as it is switch + 1
    
    // Check if the program is being run for the first time
    if (first_run() == 0)
    {
        setup("1");
    }
    
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

    cout << "Main Menu" << endl;
    cout << "-----------------------------------------------------------" << endl << endl;
    cout << "1. Switch profile" << endl;
    cout << "2. Read hosts file" << endl;
    cout << "3. View profiles" << endl;
    cout << "4. Add a new profile" << endl;
    cout << "5. Remove a profile" << endl;
    cout << "9. Setup" << endl;
    cout << "0. Exit" << endl;
    cout << endl << "Option: ";
    cin >> option; // Gets the input of the user
    menu_option(option); // Calls the function which will direct to the user defined function
}

void menu_option(int option) // Once a choice has een made, this function will call the defined function
{
    switch(option) {
        case 1:
             system("cls");
             choose_profile(); // Change active profile
             wait_for_menu(); // Waits for user input before displaying the menu again
             break;
        case 2:
             system("cls");
             cout << view_hosts(); // Display contents of hosts file
             wait_for_menu();
             break;
        case 3:
             system("cls");
             view_profiles(); // View the available profiles
             wait_for_menu();
             break;
        case 4:
             system("cls");
             add_profile("1"); // Add new profile
             wait_for_menu();
             break;
        case 5:
             system("cls");
             remove_profile("1"); // Remove a profile
             wait_for_menu();
             break;
        case 9:
             system("cls");
             setup("1"); // Setup the software config files
             wait_for_menu();
             break;
        case 0:
             exit(0);
        default:
             system("cls");
             menu();
    }
}

void wait_for_menu() // Waits for a user to press enter before displaying the menu
{

    //Used to wait for user input before returning to the main menu
    std::cin.ignore(1024, '\n');
    std::cout << "press enter to continue ";
    std::cin.get();
    system("cls");
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
    cout << endl;
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
    string header = "#quick_hosts_profile"; // A header which defines the beginning of the host_switch configs in the hosts file
    bool check = false; // False until the header is found in the hosts file

    _mkdir("C:\\Program Files\\kyco");
    _mkdir("C:\\Program Files\\kyco\\qs");
    _chdir("C:\\Program Files\\kyco\\qs");

    if (switch_host_file == "1") // If the hosts file needs to be setup, run this.
    {
        store_path = "C:\\Windows\\System32\\drivers\\etc\\hosts";
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
        ahf << "" << endl << endl; // Creates a new line at the end of the file
        ahf << header << endl; // Adds the header at the end of the file
        ahf.close(); // Closes the variable
    }

    given_host_file.close(); // Closes the variable for the hosts file

    ofstream store_hosts("C:\\Program Files\\kyco\\qs\\host_path.ky"); // Opens the file host_path.ky for editing
    store_hosts << store_path << endl; // Adds the path to the config file
    store_hosts.close(); // closes store_hosts

    cout << "You have selected this to be your hosts file: " << endl;
    ifstream host_file; // Initializes the variable for display
    host_file.open ("C:\\Program Files\\kyco\\qs\\host_path.ky"); // Opens the config file
    while(!host_file.eof()) // Loops until the end of the file
    {
        getline(host_file,file); // Saves the each line in file.
        cout << file << endl; // Prints out file.
    }
    host_file.close(); // Closes the config file
    _mkdir("C:\\Program Files\\kyco\\qs\\profiles"); // Creates a folder with permissions for everyone to read and write to later save profiles in

    return 0;
}

void switch_profile(string profile)
{
    string line; // Stores each line 
    string host = view_hosts(); // Stores the host file
    string profile_replace; // Stores the replacement profile
    string new_line = "\n"; // New line
    string host_path = check_host_path(); // Stores the path of the hosts file
    string header = "#quick_hosts_profile"; // Header

    // Load the profile directory
    _chdir("C:\\Program Files\\kyco\\qs\\profiles"); // Changes dir
    string working_dir = getcwd(NULL,0); // Get current working dir

    //Get the contents of the new profile
    ifstream new_profile; // Initialize variable for opening files
    new_profile.open(profile.c_str()); // Open the profile
    while(!new_profile.eof()) // Loop until end of file
    {
        getline(new_profile,line); // Store each line
        profile_replace = profile_replace+new_line+line; // Adds a new line after each line was saved
    }
    new_profile.close(); // Close the variable
    _chdir(".."); // Move to parent folder
    working_dir = getcwd(NULL,0); // Get current working dir
    profile_replace = header+new_line+profile_replace; // Replace the profile

    // let's replace the first needle:
    host.replace(host.find(header),host.length(),profile_replace);
    ofstream replace; // Initialize a variable for editing files
    replace.open(host_path.c_str()); // Open the hosts file
    replace << host << endl; // Replace the hosts file with new hosts file
    replace.close(); // Close the variable
    cout << host << endl; // Print the new hosts file
}

string check_host_path() // Get the path to the hosts file
{
    string get_path; // Temporary stoarage for the path
    string path; // Variable to store the actual path

    ifstream host_path; // Initialize the variable for opening files
    host_path.open ("C:\\Program Files\\kyco\\qs\\host_path.ky"); // Open the config file
    if (!host_path.fail()) // If it does not fail opening the config file
    {
        while(!host_path.eof()) // Loop until end of file
        {
            getline(host_path,get_path); // Store the path in get_path
            if (get_path != "") // If it is not empty
            {
                path = get_path; // Store the result in path
            }
        }
    }
    else // Otherwise print error
    {
        cout << "Can't open config file" << endl;
        menu();
    }
    return path;
}

void choose_profile() // Choose the profile
{
    string contents[50]; // Contents of the profile
    int control = 0;
    string control_unit;
    int host = 0;
    string get;
    
    cout << "Profile Switching" << endl;
    cout << "-----------------------------------------------------------" << endl << endl;
    
    DIR *dir;
    struct dirent *ent;
    dir = opendir ("C:\\Program Files\\kyco\\qs\\profiles");
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
        if (control == 0)
        {
            no_profiles();
        }
        for (int i = 0; i < control; ++i)
        {
            cout << i << ". " << contents[i] << endl;
        }
        cout << endl << "Option: ";
        cin >> host;
        closedir (dir);
        switch_profile(contents[host].c_str());
    }
    else
    {
        setup("1");
    }
}

void add_profile(string add_profile)
{
    string profile;
    string p_name;
    string view_line;

    cout << "Add profile" << endl;
    cout << "-----------------------------------------------------------" << endl << endl;

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
    _chdir("C:\\Program Files\\kyco\\qs\\profiles");
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
    _chdir("..");
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

    cout << "Profile removal" << endl;
    cout << "-----------------------------------------------------------" << endl << endl;

    if (r_profile == "1")
    {
        cout << "1. Remove profile" << endl;
        cout << "2. View all profiles" << endl;
        cout << endl << "Option: ";
        cin >> option;
        cout << "\n";
        system("cls");
        if (option == 2)
        {
            cout << "Please pick a profile to remove" << endl << endl;
            DIR *dir;
            struct dirent *ent;
            dir = opendir ("C:\\Program Files\\kyco\\qs\\profiles");
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
            if (control == 0)
            {
                no_profiles();
            }
            for (int i = 0; i < control; ++i)
            {
                cout << i << ". " << contents[i] << endl;
            }
            cout << endl << "Option: ";
            cin >> chosen;
            _chdir("C:\\Program Files\\kyco\\qs\\profiles");
            remove_this_one = contents[chosen];
            if (remove(remove_this_one.c_str()) != 0)
            {
                system("cls");
                cout << "Remove failed. Please try again" << endl;
                _chdir("..");
                std::cin.ignore(1024, '\n');
                std::cout << "press enter to continue ";
                std::cin.get();
                system("cls");
                remove_profile("1");
            }
            else
            {
                system("cls");
                cout << "Remove successful" << endl;
                _chdir("..");
            }
        }
        else if (option == 1)
        {
            cout << "Please enter the name of the profile you would like to remove (case sensitive): ";
            cin >> remove_this_one;
            _chdir("C:\\Program Files\\kyco\\qs\\profiles");
            if (remove(remove_this_one.c_str()) != 0)
            {
                cout << "Remove failed. Please try again" << endl;
                _chdir("..");
                remove_profile("1");
            }
            else
            {
                cout << "Remove successful" << endl;
                _chdir("..");
            }
        }
    }
    else
    {
        _chdir("C:\\Program Files\\kyco\\qs\\profiles");
        if (remove(r_profile.c_str()) != 0)
        {
            cout << "Remove failed. Please try again" << endl;
            _chdir("..");
            remove_profile("1");
        }
        else
        {
            cout << "Remove successful" << endl;
            _chdir("..");
        }
    }
    //remove(profile_remove.c_str());

    //Used to wait for user input before returning to the main menu
    std::cin.ignore(1024, '\n');
    std::cout << "press enter to continue ";
    std::cin.get();
    system("cls");
    menu();
}


void view_profiles()
{
    string contents[50];
    int control = 0;
    string control_unit;

    cout << "Available profiles" << endl;
    cout << "-----------------------------------------------------------" << endl << endl;

    DIR *dir;
    struct dirent *ent;
    dir = opendir ("C:\\Program Files\\kyco\\qs\\profiles");
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
        no_profiles();
    }
    for (int i = 0; i < control; ++i)
    {
        cout << i << ". " << contents[i] << endl;
    }
    _chdir("..");
    cout << endl;
}

int first_run()
{
    if (GetFileAttributes("C:\\Program Files\\kyco") == INVALID_FILE_ATTRIBUTES) {
        return 0;
    }
    else
    {
        return 1;
    }
}

void no_profiles()
{
    int add_new_profile = 0;
    
    cout << "You currently have no profiles, would you like to add one?" << endl;
    cout << "0. Yes" << endl;
    cout << "1. No" << endl;
    do
    {
         add_new_profile = 0;
         cout << endl << "Option: ";
         cin >> add_new_profile;
    }
    while (add_new_profile > 1);
    
    if (add_new_profile == 0)
    {
        system("cls");
        add_profile("1");
        wait_for_menu();
        menu();
    }
    else if (add_new_profile == 1)
    {
         system("cls");
         menu();
    }
}
