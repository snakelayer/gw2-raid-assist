#pragma once

#include <sstream>

#include <boost/format.hpp>

#include "gw2lib.h"

class Slubling
{
    public:

        void drawSlublingCounts(std::stringstream &ss);

    private:
        static const float SLUBLING_MAX_HP;
        static const float EVOLVED_SLUBLING_MAX_HP;

        bool isSlubling(GW2LIB::Agent &agent);
};