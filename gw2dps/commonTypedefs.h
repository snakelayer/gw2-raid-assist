#pragma once

#include <map>
#include <utility>

#include "raid/squad_member.h"

class SquadMember;

typedef map<string, GW2LIB::Character> CharacterMap;
typedef pair<string, GW2LIB::Character> CharacterEntry;
typedef map<string, SquadMember> SquadMemberMap;
typedef pair<string, SquadMember> SquadMemberEntry;
