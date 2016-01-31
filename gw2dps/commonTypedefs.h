#pragma once

#include <map>
#include <utility>

#include <boost/circular_buffer.hpp>

#include "gw2lib.h"

#include "raid/squad_member.h"

typedef map<string, GW2LIB::Character> CharacterMap;
typedef pair<string, GW2LIB::Character> CharacterEntry;
typedef map<string, SquadMember> SquadMemberMap;
typedef pair<string, SquadMember> SquadMemberEntry;

typedef map<string, GW2LIB::Vector3> CharacterPositions;
typedef map<string, boost::circular_buffer<int> > CharacterSpeeds;
