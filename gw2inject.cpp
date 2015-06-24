#include <windows.h>

#include <tlhelp32.h> 
#include <shlwapi.h> 
#include <conio.h> 
#include <stdio.h> 

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WI32_LEAN_AND_MEAN 
#define CREATE_THREAD_ACCESS (PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ) 

BOOL Inject(DWORD pID, const char * DLL_NAME);
DWORD GetTargetThreadIDFromProcName(const char * ProcName);

using namespace std;
using boost::interprocess::shared_memory_object;
using boost::interprocess::mapped_region;

int main(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// Retrieve process ID 
	DWORD pID = GetTargetThreadIDFromProcName("Guild Wars 2");

	// Get the dll's full path name 
	wchar_t wchar_buf[MAX_PATH] = { 0 };
	GetFullPathName(L"gw2dps.dll", MAX_PATH, wchar_buf, NULL);
	char char_buf[MAX_PATH + 1] = { 0 };
	wcstombs(char_buf, wchar_buf, wcslen(wchar_buf));
	
	// write current directory in shared memory
	int cur_path_len = GetCurrentDirectory(0, NULL);
	shared_memory_object shm(boost::interprocess::open_or_create, "config_path", boost::interprocess::read_write);
	shm.truncate(cur_path_len + 1);
	mapped_region region(shm, boost::interprocess::read_write);
	wchar_t* shm_pointer = (wchar_t*) region.get_address();
	GetCurrentDirectory(cur_path_len, shm_pointer);
	shm_pointer[cur_path_len] = '\0';

	// Inject our main dll 
	if (!Inject(pID, char_buf))
	{
		printf("DLL Not Loaded! \n");
		system("Pause");
	}
	
	return nRetCode;
}

BOOL Inject(DWORD pID, const char * DLL_NAME)
{
	HANDLE Proc;
	char buf[50] = { 0 };
	LPVOID RemoteString, LoadLibAddy;

	if (!pID)
		return false;

	Proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
	if (!Proc)
	{
		printf(buf, "OpenProcess() failed: %d", GetLastError());
		printf(buf);
		return false;
	}

	LoadLibAddy = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");

	// Allocate space in the process for our DLL 
	RemoteString = (LPVOID)VirtualAllocEx(Proc, NULL, strlen(DLL_NAME), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	// Write the string name of our DLL in the memory allocated 
	WriteProcessMemory(Proc, (LPVOID)RemoteString, DLL_NAME, strlen(DLL_NAME), NULL);

	// Load our DLL 
	CreateRemoteThread(Proc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddy, (LPVOID)RemoteString, NULL, NULL);

	CloseHandle(Proc);
	return true;
}

DWORD GetTargetThreadIDFromProcName(const char * ProcName)
{
	HWND windowHandle = FindWindowA(NULL, ProcName);
	DWORD* processID = new DWORD;
	GetWindowThreadProcessId(windowHandle, processID);

	return *processID;
}
