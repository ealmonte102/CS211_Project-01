// RSFileManager.cpp
// Created by Evan Almonte
//
#include "RSFileManager.hpp"
#include "Student.hpp"
#include "Utilities.hpp"
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::cin;

namespace RSFileManagerUtils{
	void parseRosterLine (string aRosterLine, Roster& aRoster);
	void parseStudentLine (string aStudentLine, Student& aStudent);
	string trim (string aString);
}

RosterSystem::RSFileManager::RSFileManager(std::string fileName, bool useAsOutput) : fileName(fileName) {
	if (useAsOutput) {
		ioFile.open(fileName.c_str(), std::ios::out);
	} else {
		ioFile.open(fileName.c_str(), std::ios::in);
	}
}

bool RosterSystem::RSFileManager::loginIsValid(std::string username, std::string password) {
	if (ioFile.is_open()) {
		string uN, pN;
		while (!ioFile.eof()) {
			ioFile >> uN >> pN;
			if (upperConvert(username) == upperConvert(uN) && password == pN) {
				return true;
			}
		}
	}
	return false;
}

RosterSystem::loginFlags RosterSystem::RSFileManager::attemptLogin( ) {
	string username;
	string password;
	int numOfTries = 3;
	while (numOfTries-- > 0) {
		cout << "Please enter your username: ";
		getline(cin, username);
		cout << "Please enter your password: ";
		getline(cin, password);
		if (!ioFile.is_open()) {
			ioFile.open(fileName.c_str());
		}
		if (loginIsValid(username, password)) {
			return SUPERVISOR;
		}
		ioFile.close();
		cout << "Invalid login. Please try again " << numOfTries << " tries left.\n" ;
	}
	return USER;
}

void RosterSystem::RSFileManager::exportRosters(const Roster* const* const rosterList, int size) {
	if (ioFile.is_open()) {
		for (int i = 0; i < size; ++i) {
			const Roster* currentRoster = rosterList[i];
			ioFile << rosterList[i]->getCourseName() << " | ";
			ioFile << rosterList[i]->getCourseCode() << " | ";
			ioFile << rosterList[i]->getNumOfCredits() << " | ";
			ioFile << rosterList[i]->getInstructorName() << "\n";
			int numOfStudents = rosterList[i]->getNumEnrolled();
			for (int j = 0; j < numOfStudents; ++j) {
				const Student currentStudent = (*currentRoster)[j];
				ioFile << currentStudent.getFirstName() << " | ";
				ioFile << currentStudent.getLastName() << " |";
				ioFile << currentStudent.getId() << "|";
				ioFile << currentStudent.getStanding() << "|";
				ioFile << currentStudent.getCredits() << "|";
				ioFile << currentStudent.getGpa() << "|";
				ioFile << currentStudent.getDob().getDay() << "/";
				ioFile << currentStudent.getDob().getMonthNum() << "/";
				ioFile << currentStudent.getDob().getYear() << "|";
				ioFile << currentStudent.getDob().getDay() << "/";
				ioFile << currentStudent.getDob().getMonthNum() << "/";
				ioFile << currentStudent.getDob().getYear() << "\n";
			}
			ioFile << "end_roster|";
			if (i != size - 1) { ioFile << "\n"; }
		}
	}
}

namespace RSFileManagerUtils {
	void parseRosterLine (string aRosterLine, Roster& aRoster) {			
		string courseName, courseCode, profName;
		int creds;
		int posOfLine, posOfLine2;

		posOfLine = aRosterLine.find ('|');
		courseName = trim (aRosterLine.substr (0, posOfLine - 1));

		posOfLine2 = aRosterLine.find ('|', posOfLine + 1);
		courseCode = trim (aRosterLine.substr (posOfLine + 1, posOfLine2 - posOfLine - 1));

		posOfLine = posOfLine2;

		posOfLine2 = aRosterLine.find ('|', posOfLine + 1);
		creds = stoi (trim (aRosterLine.substr (posOfLine + 1, posOfLine2 - posOfLine - 1)));

		posOfLine = posOfLine2;

		profName = trim (aRosterLine.substr (posOfLine + 1, aRosterLine.length ( ) - posOfLine - 1));
		
		aRoster.setCourseName (courseName);
		aRoster.setCourseCode (courseCode);
		aRoster.setInstructorName (profName);
		aRoster.setCredits (creds);
	}

