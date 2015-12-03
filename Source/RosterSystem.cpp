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
	int getMenuOptSelection (int start, int end);
	char getLoginOptSelection ( );
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
                                eListSz(0), eListCap(0), enrollmentList(nullptr) {}

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
	clearScreen ( );
	loginStatus = USER;
	cout << "********************************\n";
	cout << "  Welcome To SchoolManager v.1  \n";
	cout << "********************************\n";
	cout << "A) Supervisor Mode\n";
	cout << "B) User Mode\n";
	cout << "C) Exit\n";
	cout << "Please choose an option: ";
	char choice = RosterSystemUtils::getLoginOptSelection ( );
	switch(choice) {
		case 'A':
		case 'a': {
			RosterSystem::RSFileManager userDatabase ("Database.txt", false);
			loginStatus = userDatabase.attemptLogin ( );
		}
		case 'B':
		case 'b':
			mainMenu ( );
			loginMenu ( );
			break;
		case 'C':
		case 'c':
			break;
		default:
			cout << "Invalid choice please try again.\n";
			cout << "Press ENTER to continue . . .";
			cin.get ( );
			loginMenu ( );
	}
}

void RosterSystem::mainMenu( ) {
	clearScreen ( );
	cout << "******************************\n";
	cout << "          Main Menu           \n";
	cout << "******************************\n";
	int startOption = 1, endOption;
	if (loginStatus == SUPERVISOR) {
		endOption = numOfMenuOpts;
		displayAdminMenu();
	} else {
		endOption = 1;
		displayUserMenu();
	}
	cout << "Please choose an option(q to Quit): ";
	int choice = RosterSystemUtils::getMenuOptSelection(startOption, endOption);

	if (choice == 1 && loginStatus != SUPERVISOR) {
		choice = 5;
	}

	switch (choice) {
		case 1:
			addRoster();
			break;
		case 2:
			{
				string courseNumber;
				cout << "Please enter the rosters course number: ";
				getline(cin, courseNumber);
				removeRoster(courseNumber);
			}
			break;
		case 3:
			{
				cout << "Please enter the rosters course number: ";
				string courseNumber;
				getline(cin, courseNumber);
				displayRoster(courseNumber);
			}
			break;
		case 4:
			displayAllRosters();
			break;
		case 5:
			{
				string courseNumber;
				cout << "Please enter the rosters course number: ";
				getline(cin, courseNumber);
				selectRoster(courseNumber);
			}
			break;
		default:
			cout << "Exiting to login menu.\n";
			return;
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
	int i = numOfMenuOpts - 1;
	cout << i + 1 << ") " << menuOpts[numOfMenuOpts - 1] << "\n";
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
	Student* aStudent = new Student;
	cout << "******************************\n";
	cout << "      New Student Added       \n";
	cout << "******************************\n";
	cin >> *aStudent;
	enrollmentList[eListSz++] = aStudent;
	selectedRoster.addStudent(aStudent);
}

int RosterSystem::findRoster(std::string courseCode) const {
	int foundIndex = NOT_FOUND;
	for (int i = 0; i < rListSz; ++i) {
		if (courseCode == rosterList[i]->getCourseCode()) {
			foundIndex = i;
		}
	}
	return foundIndex;
}

void RosterSystem::addRoster( ) {
	clearScreen();
	if (rListSz == rListCap) {
		growRosterList();
	}
	Roster* rosterToAdd = new Roster();
	cout << "******************************\n";
	cout << "      New Roster Created      \n";
	cout << "******************************\n";
	cin >> *rosterToAdd;
	rosterList[rListSz++] = rosterToAdd;
}

void RosterSystem::selectRoster(std::string courseNumber) {
	int location = findRoster(courseNumber);
	if (location == NOT_FOUND) {
		cout << "No roster with a course number of: " << courseNumber << " was found.\n";
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
	if (location == NOT_FOUND) {
		cout << "The course with a code of: " << courseCode << " was not found.\n";
		return;
	}
	delete rosterList[rListSz - 1];
	for (; location < rListSz - 1; ++location) {
		rosterList[location] = rosterList[location + 1];
	};
	rosterList[location] = nullptr;
	--rListSz;
}

void RosterSystem::growEnrollmentList( ) {
	int newCap = eListCap * 2 + 1;
	Student** tempList = new Student*[newCap];
	for (int i = 0; i < eListSz; ++i) {
		tempList[i] = enrollmentList[i];
	}
	delete[] enrollmentList;
	enrollmentList = tempList;
}

void RosterSystem::growRosterList( ) {
	int newCap = rListCap * 2 + 1;
	Roster** tempList = new Roster*[newCap];
	for (int i = 0; i < rListSz; ++i) {
		tempList[i] = rosterList[i];
	}
	delete[] rosterList;
	rListCap = newCap;
	rosterList = tempList;
}

void RosterSystem::adminSelectOpts(Roster& selectedRoster) {
	clearScreen();
	cout << "******************************\n";
	cout << "      Editing Roster          \n";
	cout << "******************************\n";
	cout << "Course: " << selectedRoster.getCourseName() << "\n";
	cout << "Code: " << selectedRoster.getCourseCode() << "\n";
	for (int i = 0; i < numOfSelectOpts; ++i) {
		cout << static_cast<char>('a' + i) << ") " << selectOpts[i] << "\n";
	}
	string userChoice;
	cout << "Please choose an option(a-d): ";
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
	int location = findRoster(courseNumber);
	if (location == NOT_FOUND) {
		cout << "No roster with a course number of: " << courseNumber << " was found.\n";
		return;
	}
	Roster* rosterToDisplay = rosterList[location];
	rosterToDisplay->displayInfo();
}

void RosterSystem::displayAllRosters( ) const {
	for (int i = 0; i < rListSz; ++i) {
		rosterList[i]->displayInfo();
	}
}

void RosterSystem::userSelectOpts(Roster& selectedRoster) {
	clearScreen();
	cout << "******************************\n";
	cout << "      Editing Roster          \n";
	cout << "******************************\n";
	cout << "Course: " << selectedRoster.getCourseName() << "\n";
	cout << "Code: " << selectedRoster.getCourseCode() << "\n";

	//Subtract 1 due to the fact that "user" does not have access to the last option.
	for (int i = 0; i < numOfSelectOpts - 1; ++i) {
		cout << static_cast<char>('a' + i) << ") " << selectOpts[i] << "\n";
	}
	string userChoice;
	cout << "Please choose an option(a-c): ";
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
		default:
			cout << "Invalid option selected.\n";
	}
}

namespace RosterSystemUtils {
	int getMenuOptSelection (int start, int end) {
		string choice;
		do {
			getline (cin, choice);
		} while (choice[0] - '0' < start || choice[0] - '0' > end &&
				 choice != "q" && choice != "Q");
		int selection = choice[0] - '0';
		return selection;
	}

	char getLoginOptSelection ( ) {
		string choice;
		do {
			getline (cin, choice);
		} while (choice != "A" && choice != "B" && choice != "C" &&
				 choice != "a" && choice != "b" && choice != "c");
		return choice[0];

	}
}