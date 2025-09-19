#pragma once
#pragma comment(lib, "user32.lib")

#include <unordered_map>
#include <Windows.h>
#include "GameWnd.h"
#include "Logger.h"
#include <vector>

const UINT WM_HOOK_WRITE = RegisterWindowMessage(L"WM_HOOK_WRITE");
const UINT WH_VTAUTO = RegisterWindowMessage(L"WM_VTAUTO");

class GameWnd;

class GameWndManager
{
public:
	std::unordered_map<DWORD, GameWnd*> gameWnds;
	VTAutoLogger* logger;

	GameWndManager(VTAutoLogger* logger);
	void RefreshGameData();
	GameWnd* getGameWnd(DWORD pid);
	GameWnd* getGameWnd(System::String^ pid);
	void notifyDeath();
};