	void parseStudentLine (string aStudentLine, Student& aStudent) {
		string first, last;
		int credits, id;
		double gpa;
		Date dob, mat;
		int month, day, year;
		unsigned long int posOfLine, posOfLine2; //4,294,967,295 numbers

		posOfLine = aStudentLine.find ('|');
		first = trim (aStudentLine.substr (0, posOfLine - 1));

		posOfLine2 = aStudentLine.find ('|', posOfLine + 1);
		last = trim (aStudentLine.substr (posOfLine + 1, posOfLine2 - posOfLine - 1));

		posOfLine = posOfLine2;

		posOfLine2 = aStudentLine.find ('|', posOfLine + 1);
		id = stoi (trim (aStudentLine.substr (posOfLine + 1, posOfLine2 - posOfLine - 1)));

		posOfLine = aStudentLine.find ('|', posOfLine2 + 1);
		posOfLine2 = aStudentLine.find ('|', posOfLine + 1);

		credits = stoi (trim (aStudentLine.substr (posOfLine + 1, posOfLine2 - posOfLine - 1)));
		posOfLine = posOfLine2;

		posOfLine2 = aStudentLine.find ('|', posOfLine + 1);
		gpa = stod (trim (aStudentLine.substr (posOfLine + 1, posOfLine2 - posOfLine - 1)));

		posOfLine = posOfLine2;

		posOfLine2 = aStudentLine.find ('/', posOfLine + 1);
		month = stoi (trim (aStudentLine.substr (posOfLine + 1, posOfLine2 - posOfLine - 1)));

		posOfLine = posOfLine2;

		posOfLine2 = aStudentLine.find ('/', posOfLine + 1);
		day = stoi (trim (aStudentLine.substr (posOfLine + 1, posOfLine2 - posOfLine - 1)));

		posOfLine = posOfLine2;

		posOfLine2 = aStudentLine.find ('|', posOfLine + 1);
		year = stoi (trim (aStudentLine.substr (posOfLine + 1, posOfLine2 - posOfLine - 1)));

		dob = Date (month, day, year);

		posOfLine = posOfLine2;

		posOfLine2 = aStudentLine.find ('/', posOfLine + 1);
		month = stoi (trim (aStudentLine.substr (posOfLine + 1, posOfLine2 - posOfLine - 1)));

		posOfLine = posOfLine2;

		posOfLine2 = aStudentLine.find ('/', posOfLine + 1);
		day = stoi (trim (aStudentLine.substr (posOfLine + 1, posOfLine2 - posOfLine - 1)));

		posOfLine = posOfLine2;

		year = stoi (trim (aStudentLine.substr (posOfLine + 1, aStudentLine.length ( ) - posOfLine - 1)));

		mat = Date (month, day, year);

		aStudent.setFirstName (first);
		aStudent.setLastName (last);
		aStudent.setId (id);
		aStudent.setCredits (credits);
		aStudent.setGpa (gpa);
		aStudent.setDob (mat);
		aStudent.setMatric (dob);
	}
	
	string trim (string aString) {
		int firstNonWhite = aString.find_first_not_of (" ");
		int lastNonWhite;
		if (firstNonWhite != -1) {
			aString.erase (0, firstNonWhite);
		}
		lastNonWhite = aString.find_last_not_of (" ");
		if (lastNonWhite != -1) {
			aString.erase (lastNonWhite + 1);
		}
		return aString;
	}
}
