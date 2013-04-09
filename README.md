#QH

##Requirements
For now the project is only written with Unix like systems in mind.
It will not work under Windows yet.

Once the project has reached its first RC, I will rewrite what is needed
for it to be compiled for Windows.

Once that is done, there will also be executables for Windows OSs.

##Compile
Nothing extra is needed for compiling

No binaries are included. A compile is necessary.
Tested with g++ in Linux Mint for both 32 bit & 64 bit installs.

##First run
The first time you run the program you will need to give it the
full path of your hosts file.

Once that is done, you can proceed to create profiles

##Profiles
Profiles are used to quickly switch between different "modes"
The idea behind it is very simple.

If you're at work and have to connect to internal IP addresses
for whatever reasons, you would most likely add entries in your
hosts file.
However when you get home, the same services might be on a different
IP address and so you would have to go to the hosts file and manually
change the entries.

With this however, you can just setup your profiles and switch between
them whenever you need to.

In later versions it is planned for network detection. So when it
detects your work network, it will switch the profile automatically.

##Arguments
This program accepts arguments.

Which are as follows

###Switching profiles
	./qh -p home //Will switch to a profile named "home"

###Adding profiles
	./qh -c 127.0.0.1 test //Create a new profile with the contents of 127.0.0.1 test

###Removing profiles
	./qh -r test //This will remove a profile called test

###Setup
	./qh -h //This will initiate the setup process
