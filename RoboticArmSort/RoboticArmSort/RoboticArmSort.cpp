// RoboticArmSort.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "RoboticArmSort.h"
#include "drawing.h"
#include "painting.h"
#include "animation.h"
#include "blocks.h"
#include "misc.h"
#include "arm.h"

#define MAX_LOADSTRING 100

const REAL SCALE = 20.0f, ARM_LEN_1 = 300.0f, ARM_LEN_2 = 200.0f,
	ARM_OFFSET = 100.0f, ARM_MAGNET_OFFSET = 50.0f, MIN_BLOCK_HEIGHT = 50.0f,
	MAX_BLOCK_HEIGHT = 150.0f, BLOCK_WIDTH = 30.0f, GRAB_ERROR = 10.0f;
const double ANIMATE_INCR = 0.02;

RectF window;
double velocity = 1.0;
bool autoMode = false;
std::pair<double, double> angle = { M_PI / 4.0, M_PI / 4.0 }, target, move;
std::vector<Block> blocks;
Arm arm(300.0f, 200.0f, 1.5f * SCALE, Color::DarkGray, Color::LightGray);

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hwndButton, hwndSlider;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ROBOTICARMSORT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ROBOTICARMSORT));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

	GdiplusShutdown(gdiplusToken);

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ROBOTICARMSORT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ROBOTICARMSORT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	// BUTTONS HERE

	hwndButton = CreateWindow(WC_BUTTON, L"\u23FA",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		10 * SCALE, 8 * SCALE, (8 * SCALE) / 4, 2 * SCALE, hWnd,
		(HMENU)ID_REC, hInstance, NULL);

	hwndButton = CreateWindow(WC_BUTTON, L"\u25B6\u23F8",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10 * SCALE + (8 * SCALE) / 3, 8 * SCALE, (8 * SCALE) / 4, 2 * SCALE, hWnd,
		(HMENU)ID_PLAYPAUSE, hInstance, NULL);

	hwndButton = CreateWindow(WC_BUTTON, L"\u25A0",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10 * SCALE + 2 * (8 * SCALE) / 3, 8 * SCALE, (8 * SCALE) / 4, 2 * SCALE, hWnd,
		(HMENU)ID_STOP, hInstance, NULL);

	hwndButton = CreateWindow(WC_BUTTON, L"Automatyczny",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		10 * SCALE, 2 * SCALE, 8 * SCALE, SCALE, hWnd,
		(HMENU)ID_AUTO, hInstance, NULL);

	hwndButton = CreateWindow(WC_BUTTON, L"Manualny",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		10 * SCALE, 3 * SCALE, 8 * SCALE, SCALE, hWnd,
		(HMENU)ID_MAN, hInstance, NULL);

	// SLIDER BELOW

	hwndSlider = CreateWindowEx(0, TRACKBAR_CLASS, TEXT(""),
		TBS_AUTOTICKS | WS_CHILD | WS_VISIBLE | WS_TABSTOP |
		TBS_HORZ, 10 * SCALE, 5 * SCALE, 8 * SCALE, SCALE, hWnd,
		(HMENU)ID_SLIDER, NULL, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	blocks = RandBlocks();

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			OnPaint(hdc);
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_KEYDOWN:
		{
		bool timerWasRunning = move.first || move.second;
		if (!autoMode && DecodeKeys(move, wParam) && !timerWasRunning)
			SetTimer(hWnd, TMR, (UINT)(1000.0 * ANIMATE_INCR / velocity), NULL);
		else if (wParam == VK_SPACE && !arm.IsMounted())
			for (std::vector<Block>::iterator it = blocks.begin(); it != blocks.end(); it++)
				arm.AttemptGrab(*it);
		else if (wParam == VK_SPACE)
			for (std::vector<Block>::iterator it = blocks.begin(); it != blocks.end(); it++)
				arm.AttemptLayDown(*it);
		}
		break;
	case WM_KEYUP:
		move.first = (wParam == VK_LEFT || wParam == VK_RIGHT) ? 0.0 : move.first;
		move.second = (wParam == VK_UP || wParam == VK_DOWN) ? 0.0 : move.second;
		if (!(move.first || move.second))
			KillTimer(hWnd, TMR);
		break;
	case WM_TIMER:
		arm.Increment(move.first, move.second);
		/*
		if (InConflict(blocks))
		{
			angle = { angle.first - move.first, angle.second - move.second };
			KillTimer(hWnd, TMR);
		}
		*/
		RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);
		break;
	case WM_SIZE:
		{
		RECT temp;
		GetClientRect(hWnd, &temp);
		window.X = 0.0f;
		//window.Y = (REAL)(GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(
			//SM_CYCAPTION) + GetSystemMetrics(SM_CXPADDEDBORDER));
		window.Y = 0.0f;
		window.Width = (REAL)temp.right;
		window.Height = (REAL)temp.bottom - window.Y;
		}
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
