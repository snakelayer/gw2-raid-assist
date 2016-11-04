#pragma once

#include <map>
#include <utility>

#include <boost/circular_buffer.hpp>

#include "gw2lib.h"

#include "raid/squad_member.h"

typedef std::map<uint32_t, GW2LIB::Character> CharacterMap;
typedef std::pair<uint32_t, GW2LIB::Character> CharacterEntry;
typedef std::map<uint32_t, SquadMember> SquadMemberMap;
typedef std::pair<uint32_t, SquadMember> SquadMemberEntry;
