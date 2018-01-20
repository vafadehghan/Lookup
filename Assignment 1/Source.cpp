/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE:		Source.cpp - The main entry point of the program that houses WinMain and WndProc.
--
-- PROGRAM:			Lookup
--
-- FUNCTIONS:		int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hprevInstance, LPSTR lspzCmdParam, int nCmdShow)
--					LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
--					INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
--
--
-- DATE:			January 17,2018
--
--
-- DESIGNER:		Vafa Dehghan Saei
--
-- PROGRAMMER:		Vafa Dehghan Saei
--
-- NOTES:			This file is the entry point of DummyTerminal the creates the window that stores the terminal.
--					Once a key is setup is completed then users can communicate with another computer through the serial port.
----------------------------------------------------------------------------------------------------------------------*/
#include "Source.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	WinMain
--
-- DATE:		January 17,2018
--
--
-- DESIGNER:	Vafa Dehghan Saei
--
-- PROGRAMMER:	Vafa Dehghan Saei
--
-- INTERFACE:	int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hprevInstance, LPSTR lspzCmdParam, int nCmdShow)
--					HINSTANCE hInst: A handle to the current instance of the application
--					HINSTANCE hprevInstance: A handle to the previous instance of the application.
--					LPSTR lspzCmdParam: The command line for the application.
--					int nCmdShow: Controls how the windows should be shown.
--
--
-- RETURNS:		If the function succeeds, and terminated successfully with WM_QUIT, it returns the exit value contained in WM_QUITs wParam. If terminated before the message loop, return a 0
--
-- NOTES:		This function creates the window and is the application entry point.
--
----------------------------------------------------------------------------------------------------------------------*/
int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hprevInstance, LPSTR lspzCmdParam, int nCmdShow) {

	MSG Msg;
	WNDCLASSEX Wcl;

	// Define a Window class
	Wcl.cbSize = sizeof(WNDCLASSEX);
	Wcl.style = 0; // default style
	Wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION); // large icon 
	Wcl.hIconSm = NULL; // use small version of large icon
	Wcl.hCursor = LoadCursor(NULL, IDC_ARROW);  // cursor style

	Wcl.lpfnWndProc = WndProc; // window function
	Wcl.hInstance = hInst; // handle to this instance
	Wcl.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); //white background
	Wcl.lpszClassName = "Lookup"; // window class name

	Wcl.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1); // no class menu 
	Wcl.cbClsExtra = 0;      // no extra memory needed
	Wcl.cbWndExtra = 0;

	// Register the class
	if (!RegisterClassEx(&Wcl))
		return 0;

	hwnd = CreateWindow(
		"Lookup", // name of window class
		"Lookup", // title 
		WS_OVERLAPPEDWINDOW, // window style - normal
		CW_USEDEFAULT,	// X coord
		CW_USEDEFAULT, // Y coord
		CW_USEDEFAULT, // width
		CW_USEDEFAULT, // height
		NULL, // no parent window
		NULL, // no menu
		hInst, // instance handle
		NULL // no additional arguments
	);
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	WSAStartup(wVersionRequested, &wsaData);

	// Display the window
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	// Create the message loop
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg); // translate keybpard messages
		DispatchMessage(&Msg); // dispatch message and return control to windows
	}

	return Msg.wParam;
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	WndProc
--
-- DATE:		January 17,2018
--
--
-- DESIGNER:	Vafa Dehghan Saei
--
-- PROGRAMMER:	Vafa Dehghan Saei
--
-- INTERFACE:	LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
--					HWND hwnd: A handle to the window.
--					UINT Message: The message.
--					WPARAM wParam: Additional message information.
--					LPARAM lParam: Additional message information.
--
--
-- RETURNS:		The result of the message processing which depends on the message sent.
--
-- NOTES:		This function processes the messages sent to a window, and takes appropriate actions which is defined by the user.
--
----------------------------------------------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {

	switch (Message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILE_HOSTNAME:
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, GetHost);
			break;
		case ID_FILE_IP:
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG2), hwnd, GetIP);
			break;
		case ID_FILE_PORTNUMBER:
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, GetPort);
			break;
		case ID_FILE_SERVICE:
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG4), hwnd, GetService);
			break;
		}
		break;
	case WM_CHAR:
		break;

	case WM_DESTROY:		// message to terminate the program
		PostQuitMessage(0);
		break;

	default: // Let Win32 process all other messages
		return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK GetHost(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	struct	hostent *hp;
	HDC hdc;
	hdc = GetDC(hwnd);
	struct in_addr addr;


	switch (message) {
	case WM_INITDIALOG:
		SendMessage(hDlg,
			DM_SETDEFID,
			(WPARAM)IDCANCEL,
			(LPARAM)0);
		return TRUE;

	case WM_COMMAND:
		if (HIWORD(wParam) == EN_CHANGE && LOWORD(wParam) == IDC_EDIT1) {
			SendMessage(hDlg,
				DM_SETDEFID,
				(WPARAM)IDOK,
				(LPARAM)0);
		}

		switch (wParam) {
		case IDOK:
		{
			int len = SendDlgItemMessage(hDlg, IDC_EDIT1, WM_GETTEXTLENGTH, 0, 0);
			char* buffer = new char[len];
			SendDlgItemMessage(hDlg, IDC_EDIT1, WM_GETTEXT, (WPARAM)len + 1, (LPARAM)buffer);
			if ((hp = gethostbyname(buffer)) == NULL) {
				switch (h_errno)
				{
				case HOST_NOT_FOUND:
					MessageBox(hwnd, "No Such Host", "", MB_OK);
					exit(1);
				case TRY_AGAIN:
					MessageBox(hwnd, "Try Again Later", "", MB_OK);
					exit(1);
				case NO_RECOVERY:
					MessageBox(hwnd, "DNS Error", "", MB_OK);
					exit(1);
				case NO_ADDRESS:
					MessageBox(hwnd, "No Address for this Host", "", MB_OK);
					exit(1);
				default:
					MessageBox(hwnd, "Unknown Error", "", MB_OK);
					exit(1);
				}
			}
			/*addr.s_addr = *(u_long *)hp->h_addr_list[0];
			TextOut(hdc, 0, 40, inet_ntoa(addr), strlen(inet_ntoa(addr)));*/

			EndDialog(hDlg, LOWORD(wParam));
			char** p;
			int i = 0;
			TextOut(hdc, 0, 0, buffer, len);
			for (p = hp->h_addr_list; *p != 0; p++) {
				addr.s_addr = *(u_long *)*p;
				TextOut(hdc, 0, i += 15, inet_ntoa(addr), strlen(inet_ntoa(addr)));
			}

			break;
		}
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			break;
		default:
			break;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK GetIP(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	struct hostent *hp;
	struct in_addr addr, *addr_p;
	addr_p = (struct in_addr*)malloc(sizeof(struct in_addr));
	addr_p = &addr;
	HDC hdc;
	hdc = GetDC(hwnd);

	switch (message)
	{
	case WM_INITDIALOG:
		SendMessage(hDlg, DM_SETDEFID, (WPARAM)IDCANCEL, (LPARAM)0);
		return TRUE;

	case WM_COMMAND:
		if (HIWORD(wParam) == EN_CHANGE && LOWORD(wParam) == IDC_EDIT1) {
			SendMessage(hDlg, DM_SETDEFID, (WPARAM)IDOK, (LPARAM)0);
		}
		switch (wParam)
		{
		case IDOK:
		{
			int len = SendDlgItemMessage(hDlg, IDC_EDIT1, WM_GETTEXTLENGTH, 0, 0);
			char* buffer = new char[len];
			SendDlgItemMessage(hDlg, IDC_EDIT1, WM_GETTEXT, (WPARAM)len + 1, (LPARAM)buffer);
			addr_p->s_addr = inet_addr(buffer);

			hp = gethostbyaddr((char *)addr_p, AF_INET, len);
			if (hp == NULL)
			{
				int error = WSAGetLastError();
				switch (h_errno)
				{
				case HOST_NOT_FOUND:
					MessageBox(hwnd, "No Such Host", "", MB_OK);
					exit(1);
				case TRY_AGAIN:
					MessageBox(hwnd, "Try Again Later", "", MB_OK);
					exit(1);
				case NO_RECOVERY:
					MessageBox(hwnd, "DNS Error", "", MB_OK);
					exit(1);
				case NO_ADDRESS:
					MessageBox(hwnd, "No Host for this IP", "", MB_OK);
					exit(1);
				default:
					MessageBox(hwnd, "Unknown Error", "", MB_OK);
					exit(1);
				}
			}

			EndDialog(hDlg, LOWORD(wParam));
			char **p;
			int i = 0;
			TextOut(hdc, 0, 0, buffer, len);
			for (p = hp->h_addr_list; *p != 0; p++) {
				TextOut(hdc, 0, i = i + 15, hp->h_name, strlen(hp->h_name));
			}
			break;
		}
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return (INT_PTR)FALSE;


}

INT_PTR CALLBACK GetPort(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	struct servent *sv;
	HDC hdc;
	hdc = GetDC(hwnd);

	switch (message)
	{
	case WM_INITDIALOG:
		SendMessage(hDlg, DM_SETDEFID, (WPARAM)IDCANCEL, (LPARAM)0);
		return TRUE;

	case WM_COMMAND:
		if (HIWORD(wParam) == EN_CHANGE && LOWORD(wParam) == IDC_EDIT1) {
			SendMessage(hDlg, DM_SETDEFID, (WPARAM)IDOK, (LPARAM)0);
		}
		switch (wParam)
		{
		case IDOK:
		{
			int len = SendDlgItemMessage(hDlg, IDC_EDIT1, WM_GETTEXTLENGTH, 0, 0);
			int len2 = SendDlgItemMessage(hDlg, IDC_EDIT3, WM_GETTEXTLENGTH, 0, 0);
			char* portNum = new char[len];
			char* ProtocolName = new char[len2];
			SendDlgItemMessage(hDlg, IDC_EDIT1, WM_GETTEXT, (WPARAM)len + 1, (LPARAM)portNum);
			SendDlgItemMessage(hDlg, IDC_EDIT3, WM_GETTEXT, (WPARAM)len2 + 1, (LPARAM)ProtocolName);
			sv = getservbyport(htons(atoi(portNum)), ProtocolName);
			if (sv == NULL) {
				int n = WSAGetLastError();
				MessageBox(hwnd, "Error", "", MB_OK);
				exit(1);
			}

			TextOut(hdc, 0, 0, portNum, strlen(portNum));
			TextOut(hdc, 0, 15, ProtocolName, strlen(ProtocolName));
			TextOut(hdc, 0, 30, sv->s_name, strlen(sv->s_name));
			break;
		}
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK GetService(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	struct servent *sv;
	HDC hdc;
	hdc = GetDC(hwnd);

	switch (message)
	{
	case WM_INITDIALOG:
		SendMessage(hDlg, DM_SETDEFID, (WPARAM)IDCANCEL, (LPARAM)0);
		return TRUE;

	case WM_COMMAND:
		if (HIWORD(wParam) == EN_CHANGE && LOWORD(wParam) == IDC_EDIT1) {
			SendMessage(hDlg, DM_SETDEFID, (WPARAM)IDOK, (LPARAM)0);
		}
		switch (wParam)
		{
		case IDOK:
		{
			int len = SendDlgItemMessage(hDlg, IDC_EDIT1, WM_GETTEXTLENGTH, 0, 0);
			int len2 = SendDlgItemMessage(hDlg, IDC_EDIT3, WM_GETTEXTLENGTH, 0, 0);
			char* serviceName = new char[len];
			char* ProtocolName = new char[len2];
			SendDlgItemMessage(hDlg, IDC_EDIT1, WM_GETTEXT, (WPARAM)len + 1, (LPARAM)serviceName);
			SendDlgItemMessage(hDlg, IDC_EDIT3, WM_GETTEXT, (WPARAM)len2 + 1, (LPARAM)ProtocolName);
			sv = getservbyname(serviceName, ProtocolName);
			if (sv == NULL) {
				int n = WSAGetLastError();
				MessageBox(hwnd, "Error", "", MB_OK);
				exit(1);
			}
			char* buffer = new char[32];
			_itoa_s(ntohs(sv->s_port), buffer, 32, 10);
			TextOut(hdc, 0, 0, serviceName, strlen(serviceName));
			TextOut(hdc, 0, 15, ProtocolName, strlen(ProtocolName));
			TextOut(hdc, 0, 30, buffer, strlen(buffer));
			EndDialog(hDlg, LOWORD(wParam));
			break;
		}
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return (INT_PTR)FALSE;
}