#include "assist_drawer.h"

using namespace boost;
using namespace std;

const HWND AssistDrawer::hwnd = FindWindowEx(NULL, NULL, L"Guild Wars 2", NULL);

AssistDrawer::AssistDrawer() {
	font.Init(AssistDrawer::lineHeight, "Verdana");
}

inline wstring AssistDrawer::convert(const string& as)
{
	wstring ws = wstring(as.begin(), as.end());
	replace_all(ws, "%%", "%");
	replace_all(ws, "&", "&&");
	wstring rval = ws;

	return rval;
}

StrInfo AssistDrawer::StringInfo(string str) {
	StrInfo strInfo;

	// Line Count
	{
		size_t lineCount = 0;
		regex token("\n");
		sregex_token_iterator i(str.begin(), str.end(), token, -1);
		sregex_token_iterator j;
		while (i != j)
		{
			lineCount++;
			i++;
		}
		strInfo.lineCount = lineCount;
	}

	// Width
	{
		typedef vector<string> split_vector_type;
		split_vector_type lines; // #2: Search for tokens
		split(lines, str, is_any_of("\n"), token_compress_on);

		HDC hdc = GetDC(hwnd);
		HFONT hFont = CreateFont(lineHeight, 0, 0, 0, 600, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_TT_ALWAYS, ANTIALIASED_QUALITY,
			DEFAULT_PITCH, L"Verdana");
		HFONT hFontOld = (HFONT)SelectObject(hdc, hFont);

		// find longest line
		LONG width = 0;
		string longLine;
		for (auto & line : lines) {
			RECT r = { 0, 0, 0, 0 };
			DrawText(hdc, convert(line.c_str()).c_str(), -1, &r, DT_CALCRECT);
			LONG w = abs(r.right - r.left);

			// refine the width
			size_t i;
			i = count(line.begin(), line.end(), ':'); w -= i * 2;
			i = count(line.begin(), line.end(), ','); w -= i * 1;
			i = count(line.begin(), line.end(), ' '); w -= i * 2;
			i = count(line.begin(), line.end(), '['); w -= i * 1;
			i = count(line.begin(), line.end(), ']'); w -= i * 1;
			i = count(line.begin(), line.end(), 'T'); w -= i * 2;
			i = count(line.begin(), line.end(), 't'); w -= i * 1;
			//i = count(line.begin(), line.end(), '%'); w -= i * 6;

			i = count(line.begin(), line.end(), 'm'); w += i * 1;
			i = count(line.begin(), line.end(), 'o'); w += i * 1;

			if (w > width) {
				width = w;
				longLine = line;
			}
		}

		DeleteObject(hFont);
		ReleaseDC(hwnd, hdc);

		strInfo.x = float(width);
		strInfo.y = float(strInfo.lineCount * lineHeight);
	}

	return strInfo;
}

void AssistDrawer::drawFont(float x, float y, DWORD color, std::string format, ...) {
	va_list vl;
	va_start(vl, format);
	font.vDraw(x, y, color, format, vl);
	va_end(vl);
}
