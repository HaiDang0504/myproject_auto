#pragma once

#ifdef VTAUTODLL_EXPORTS
#define VTAUTODLL_API __declspec(dllexport)
#else
#define VTAUTODLL_API __declspec(dllimport)
#endif

extern "C" {
    VTAUTODLL_API void InjectDll(HWND hwnd, DWORD threadId);
    VTAUTODLL_API void UnmapDll(HWND hwnd);
}
