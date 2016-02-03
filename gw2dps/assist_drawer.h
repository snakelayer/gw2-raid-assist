#pragma once

#include <windows.h>

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

#include <algorithm> // count()

#include "gw2lib.h"

using namespace boost;
using namespace std;

struct StrInfo {
	StrInfo() : lineCount(0), x(0), y(0) {}

	size_t lineCount;
	float x;
	float y;
};

class AssistDrawer
{
	private:
		const static HWND hwnd;

		GW2LIB::Font font;

		AssistDrawer();
		AssistDrawer(AssistDrawer const&) = delete;
		AssistDrawer& operator=(AssistDrawer const&) = delete;

		static inline wstring convert(const string& as);

	public:
		static const int lineHeight = 16;

		static AssistDrawer& get()
		{
			static AssistDrawer instance;
			return instance;
		}

		static StrInfo StringInfo(string str);

		void drawFont(float x, float y, DWORD color, std::string format, ...);
};