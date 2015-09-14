/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: Terminal.cpp - An application that establishes a connection via a computer's
-- serial port and then sends and receives text input.
--
-- PROGRAM: Dumb Terminal
--
-- FUNCTIONS:
-- int WINAPI WinMain (HINSTANCE hInst, HINSTANCE hprevInstance, LPSTR lspszCmdParam, int nCmdShow)
-- LRESULT CALLBACK WndProc (HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
-- DWORD WINAPI OutputThread(LPVOID n)
--
--
-- DATE: September 29, 2013
--
-- REVISIONS: None
--
-- DESIGNER: Joshua Campbell
--
-- PROGRAMMER: Joshua Campbell
--
-- NOTES:
-- The program will require the user to correctly configure the port settings.
-- In order to initiate a connection, a user will have to configure the port and then select the "connect"
-- option from the "Options" menu.
-- You cannot open more than one port at a time. You will have to select the "disconnect" first before
-- opening a different port.
----------------------------------------------------------------------------------------------------------------------*/

#define STRICT

#include <windows.h>
#include <stdio.h>
#include "resource.h"
#include <string>

TCHAR Name[] = TEXT("Comm Shell");
char str[80] = "";
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
#pragma warning (disable: 4096)

BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);

int iVertPos = 0, iHorzPos = 0;

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: WinMain
--
-- DATE: September 29, 2013
--
-- REVISIONS: none
--
-- DESIGNER: Joshua Campbell
--
-- PROGRAMMER: Joshua Campbell
--
-- RETURNS: A Windows message wParam.
--
-- NOTES:
-- The Main function for the program. Responsible for creating the Windows window for the application.
----------------------------------------------------------------------------------------------------------------------*/

int WINAPI WinMain (HINSTANCE hInst, HINSTANCE hprevInstance,
 						  LPSTR lspszCmdParam, int nCmdShow)
{
	HWND hwnd;
	MSG Msg;
	WNDCLASSEX Wcl;

	Wcl.cbSize = sizeof (WNDCLASSEX);
	Wcl.style = CS_HREDRAW | CS_VREDRAW;
	Wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION); // large icon 
	Wcl.hIconSm = NULL; // use small version of large icon
	Wcl.hCursor = LoadCursor(NULL, IDC_ARROW);  // cursor style
	
	Wcl.lpfnWndProc = WndProc;
	Wcl.hInstance = hInst;
	Wcl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH); //white background
	Wcl.lpszClassName = Name;
	
	Wcl.lpszMenuName = TEXT("MYMENU"); // The menu Class
	Wcl.cbClsExtra = 0;      // no extra memory needed
	Wcl.cbWndExtra = 0; 
	
	if (!RegisterClassEx (&Wcl))
		return 0;

	hwnd = CreateWindow (Name, Name, WS_OVERLAPPEDWINDOW, 10, 10,
   							600, 400, NULL, NULL, hInst, NULL);
	ShowWindow (hwnd, nCmdShow);
	UpdateWindow (hwnd);

	while (GetMessage (&Msg, NULL, 0, 0))
	{
   		TranslateMessage (&Msg);
		DispatchMessage (&Msg);
	}

	return Msg.wParam;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: WndProc
--
-- DATE: September 29, 2013
--
-- REVISIONS: none
--
-- DESIGNER: Joshua Campbell
--
-- PROGRAMMER: Joshua Campbell
--
-- RETURNS: False or 0
--
-- NOTES:
-- The main procedure for the application. Handles setting up COM ports, connecting, disconnecting, writing to the
-- serial port, and closing threads and connections when the program ends.
----------------------------------------------------------------------------------------------------------------------*/

LRESULT CALLBACK WndProc (HWND hwnd, UINT Message,
                          WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	static int  cxClient,cyClient;
	SCROLLINFO si;
	PAINTSTRUCT ps;
	HINSTANCE hInst;
	
	switch (Message)
	{
		case WM_COMMAND:
			switch (LOWORD (wParam))
			{
				case IDM_HOSTTOIP:
					hInst = GetModuleHandle(NULL);
					CreateDialog(hInst, MAKEINTRESOURCE(IDD_HOSTTOIP), hwnd, AboutDlgProc);
				break;
				case IDM_IPTOHOST:
				break;
				case IDM_SERVICETOPROTOCOL:
				break;
				case IDM_PROTOCOLTOSERVICE:
				break;
			}
		break;
		case WM_SIZE:
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);

			si.cbSize = sizeof(si);
			si.fMask = SIF_ALL;
			si.nMin = 0;
			si.nMax = cyClient;
			si.nPos = 0;
			si.nPage = 50;
			SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
			break;
		case WM_VSCROLL:

			si.cbSize = sizeof(si);
			si.fMask = SIF_ALL;
			GetScrollInfo(hwnd, SB_VERT, &si);
			iVertPos = si.nPos;

			switch(LOWORD(wParam)){

			case SB_LINEUP:
				si.nPos -= 10;
				break;

			case SB_LINEDOWN:
				si.nPos += 10;
				break;

			case SB_PAGEUP:
				si.nPos -= si.nPage;
				break;

			case SB_PAGEDOWN:
				si.nPos += si.nPage;
				break;

			case SB_THUMBTRACK:
				si.nPos = si.nTrackPos;
				break;

			default:
				break;
			}

			si.fMask = SIF_POS;
			SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
			GetScrollInfo(hwnd, SB_VERT, &si);

			//if there was change in the vertical scroll bar, make adjustments to redraw
			if (si.nPos != iVertPos){
			}
		break;
		case WM_CHAR:
		break;
		
		case WM_DESTROY:	// Terminate program
      		PostQuitMessage (0);
		break;

		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);
			ReleaseDC(hwnd, hdc);
		break;
		default:
			return DefWindowProc (hwnd, Message, wParam, lParam);
	}
	return 0;
}

BOOL CALLBACK AboutDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
     switch (message)
     {
     case WM_INITDIALOG:
          return TRUE;
          
     case WM_COMMAND:
          switch (LOWORD (wParam))
          {
		  case IDOK:
          case IDCANCEL:
               EndDialog (hDlg, 0);
               return TRUE;
          }
          break;
     }
     return FALSE;
}
