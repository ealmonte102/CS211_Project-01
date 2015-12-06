// RosterSystem.cpp
// Created by Evan Almonte
//
#include "RosterSystem.hpp"
#include "Roster.hpp"
#include "RSFileManager.hpp"
#include "Student.hpp"
#include "Utilities.hpp"
#include <iostream>

using std::cout;
using std::cin;
using std::string;

namespace RosterSystemUtils {
	char getMenuOptSelection(bool isAdmin);
	char getLoginOptSelection( );
	string getStarDesign(int length);
}

const string RosterSystem::menuOpts[numOfMenuOpts] = {
	"Create a new Roster",
	"Drop a Roster",
	"Display Roster Information",
	"Display All Rosters",
	"Select a Roster"
};
const string RosterSystem::selectOpts[numOfSelectOpts] = {
	"Add a New Student",
	"Remove a Student",
	"Update a Student",
	"List All Students"
};

RosterSystem::RosterSystem( ) : loginStatus(NOT_LOGGED), rListSz(0), rListCap(0), rosterList(nullptr),
                                eListSz(0), eListCap(0), enrollmentList(nullptr) {
	RSFileManager import("rosters.txt", false);
	import.importRosters(rosterList, rListSz, rListCap, enrollmentList, eListSz, eListCap);
}

RosterSystem::~RosterSystem( ) {
	for (int i = 0; i < rListSz; ++i) {
		delete rosterList[i];
	}
	delete[] rosterList;
	for (int i = 0; i < eListSz; ++i) {
		delete enrollmentList[i];
	}
	delete[] enrollmentList;
}

void RosterSystem::loginMenu( ) {
	clearScreen();
	loginStatus = USER;
	cout << "********************************\n";
	cout << "  Welcome To SchoolManager v.1  \n";
	cout << "********************************\n";
	cout << "A) Supervisor Mode\n";
	cout << "B) User Mode\n";
	cout << "C) Exit\n";
	cout << "Please choose an option: ";
	char choice = RosterSystemUtils::getLoginOptSelection();
	switch (choice) {
		case 'A':
		case 'a':
			{
				RSFileManager database("database.txt", false);
				loginStatus = database.attemptLogin();
			}
		case 'B':
		case 'b':
			mainMenu();
			loginMenu();
			break;
		case 'C':
		case 'c':
			{
				RSFileManager writeRosters("rosters.txt", true);
				writeRosters.exportRosters(rosterList, rListSz);
			}
			break;
		default:
			cout << "Invalid choice please try again.\n";
			cout << "Press ENTER to continue . . .";
			cin.get();
			loginMenu();
	}
}

void RosterSystem::mainMenu( ) {
	clearScreen();
	cout << "********************************\n";
	cout << "           Main Menu            \n";
	cout << "********************************\n";
	if (loginStatus == SUPERVISOR) {
		displayAdminMenu();
	} else {
		displayUserMenu();
	}
	cout << "Please choose an option(q to Quit): ";
	char choice = RosterSystemUtils::getMenuOptSelection(loginStatus == SUPERVISOR);

	switch (choice) {
		case '1':
			addRoster();
			break;
		case '2':
			{
				string courseNumber;
				cout << "Please enter the rosters course number: ";
				getline(cin, courseNumber);
				removeRoster(courseNumber);
			}
			break;
		case '3':
			{
				cout << "Please enter the rosters course number: ";
				string courseNumber;
				getline(cin, courseNumber);
				displayRoster(courseNumber);
			}
			break;
		case '4':
			displayAllRosters();
			break;
		case '5':
			{
				string courseNumber;
				cout << "Please enter the rosters course number: ";
				getline(cin, courseNumber);
				selectRoster(courseNumber);
			}
			break;
		case 'Q':
		case 'q':
			cout << "Exiting to login menu.\n";
			return;
		default:
			mainMenu();
	}
	cout << "Press ENTER to continue . . .";
	cin.get();
	mainMenu();
}

void RosterSystem::displayAdminMenu( ) {
	for (int i = 0; i < numOfMenuOpts; ++i) {
		cout << i + 1 << ") " << menuOpts[i] << "\n";
	}
}

