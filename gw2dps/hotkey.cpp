#ifndef HOTKEY_CPP
#define HOTKEY_CPP

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include "hotkey.h"
#include "keymap.h"

HotKey::HotKey(bool _alt, bool _shift, bool _ctrl, UINT _key)
{
    alt = _alt;
    shift = _shift;
    ctrl = _ctrl;
    key = _key;
}

HotKey::HotKey(string str)
{
    alt = false;
    shift = false;
    ctrl = false;
    key = 0;
    string alt_str = "alt";
    string shift_str = "shift";
    string ctrl_str = "ctrl";
    UINT uiKey = 0;
    UINT modifier = MOD_NOREPEAT;
    string key_hex;
    for (size_t i = 0; i < str.size(); i++)
    {
        switch (str[i])
        {
        case ' ':
            break;
        case 'a':
        case 'A':
            if (boost::iequals(str.substr(i, alt_str.size()), alt_str))
            {
                i += alt_str.size();
                alt = true;
            }
            break;
        case 's':
        case 'S':
            if (boost::iequals(str.substr(i, shift_str.size()), shift_str))
            {
                i += shift_str.size();
                shift = true;
            }
            break;
        case 'c':
        case 'C':
            if (boost::iequals(str.substr(i, ctrl_str.size()), ctrl_str))
            {
                i += ctrl_str.size();
                ctrl = true;
            }
            break;
        case '0':
            key_hex = str.substr(i, 4);
            key = strtol(key_hex.c_str(), 0, 16);
            i += 4;
            break;
        default:
            return;
        }
    }
}

string HotKey::toString()
{
    string result = "";
    if (alt)
    {
        result += "ALT + ";
    }
    if (shift)
    {
        result += "SHIFT + ";
    }
    if (ctrl)
    {
        result += "CTRL + ";
    }
    result += get_key_description(this);
    return result;
}

string HotKey::toConfigString()
{
    string result = "";
    if (alt)
    {
        result += "ALT ";
    }
    if (shift)
    {
        result += "SHIFT ";
    }
    if (ctrl)
    {
        result += "CTRL ";
    }
    stringstream stream;
    stream << hex << key;
    result += "0x" + stream.str();
    return result;
}

#endif