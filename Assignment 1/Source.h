#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include <WinSock2.h>
#include <Windows.h>
#include "resource.h"


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK GetHost(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hprevInstance, LPSTR lspzCmdParam, int nCmdShow);
INT_PTR CALLBACK GetIP(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK GetPort(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK GetService(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

HWND hwnd;