void RosterSystem::displayUserMenu( ) {
	;
	cout << 1 << ") " << menuOpts[numOfMenuOpts - 1] << "\n";
}

void RosterSystem::addToEnrollmentList(Student* aStudent) {
	if (eListSz == eListCap) {
		growEnrollmentList();
	}
	enrollmentList[eListSz++] = aStudent;
}

void RosterSystem::addToEnrollmentAndRoster(Roster& selectedRoster) {
	clearScreen();
	if (eListSz == eListCap) {
		growEnrollmentList();
	}
	Student* studentToAdd = new Student();
	cout << "********************************\n";
	cout << "        New Student Added       \n";
	cout << "********************************\n";
	cin >> *studentToAdd;
	enrollmentList[eListSz++] = studentToAdd;
	selectedRoster.addStudent(studentToAdd);
}

int RosterSystem::findRoster(std::string courseCode) const {
	if (rListSz == 0) {
		cout << "There are no rosters in the system.\n";
		return EMPTY_LIST;
	}
	int foundIndex = NOT_FOUND;
	for (int i = 0; i < rListSz; ++i) {
		if (upperConvert(courseCode) == upperConvert(rosterList[i]->getCourseCode())) {
			foundIndex = i;
		}
	}
	if (foundIndex == NOT_FOUND) {
		cout << "No roster with a course number of: " << courseCode << " was found.\n";
	}
	return foundIndex;
}

void RosterSystem::addRoster( ) {
	clearScreen();
	if (rListSz == rListCap) {
		growRosterList();
	}
	Roster* rosterToAdd = new Roster;
	cout << "********************************\n";
	cout << "       New Roster Created       \n";
	cout << "********************************\n";
	cin >> *rosterToAdd;
	rosterList[rListSz++] = rosterToAdd;
}

void RosterSystem::selectRoster(std::string courseNumber) {
	int location = findRoster(courseNumber);
	if (location == NOT_FOUND || location == EMPTY_LIST) {
		return;
	}
	Roster* rosterToEdit = rosterList[location];
	do {
		if (loginStatus == SUPERVISOR) {
			adminSelectOpts(*rosterToEdit);
		} else {
			userSelectOpts(*rosterToEdit);
		}
		cout << "Would you like to continue editing the same roster(Y/N)? ";
	} while (getYesOrNo());
}

void RosterSystem::removeRoster(std::string courseCode) {
	int location = findRoster(courseCode);
	if (location == NOT_FOUND || location == EMPTY_LIST) {
		return;
	}
	clearScreen();

	//		Displays a title as:
	//*******************************
	//  CourseName Has Been Deleted 
	//*******************************

	string titleBarStars = RosterSystemUtils::getStarDesign(23 + rosterList[location]->getCourseName().length());
	cout << titleBarStars << "\n";
	cout << "  " << rosterList[location]->getCourseName() << " Has Been Deleted\n";
	cout << titleBarStars << "\n";

	//Shifts all rosters over to the left.
	delete rosterList[location];
	for (; location < rListSz - 1; ++location) {
		rosterList[location] = rosterList[location + 1];
	};
	rosterList[rListSz - 1] = nullptr;
	--rListSz;
}

void RosterSystem::growEnrollmentList( ) {
	int newCap = eListCap * 2 + 1;
	Student** tempList = new Student*[newCap];
	for (int i = 0; i < eListSz; ++i) {
		tempList[i] = enrollmentList[i];
	}
	for (int i = eListSz; i < eListCap; i++) {
		enrollmentList[i] = nullptr;
	}
	delete[] enrollmentList;
	eListCap = newCap;
	enrollmentList = tempList;
}

void RosterSystem::growRosterList( ) {
	int newCap = rListCap * 2 + 1;
	Roster** tempList = new Roster*[newCap];
	for (int i = 0; i < rListSz; ++i) {
		tempList[i] = rosterList[i];
	}
	for (int i = rListSz; i < rListCap; i++) {
		rosterList[i] = nullptr;
	}
	delete[] rosterList;
	rListCap = newCap;
	rosterList = tempList;
}

