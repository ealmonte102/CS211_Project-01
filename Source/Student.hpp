#ifndef STUDENT_HPP
#define STUDENT_HPP

#include "Date.h"
#include "Stack.hpp"
#include <string>

class Student {
public:
	/*
	Creates a default student with first name, last name, id and standing set to "".
	  gpa and credits set to -1.0 and -1, respectively. Date and matriculation are both set to
	  the default date of Janurary 1, 1970.
	*/
	Student ( );

	Student (std::string last, std::string first, double gradeAvg, int creds, Date birthday, Date matriculationDate);

	~Student ( );
	//Accessors

	int getCredits ( ) const;

	const Date& getDob ( ) const;

	std::string getFirstName ( ) const;

	double getGpa ( ) const;

	std::string getId ( ) const;

	std::string getLastName ( ) const;

	const Date& getMatric ( ) const;

	std::string getStanding ( ) const;

	//Returns the student's full name seperated by a space.
	std::string getFullName ( ) const;

	//Mutators

	//Sets the student's credits to the specified parameter
	//and updates thier current standing.
	void setCredits (int creds);

	void setDob (const Date& birthday);

	void setFirstName (std::string first);

	void setGpa (double gradeAvg);

	void setLastName (std::string last);

	void setMatric (const Date& matric);

	//Overloaded Operators

	//Prints out the student's information in a nice format.
	friend std::ostream& operator<<(std::ostream& output, const Student& currentStudent);

	//Solicit the student's information from the user
	friend std::istream& operator>>(std::istream& input, Student& currentStudent);

	//Returns true if the student's last name, first name, AND
	//ID are equal to the other student's, otherwise returns false.
	bool operator==(const Student& otherStudent) const;

	//Returns true if the student's last name, first name, AND
	//ID are NOT equal to the other student's, otherwise returns false.
	bool operator!=(const Student& otherStudent) const;

	//Returns true if the student's last name, first name, OR
	//ID are greater than the other student's, otherwise returns false.
	bool operator>(const Student& otherStudent) const;

	//Returns true if the student's last name, first name, or
	//ID are greater than OR equal to the other student's, otherwise returns false.
	bool operator>=(const Student& otherStudent) const;

	//Returns true if the student's last name, first name, OR
	//ID are less than the other student's, otherwise returns false.
	bool operator<(const Student& otherStudent) const;

	//Returns true if the student's last name, first name, and
	//ID are less than OR equal to the other student's, otherwise returns false.
	bool operator<=(const Student& otherStudent) const;

private:
	static void generateNextID ( );

	//Sets the standing based on the student's acquired credits.
	void setStanding ( );

	static const double MAX_GPA;
	static const double MIN_GPA;
public:
	static Stack unusedID;

	std::string lastName;
	std::string firstName;
	int idAsNum;
	std::string id;
	double gpa;
	int credits;
	std::string standing;
	Date dob;
	Date matriculation;
};

#endif //STUDENT_HPP