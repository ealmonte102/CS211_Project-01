// RosterSystem.cpp
// Created by Evan Almonte
//
#include "RosterSystem.hpp"
#include "Roster.hpp"
#include <iostream>

using std::cout;
using std::cin;

RosterSystem::RosterSystem( ) : loginStatus(NOT_LOGGED), size(0),
                                capacity(0), rosterList(new Roster*[capacity]) {
	rosterList = nullptr;
}

RosterSystem::~RosterSystem( ) {
	for (int i = 0; i < size; ) {
		delete rosterList[i];
	}
	delete[] rosterList;
}

void RosterSystem::grow( ) {
	int newCap = capacity * 2 + 1;
	Roster** tempList = new Roster*[newCap];
	for (int i = 0; i < size; ++i) {
		tempList[i] = rosterList[i];
	}
	delete[] rosterList;
	rosterList = tempList;
}

int RosterSystem::findRoster(std::string courseCode) const{
	int foundIndex = NOT_FOUND;
	for (int i = 0; i < size; ++i) {
		if (courseCode == rosterList[i]->getCourseCode()) {
			foundIndex = i;
		}
	}
	return foundIndex;
}

void RosterSystem::addRoster( ) {
	if(size == capacity) { grow(); }
	Roster* rosterToAdd = new Roster ( );
	cin >> *rosterToAdd;
	rosterList[size++] = rosterToAdd;
}

void RosterSystem::removeRoster(std::string courseCode) {
	int location = findRoster (courseCode);
	if(location == NOT_FOUND) {
		cout << "The course with a code of: " << courseCode << " was not found.\n";
		return;
	}
	delete rosterList[size - 1];
	for (; location < size - 1; ++location) {
		rosterList[location] = rosterList[location + 1];
	};
	rosterList[location] = nullptr;
	--size; 
}