void RosterSystem::adminSelectOpts(Roster& selectedRoster) {
	clearScreen();
	cout << "********************************\n";
	cout << "         Editing Roster         \n";
	cout << "********************************\n";
	cout << "Course: " << selectedRoster.getCourseName() << "\n";
	cout << "Code: " << selectedRoster.getCourseCode() << "\n";
	for (int i = 0; i < numOfSelectOpts; ++i) {
		cout << static_cast<char>('A' + i) << ") " << selectOpts[i] << "\n";
	}
	string userChoice;
	cout << "Please choose an option(A-D): ";
	getline(cin, userChoice);

	switch (userChoice[0]) {
		case 'A':
		case 'a':
			{
				addToEnrollmentAndRoster(selectedRoster);
			}
			break;
		case 'B':
		case 'b':
			{
				cout << "Please enter the student's last name: ";
				string lastName;
				getline(cin, lastName);
				selectedRoster.removeStudent(lastName);
			}
			break;
		case 'C':
		case 'c':
			{
				cout << "Please enter the student's last name: ";
				string lastName;
				getline(cin, lastName);
				selectedRoster.editStudent(lastName);
			}
			break;
		case 'D':
		case 'd':
			selectedRoster.listAllStudents();
			break;
		default:
			cout << "Invalid option selected.\n";
	}
}

void RosterSystem::displayRoster(std::string courseNumber) const {
	clearScreen();
	int location = findRoster(courseNumber);
	if (location == NOT_FOUND || location == EMPTY_LIST) {
		return;
	}
	Roster* rosterToDisplay = rosterList[location];
	cout << *rosterToDisplay;
}

void RosterSystem::displayAllRosters( ) const {
	clearScreen();
	if (rListSz == 0) {
		cout << "There are no rosters in the system.\n";
		return;
	}
	cout << "********************************\n";
	cout << "     Displaying All Rosters     \n";
	cout << "********************************\n";
	for (int i = 0; i < rListSz; ++i) {
		rosterList[i]->displayInfo();
		cout << "\n";
	}
}

void RosterSystem::userSelectOpts(Roster& selectedRoster) {
	clearScreen();
	cout << "********************************\n";
	cout << "         Editing Roster         \n";
	cout << "********************************\n";
	cout << "Course: " << selectedRoster.getCourseName() << "\n";
	cout << "Code: " << selectedRoster.getCourseCode() << "\n";

	//Subtract 1 due to the fact that "user" does not have access to the last option.
	for (int i = 0; i < numOfSelectOpts - 1; ++i) {
		cout << static_cast<char>('a' + i) << ") " << selectOpts[i] << "\n";
	}
	string userChoice;
	cout << "Please choose an option(A-C): ";
	getline(cin, userChoice);
	if (userChoice.length() > 0) {
		userChoice = "";
	}
	switch (userChoice[0]) {
		case 'A':
		case 'a':
			{
				addToEnrollmentAndRoster(selectedRoster);
			}
			break;
		case 'B':
		case 'b':
			{
				cout << "Please enter the student's last name: ";
				string lastName;
				getline(cin, lastName);
				selectedRoster.removeStudent(lastName);
			}
			break;
		case 'C':
		case 'c':
			{
				cout << "Please enter the student's last name: ";
				string lastName;
				getline(cin, lastName);
				selectedRoster.editStudent(lastName);
			}
			break;
		default:
			cout << "Invalid option selected.\n";
	}
}

namespace RosterSystemUtils {
	char getMenuOptSelection(bool isAdmin) {
		string choice;
		getline(cin, choice);
		if (choice.length() > 1) {
			return '?';
		}
		if (choice[0] == 'Q' || choice[0] == 'q') {
			return choice[0];
		}
		if (choice[0] > '1' && !isAdmin) {
			return '?';
		}
		if (choice[0] == '1' && !isAdmin) {
			return '5';
		}
		return choice[0];
	}

	char getLoginOptSelection( ) {
		string choice;
		do {
			getline(cin, choice);
		} while (choice != "A" && choice != "B" && choice != "C" &&
			choice != "a" && choice != "b" && choice != "c");
		return choice[0];

	}

	string getStarDesign(int length) {
		string starDesign = "";
		for (int i = 0; i < length; ++i) {
			starDesign += "*";
		}
		return starDesign;
	}
}
