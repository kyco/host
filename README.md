#QS

##Description
QS is designed for people who edit their hosts file a lot.
The idea behind it is simple, profiles.

Instead of having to comment out lines in your hosts file,
simply create profiles for each specific case.
For instance a home profile:

    127.0.0.1 localhost testwebsite google.co.za
    192.168.0.3 owncloud home.server

Work profile:

    127.0.0.1 big.project.1
    172.16.10.1 cloud.work cups.server

Friends house profile:

    10.0.0.1 warcraft3server file.sharing.server
    10.0.0.5 friends.homewebsite

###Example
An example of a hosts file with qs in use:

    # The following lines are desirable for IPv6 capable hosts
    ::1     ip6-localhost ip6-loopback
    fe00::0 ip6-localnet
    ff00::0 ip6-mcastprefix
    ff02::1 ip6-allnodes
    ff02::2 ip6-allrouters

    #quick_hosts_profile

    127.0.0.1 localhost testwebsite google.co.za
    192.168.0.3 owncloud home.server

##Requirements
A PC running Windows XP or later. Linux version is available in the master branch.

##Compile
You will need a C++ compiler to compile this software.

The easiest way, for new users, is to install Bloodshed DevC++ and use it to compile.

	http://www.bloodshed.net/devcpp.html

##First run
The first time you run the program, it will require you to setup the software.
This setup is usually done automaticall unless there was a problem.

##Profiles
Profiles are used to quickly switch between different "modes".
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

Which are as follows.

###Switching profiles
This will switch to a profile named "home"

	qs -p home

###Adding profiles
Create a new profile with the contents of 127.0.0.1 test

	qs -c 127.0.0.1 test

###Removing profiles
This will remove a profile called test

	qs -r test

###Setup
This will initiate the setup process

	qs -h

Support
-------

For bugs or improvements please use the [issues tab](https://github.com/kyco/host/issues)
or email [support@kycosoftware.com](mailto:support@kycosoftware.com).
