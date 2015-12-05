#ifndef RSFileManager_HPP
#define RSFileManager_HPP
#include "RosterSystem.hpp"
#include <fstream>
#include <string>

class RosterSystem::RSFileManager {
public:
	RSFileManager(std::string fileName, bool useAsOutput);
	bool loginIsValid(std::string username, std::string password);
	loginFlags attemptLogin( );
	void exportRosters(const Roster* const* const rosterList, int size);
	void importRosters( ) = delete;
private:
	std::string fileName;
	std::fstream ioFile;
};

#endif //RSFileManager_HPP


