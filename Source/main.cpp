//  main.cpp
//  Copyright (c) 2015 Evan Almonte. All rights reserved.
//  

#include "Date.h"
#include "Student.hpp"
#include "Roster.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

Student** createRandomList(int size);
void deleteRandomList(Student** studentList, int size);
void printStudentList(Student** studentList, int size);

using std::cout;
using std::cin;
using std::string;

int main( ) {
	srand(static_cast<unsigned int>(time(nullptr)));
	int sizeOfList = 6;
	Student** studentList = createRandomList(sizeOfList);
	Roster* testRoster = new Roster;

	testRoster->addStudent(studentList, sizeOfList);
	testRoster->driver();
	delete testRoster;

	cout << "Students still exist after deleting the roster:\n";
	printStudentList(studentList, sizeOfList);

	deleteRandomList(studentList, sizeOfList);
	testRoster = nullptr;
	studentList = nullptr;

	return 0;
}

//Generates a random list of students as large as the specified parameter.
Student** createRandomList(int size) {
	Student** studentList = new Student*[size];
	int numOfLastNames = 6;
	string lastNames[] = {"Almonte" , "Petrou", "Capaldo", "Abreu", "McSorley", "Toribio"};

	int numOfFirstNames = 6;
	string firstNames[] = {"Evan", "Andrew", "Joshua", "Max", "Andy", "Alex"};

	int numOfGpas = 6;
	double gpas[] = {3.5, 2.98, 4.0, 2.6, 3.75, 3.9};

	int numOfBirthdays = 5;
	Date birthdays[] = {Date(6, 16, 1995), Date(3, 23, 1992), Date(8, 23, 1998), Date(5, 15, 1990), Date(3, 5, 1995)};

	for (int i = 0; i < size; ++i) {
		string randLast = lastNames[rand() % numOfLastNames];
		string randFirst = firstNames[rand() % numOfFirstNames];
		double randGpa = gpas[rand() % numOfGpas];
		int randCreds = rand() % 117;
		Date randDob = birthdays[rand() % numOfBirthdays];


		studentList[i] = new Student(randLast, randFirst, randGpa, randCreds, randDob, Date());
	}
	return studentList;
}

//Deletes a list of students.
void deleteRandomList(Student** studentList, int size) {
	for (int i = 0; i < size; ++i) {
		delete studentList[i];
	}
	delete[] studentList;
}

//Prints out a list of students.
void printStudentList(Student** studentList, int size) {
	for (int i = 0; i < size; ++i) {
		cout << *studentList[i] << "\n";
	}
}
