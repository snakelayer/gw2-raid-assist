#pragma once

#include <sstream>
#include <windows.h>

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

#include <algorithm> // count()

#include "gw2lib.h"

struct StrInfo {
	StrInfo() : lineCount(0), x(0), y(0) {}

	size_t lineCount;
	float x;
	float y;
};

class AssistDrawer
{
	private:
		static const HWND hwnd;

		static const int PADX = 5;
		static const int PADY = 2;
		static const DWORD BACKGROUND_COLOR = 0xdd000000;
		static const DWORD BORDER_COLOR = 0xff444444;
		static const DWORD BLACK = 0xffffffff;

		GW2LIB::Font font;

		AssistDrawer();
		AssistDrawer(AssistDrawer const&) = delete;
		AssistDrawer& operator=(AssistDrawer const&) = delete;

		static inline std::wstring convert(const std::string& as);

	public:
		static const int lineHeight = 16;

		static AssistDrawer& get()
		{
			static AssistDrawer instance;
			return instance;
		}

		static StrInfo StringInfo(std::string str);

		void drawFont(float x, float y, DWORD color, std::string format, ...);
		void drawStreamToWindow(std::stringstream &ss, float x, float y);
};