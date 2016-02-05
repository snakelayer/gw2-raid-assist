#pragma once

#include <map>
#include <utility>

#include <boost/circular_buffer.hpp>

#include "gw2lib.h"

#include "raid/squad_member.h"

typedef std::map<std::string, GW2LIB::Character> CharacterMap;
typedef std::pair<std::string, GW2LIB::Character> CharacterEntry;
typedef std::map<std::string, SquadMember> SquadMemberMap;
typedef std::pair<std::string, SquadMember> SquadMemberEntry;

typedef std::map<std::string, GW2LIB::Vector3> CharacterPositions;
typedef std::map<std::string, boost::circular_buffer<int> > CharacterSpeeds;
