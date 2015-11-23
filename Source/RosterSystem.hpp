#ifndef RosterSystem_HPP
#define RosterSystem_HPP

class Roster;
class RosterSystem {
public:
	RosterSystem ( );
	
	~RosterSystem ( );

private:
	RosterSystem (const RosterSystem& rhs);
	
	RosterSystem& operator=(const RosterSystem& rhs);

	enum loginFlag {
		NOT_LOGGED, USER, SUPERVISOR
	} loginStatus;
	int size;
	Roster* rosterList;
};

#endif //RosterSystem_HPP

