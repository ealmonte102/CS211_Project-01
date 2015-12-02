#ifndef RosterSystem_HPP
#define RosterSystem_HPP

#include <string>
#include "Roster.hpp"

class RosterSystem {
public:
	RosterSystem ( );
	
	~RosterSystem ( );

	void loginMenu ( );

	void mainMenu ( );
private:
	static void displayAdminMenu ( );

	static void displayUserMenu ( );

	void addToEnrollmentAndRoster (Roster& selectedRoster);
	
	void addToEnrollmentList (Student* aStudent);

	void addRoster ( );

	//Function which allows supervisor to use the passed Roster. Functions provided listed in selectOps array.
	void adminSelectOpts (Roster& selectedRoster);

	void displayRoster (std::string courseNumber) const;

	void displayAllRosters () const;

	int findRoster (std::string courseCode) const;

	void growRosterList ( );

	void growEnrollmentList ( );
	
	void removeRoster (std::string courseNumber);

	void selectRoster (std::string courseNumber);

	void userSelectOpts (Roster& selectedRoster);	
	
	RosterSystem (const RosterSystem& rhs) = delete;
	
	RosterSystem& operator=(const RosterSystem& rhs) = delete;

	enum findFlags { NOT_FOUND = -1 };
	enum loginFlags {
		NOT_LOGGED, USER, SUPERVISOR
	} loginStatus;

	static const int numOfMenuOpts = 5;
	static const std::string menuOpts[numOfMenuOpts];

	static const int numOfSelectOpts = 4;
	static const std::string selectOpts[numOfSelectOpts];
	
	int rListSz;
	int rListCap;
	Roster** rosterList;
	
	int eListSz;
	int eListCap;
	Student** enrollmentList;
};

#endif //RosterSystem_HPP

