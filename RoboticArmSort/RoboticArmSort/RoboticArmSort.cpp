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
#include "sorter.h"
#include "recorder.h"

#define MAX_LOADSTRING 100

const REAL SCALE = 25.0f, ARM_OFFSET = 150.0f, MIN_BLOCK_HEIGHT = 50.0f,
	MAX_BLOCK_HEIGHT = 150.0f, BLOCK_WIDTH = 30.0f, GRAB_ERROR = 20.0f, 
	BLOCKS_OFFSET = 100.0f;
const UINT TIMER_DELAY = 10;

RectF window, armBounds;
RECT armBoundsAlt;
double velocity = 1.0;

std::vector<Block> blocks;
std::queue<AnimationActionCont> actionsQueue;
Arm arm(300.0f, 300.0f, 30.0f, Color::DarkGray, Color::LightGray);
Animation animation(TMR, velocity);
Sorter sorter;
Recorder recorder;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hwndSlider;
std::vector<HWND> hwndButtons;

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

	hwndButtons.resize(5);
	// Basic controls
	hwndButtons[0] = CreateWindow(WC_BUTTON, L"Sortuj",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		(UINT)SCALE, (UINT)(2.0f * SCALE), (UINT)(3.0f * SCALE),
		(UINT)SCALE, hWnd, (HMENU)ID_SORT, hInstance, NULL);
	hwndButtons[1] = CreateWindow(WC_BUTTON, L"Reset",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		(UINT)(4.0f * SCALE), (UINT)(2.0f * SCALE), (UINT)(3.0f * SCALE),
		(UINT)SCALE, hWnd, (HMENU)ID_RESET, hInstance, NULL);
	// Velocity control slider
	hwndSlider = CreateWindowEx(0, TRACKBAR_CLASS, TEXT(""),
		TBS_AUTOTICKS | WS_CHILD | WS_VISIBLE | WS_TABSTOP |
		TBS_HORZ, (UINT)SCALE, (UINT)(4.0f * SCALE), (UINT)(6.0f * SCALE),
		(UINT)SCALE, hWnd, (HMENU)ID_SLIDER, NULL, NULL);
	// Recording controls
	hwndButtons[2] = CreateWindow(WC_BUTTON, L"\u23FA",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		(UINT)SCALE, (UINT)(7.0f * SCALE), (UINT)(2.0f * SCALE),
		(UINT)SCALE, hWnd, (HMENU)ID_REC, hInstance, NULL);
	hwndButtons[3] = CreateWindow(WC_BUTTON, L"\u25B6",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		(UINT)(3.0f * SCALE), (UINT)(7.0f * SCALE), (UINT)(2.0f * SCALE),
		(UINT)SCALE, hWnd, (HMENU)ID_START, hInstance, NULL);
	hwndButtons[4] = CreateWindow(WC_BUTTON, L"\u25A0",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		(UINT)(5.0f * SCALE), (UINT)(7.0f * SCALE), (UINT)(2.0f * SCALE),
		(UINT)SCALE, hWnd, (HMENU)ID_STOPP, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	SendMessage(hwndSlider, TBM_SETRANGE, TRUE, MAKELONG(0, 15));
	SendMessage(hwndSlider, TBM_SETPOS, TRUE, 5);
	armBounds = GetBoundingRect(arm);
	armBoundsAlt = ConvertRect(armBounds);
	RandBlocks(blocks);

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
			case ID_SORT:
				sorter.InitSort(hWnd, actionsQueue, animation, blocks, arm);
				EnableWindow(hwndButtons[2], FALSE);
				EnableWindow(hwndButtons[3], FALSE);
				EnableWindow(hwndButtons[4], FALSE);
				SetFocus(hWnd);
				EnableWindow(hwndSlider, FALSE);
				break;
			case ID_RESET:
				sorter.KillSort(actionsQueue);
				RandBlocks(blocks);
				SetFocus(hWnd);
				RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);
				break;
			case ID_REC:
				recorder.StartRecording(blocks, arm);
				EnableWindow(hwndButtons[3], FALSE);
				SetFocus(hWnd);
				break;
			case ID_START:
				recorder.StartPlayback(hWnd, animation, actionsQueue, blocks);
				EnableWindow(hwndButtons[2], FALSE);
				SetFocus(hWnd);
				break;
			case ID_STOPP:
				if (recorder.IsPlaying())
					recorder.StopPlayback(actionsQueue);
				else recorder.StopRecording();
				EnableWindow(hwndButtons[2], TRUE);
				EnableWindow(hwndButtons[3], TRUE);
				SetFocus(hWnd);
				break;
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
		bool armWasMounted = arm.IsMounted();
		if (wParam == VK_SPACE && !arm.IsMounted())
			AttemptGrab(arm, blocks);
		else if (wParam == VK_SPACE)
			AttemptLayDown(arm, blocks);
		else
			animation.OnKeydown(hWnd, wParam, arm);
		recorder.OnKeypush(wParam, arm, armWasMounted);
		}
		break;
	case WM_KEYUP:
		if (wParam != VK_SPACE)
			recorder.OnKeypush(wParam, arm, false);
		animation.OnKeyup(hWnd, wParam);
		break;
	case WM_HSCROLL:
		velocity = (double)(5 + SendMessage(hwndSlider, TBM_GETPOS, 0, 0)) / 10.0;
		animation.SetMaxSpeed(velocity);
		animation.UpdateSpeed(arm);
		RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);
		SetFocus(hWnd);
		break;
	case WM_TIMER:
		sorter.OnTick(actionsQueue, arm);
		animation.OnTick(hWnd, arm, blocks, actionsQueue, recorder.IsPlaying()
			&& !actionsQueue.empty() && actionsQueue.front().action < VerticalGrab);
		RedrawWindow(hWnd, &armBoundsAlt, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);
		break;
	case WM_SIZE:
		window = GetClientRectAlt(hWnd);
		armBounds = GetBoundingRect(arm);
		armBoundsAlt = ConvertRect(armBounds);
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
