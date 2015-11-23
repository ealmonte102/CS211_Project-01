// RosterSystem.cpp
// Created by Evan Almonte
//
#include "RosterSystem.hpp"
#include "Roster.hpp"


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
