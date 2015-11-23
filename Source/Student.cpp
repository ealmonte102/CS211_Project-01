// Student.cpp
// Created by Evan Almonte on 10/18/2015.
//
#include "Stack.hpp"
#include "Student.hpp"
#include "Utilities.hpp"
#include <iostream>

using std::cout;
using std::cin;
using std::istream;
using std::string;
using std::ostream;

string idToInt(int num) {
	string createdID = "000000";
	int location = createdID.length() - 1;
	while (num > 0) {
		createdID[location--] = '0' + num % 10;
		num /= 10;
	}
	return createdID;
}

const double Student::MIN_GPA = 0.0;
const double Student::MAX_GPA = 4.0;
Stack Student::unusedID(0);

void Student::generateNextID( ) {
	if (unusedID.getSize() > 1) {
		unusedID.pop();
	} else {
		unusedID.push(unusedID.pop() + 1);
	}
}

//Constructors
Student::Student( ) : lastName(""), firstName(""), idAsNum(-1),
                      id(""), gpa(-1.0), credits(-1), standing("") { }

Student::Student(std::string last, std::string first, double gpa, int credits, Date dob, Date matric)
	: lastName(last), firstName(first), idAsNum(unusedID.peek()), id(idToInt(idAsNum)), gpa(gpa),
	  credits(credits), dob(dob), matriculation(matric) {
	if (credits < 0) {
		this->credits = 0;
	}
	if (gpa < MIN_GPA || gpa > MAX_GPA) {
		this->gpa = 0;
	}
	setStanding();
	generateNextID();
}

Student::~Student( ) {
	unusedID.push(idAsNum);
}

//Accessors
int Student::getCredits( ) const {
	return credits;
}

std::string Student::getFirstName( ) const {
	return firstName;
}

const Date& Student::getDob( ) const {
	return dob;
}

std::string Student::getFullName( ) const {
	return firstName + " " + lastName;
}

double Student::getGpa( ) const {
	return gpa;
}

string Student::getId( ) const {
	return id;
}

std::string Student::getLastName( ) const {
	return lastName;
}

const Date& Student::getMatric( ) const {
	return matriculation;
}

std::string Student::getStanding( ) const {
	return standing;
}

//Mutators
void Student::setCredits(int creds) {
	if (creds < 0) {
		cout << "Negative number of credits entered. The default value of 0 will be used.\n";
		credits = 0;
	} else {
		credits = creds;
	}
	setStanding();
}

void Student::setDob(const Date& birthday) {
	dob = birthday;
}

void Student::setFirstName(std::string first) {
	firstName = first;
}

void Student::setGpa(double gradeAvg) {
	if (gradeAvg < MIN_GPA || gradeAvg > MAX_GPA) {
		cout << "Invalid GPA entered, the default value of 0 will be used.\n";
		gpa = 0;
	} else {
		gpa = gradeAvg;
	}
}

void Student::setLastName(std::string last) {
	lastName = last;
}

void Student::setMatric(const Date& matric) {
	matriculation = matric;
}

void Student::setStanding( ) {
	if (credits > 105) {
		standing = "Graduate";
	} else if (credits > 90) {
		standing = "Lower Senior";
	} else if (credits > 75) {
		standing = "Upper Junior";
	} else if (credits > 60) {
		standing = "Lower Junior";
	} else if (credits > 45) {
		standing = "Upper Sophmore";
	} else if (credits > 30) {
		standing = "Lower Sophomore";
	} else if (credits > 15) {
		standing = "Upper Freshman";
	} else {
		standing = "Lower Freshman";
	}
}

//Overloaded Operators
bool Student::operator==(const Student& otherStudent) const {
	return (lastName == otherStudent.lastName &&
		firstName == otherStudent.firstName &&
		id == otherStudent.id);
}

bool Student::operator!=(const Student& otherStudent) const {
	return !operator==(otherStudent);
}

bool Student::operator<(const Student& otherStudent) const {
	string myLast = upperConvert(lastName);
	string otherLast = upperConvert(otherStudent.lastName);
	string myFirst = upperConvert(firstName);
	string otherFirst = upperConvert(otherStudent.firstName);

	if (myLast < otherLast) {
		return true;
	}
	if (myLast > otherLast) {
		return false;
	}
	if (myFirst < otherFirst) {
		return true;
	}
	if (myFirst > otherFirst) {
		return false;
	}
	return id < otherStudent.id;
}

//"Zokus"
//"Petrou"

bool Student::operator<=(const Student& otherStudent) const {
	return !(*this > otherStudent);
}

bool Student::operator>(const Student& otherStudent) const {
	return otherStudent < *this;
}

bool Student::operator>=(const Student& otherStudent) const {
	return !(*this < otherStudent);
}

istream& operator>>(istream& input, Student& currentStudent) {
	cout << "Change first name(Y/N)? ";
	if (getYesOrNo()) {
		cout << "First name: ";
		input >> currentStudent.firstName;
		currentStudent.firstName = upperConvert(currentStudent.firstName);
	}
	cout << "Change last name(Y/N)? ";
	if (getYesOrNo()) {
		cout << "Last name: ";
		input >> currentStudent.lastName;
		currentStudent.lastName = upperConvert(currentStudent.lastName);
	}
	if (currentStudent.id == "") {
		currentStudent.idAsNum = Student::unusedID.peek();
		currentStudent.id = idToInt(currentStudent.idAsNum);
		Student::generateNextID();
	}
	cout << "Change credits earned(Y/N)? ";
	if (getYesOrNo()) {
		int creds;
		cout << "Credits Earned: ";
		input >> creds;
		currentStudent.setCredits(creds);
	}
	cout << "Change GPA(Y/N)? ";
	if (getYesOrNo()) {
		int gradeAvg;
		cout << "Current GPA: ";
		input >> gradeAvg;
		currentStudent.setGpa(gradeAvg);
	}
	cout << "Change date of birth?(Y/N)? ";
	if (getYesOrNo()) {
		cout << "Date of Birth:\n";
		cin >> currentStudent.dob;
	}
	cout << "Change matriculation date(Y/N)? ";
	if (getYesOrNo()) {
		cout << "Matriculation Date:\n";
		cin >> currentStudent.matriculation;
	}
	return input;
}

ostream& operator<<(ostream& output, const Student& currentStudent) {
	output << "First Name: " << currentStudent.firstName << "\n";
	output << "Last Name: " << currentStudent.lastName << "\n";
	output << "ID Number: " << currentStudent.id << "\n";
	output << "Gpa: " << currentStudent.gpa << "\n";
	output << "Credits Earned: " << currentStudent.credits << "\n";
	output << "Current Standing: " << currentStudent.standing << "\n";
	output << "Date of Birth: " << currentStudent.dob << "\n";
	output << "Matriculation Date: " << currentStudent.matriculation << "\n";
	return output;
}
