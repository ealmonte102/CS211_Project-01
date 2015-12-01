// RosterSystem.cpp
// Created by Evan Almonte
//
#include "RosterSystem.hpp"
#include "Roster.hpp"
#include "Student.hpp"
#include "Utilities.hpp"
#include <iostream>

using std::cout;
using std::cin;
using std::string;

int getMenuOptSelection(int start, int end);

const string RosterSystem::menuOpts[numOfMenuOpts] = {
	"Create a new Roster",
	"Drop a Roster",
	"Display Roster Information",
	"Display All Rosters",
	"Select a Roster"
};
const string RosterSystem::selectOpts[numOfSelectOpts] = {
	"Add new student",
	"Remove a Student",
	"Update a Student",
	"List All Student"
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

void RosterSystem::showMenuOptions( ) {
	int i = 4;
	int startOption = 4, endOption = 4;
	//Display all the menu options available for the supervisor.
	if (loginStatus == SUPERVISOR) {
		startOption = 0;
		for (i = 0; i < numOfMenuOpts - 1; ++i) {
			cout << i + 1 << ") " << menuOpts[i] << "\n";
		}
	}

	cout << i + 1 << ") " << menuOpts[i] << "\n";
	cout << "Please choose an option(q to Quit): ";
	int choice = getMenuOptSelection(startOption, endOption);

	switch (choice) {
		case 1:
			if (loginStatus == SUPERVISOR) {
				addRoster ( );
			} else {
				string courseNumber;
				getline (cin, courseNumber);
				selectRoster (courseNumber);
			}
			break;
		case 2:
			{
				string courseNumber;
				getline(cin, courseNumber);
				removeRoster(courseNumber);
			}
			break;
		case 3:
			/* intentionally blank*/
			break;
		case 4:
			/* intentionally blank*/
			break;
		case 5:
			{
				string courseNumber;
				selectRoster(courseNumber);
			}
			break;
		default:
			cout << "Exiting to login menu.\n";
			return;
	}
	showMenuOptions ( );
}

void RosterSystem::addToEnrollmentList(Student* aStudent) {
	if (eListSz == eListCap) {
		growEnrollmentList();
	}
	enrollmentList[eListSz++] = aStudent;
}

void RosterSystem::addToEnrollmentAndRoster(Roster& selectedRoster) {
	if (eListSz == eListCap) {
		growEnrollmentList();
	}
	Student* aStudent = new Student;
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
	if (rListSz == rListCap) {
		growRosterList();
	}
	Roster* rosterToAdd = new Roster();
	cin >> *rosterToAdd;
	rosterList[rListSz++] = rosterToAdd;
}

void RosterSystem::selectRoster(std::string courseNumber) {
	int location = findRoster(courseNumber);
	if (location == NOT_FOUND) {
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
	for (int i = 0; i < rListSz; ++i) {
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
	rosterList = tempList;
}

void RosterSystem::adminSelectOpts(Roster& selectedRoster) {
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

void RosterSystem::userSelectOpts(Roster& selectedRoster) {
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

int getMenuOptSelection(int start, int end) {
	char choiceAsChar;
	do {
		cin.get(choiceAsChar);
	} while (static_cast<int>(choiceAsChar) < start || static_cast<int>(choiceAsChar) > end && 
			 choiceAsChar	!= 'q' || choiceAsChar != 'Q' );
	int selection = choiceAsChar - '0';
	return selection;
}

