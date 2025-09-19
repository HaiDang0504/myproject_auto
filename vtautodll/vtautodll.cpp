#include "pch.h"
#include "vtautodll.h"

// Global variables
HMODULE hDll = NULL;

// Custom message for hook operations
#define WM_HOOKEX (WM_USER + 100)

// Hook procedure declaration
LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    // Hook procedure implementation
    if (nCode >= 0)
    {
        // Process the hook message here
        // This is where you would handle the hooked messages
    }
    
    // Call next hook in chain
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int InjectDll(HWND hWnd, DWORD threadId)
{
    if (!IsWindow(hWnd))
        return 111;

    // Get the current module handle if not already set
    if (hDll == NULL)
    {
        hDll = GetModuleHandle(NULL);
    }

    HHOOK hHook = SetWindowsHookExW(WH_CALLWNDPROC, (HOOKPROC)HookProc, hDll, threadId);
    if (hHook == NULL)
        return 222;

    SendMessageW(hWnd, WM_HOOKEX, WPARAM(hHook), 1);
    return 333;
}

int UnmapDll(HWND hWnd)
{
    if (!IsWindow(hWnd))
        return 1;
    
    // Get the current module handle if not already set
    if (hDll == NULL)
    {
        hDll = GetModuleHandle(NULL);
    }
    
    HHOOK hHook = SetWindowsHookExW(WH_CALLWNDPROC, (HOOKPROC)HookProc, hDll, GetWindowThreadProcessId(hWnd, NULL));

    if (hHook == NULL)
        return 2;
    SendMessage(hWnd, WM_HOOKEX, (WPARAM)hHook, 0);
    return 3;
}
