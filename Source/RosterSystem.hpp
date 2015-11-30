#ifndef RosterSystem_HPP
#define RosterSystem_HPP

#include <string>
#include "Roster.hpp"

class RosterSystem {
public:
	RosterSystem ( );
	
	~RosterSystem ( );

	void addRoster ( );

	void removeRoster (std::string courseNumber);

	void selectRoster(std::string courseNumber);

private:
	void runAdminOptions (Roster& selectedRoster );
	void runUserOptions (Roster& selectedRoster );
	
	RosterSystem (const RosterSystem& rhs) = delete;
	
	RosterSystem& operator=(const RosterSystem& rhs) = delete;

	void grow ( );

	int findRoster (std::string courseCode) const;

	enum findFlags { NOT_FOUND = -1 };
	enum loginFlags {
		NOT_LOGGED, USER, SUPERVISOR
	} loginStatus;
	int size;
	int capacity;
	Roster** rosterList;
};

#endif //RosterSystem_HPP

