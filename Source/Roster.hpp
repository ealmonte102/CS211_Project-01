#ifndef ROSTER_HPP
#define ROSTER_HPP

#include <iosfwd>
#include <string>

class Student;

class Roster {
public:
	//Constructors
	Roster( );

	Roster(std::string course, std::string courseNumber, std::string professor, int credits, int maxCapacity);

	~Roster( );

	//Accessor Functions
	std::string getCourseCode( ) const;

	std::string getCourseName( ) const;

	std::string getInstructorName( ) const;

	int getNumOfCredits( ) const;

	//Allows a student to be passed to the roster and added if the capacity has not been reached.
	void addStudent(Student* aStudent);

	//Allows a list of students to be passed. Will add as many students as possible in sequential order, until the capacity has been reached.
	void addStudent(Student* newStudents[], int numOfStudents);

	//Removes the student located at a particular index in the roster.
	void removeStudent(std::string lastName);

	void driver( );

	void removeAll( );

	void sortUp ( ) const;

	void sortDown ( ) const;

	//Overloaded Operators
	friend std::ostream& operator<<(std::ostream& output, const Roster& currentRoster);

	friend std::istream& operator>>(std::istream& input, Roster& currentRoster);

	const Student& operator[](int index) const;

private:
	void grow( );

	int findStudent(std::string lastName) const;

	int findStudentHelper(int* foundStudents, std::string lastName) const;

	Roster(const Roster& otherRoster);

	Roster& operator=(const Roster& otherRoster);

	std::string courseName;
	std::string courseCode;
	int numEnrolled;
	std::string instructor;
	int numOfCredits;
	int capacity;
	Student** studentList;

	enum searchFlags {
		STUDENT_NOT_FOUND = -2,
		DONT_EDIT_STUDENT
	};
};
#endif //ROSTER_HPP


