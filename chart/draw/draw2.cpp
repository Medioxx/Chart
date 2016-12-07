// draw.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "draw.h"
//#include "wszystko.h"
#include "wykres.h"
#include <cmath>
#include <ctime>
#define MAX_LOADSTRING 100
#define TMR_1 1

#define PRZYCISK_START 500
#define PRZYCISK_TPLUS 501
#define PRZYCISK_TMINUS 502
#define PRZYCISK_APLUS 503
#define PRZYCISK_AMINUS 504


#define PRZYCISK_KROK_PLUS		0xE1
#define PRZYCISK_KROK_MINUS		0xE2
#define PRZYCISK_WYBIERZ_A		0xE3
#define PRZYCISK_WYBIERZ_V		0xE4
#define PRZYCISK_WYBIERZ_S		0xE5

#define PRZYCISK_ROZSZERZ_PION	0xE6
#define PRZYCISK_ZMINIEJSZ_PION	0xE7

#define PRZYCISK_WYSWIETLAJ_A	0xF1
#define PRZYCISK_WYSWIETLAJ_V	0xF2
#define PRZYCISK_WYSWIETLAJ_S	0xF3

// Global Variables:

HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

RECT ClientRect;
HDC hdc;
_Wykres wykres;

HWND PrzyciskStart, PrzyciskKrokPlus, PrzyciskKrokMinus;
HWND PrzyciskWyswietlajA, PrzyciskWyswietlajV, PrzyciskWyswietlajS;
HWND PrzyciskPionPlus, PrzyciskPionMinus;
HWND PrzyciskWybierzA, PrzyciskWybierzV, PrzyciskWybierzS;


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


void MyOnPaint(HDC m_hDC)
{
	HDC memDC = CreateCompatibleDC(m_hDC);
	HBITMAP hBM = CreateCompatibleBitmap(m_hDC, ClientRect.right, ClientRect.bottom);
	SelectObject(memDC, hBM);
	// Now you can draw on memDC

	// Fill with white color
	HBRUSH hbr = CreateSolidBrush(RGB(255, 255, 255));
	RECT r;
	SetRect(&r, 0, 0, ClientRect.right, ClientRect.bottom);
	FillRect(memDC, &r, hbr);

	//RYSOWNANIE :
	Graphics GFX(memDC);
	wykres.rysuj(GFX, wykres.ilosc_probek);


	/*Analizator.wykres_g.rysuj(GFX);
	Analizator.wykres_czyspadek.rysuj(GFX);*/

	// Paint on window
	BitBlt(m_hDC, 0, 0, ClientRect.right, ClientRect.bottom, memDC, 0, 0, SRCCOPY);

	DeleteObject(hBM);
	DeleteDC(memDC);
}


int OnCreate(HWND window)
{
	SetTimer(window, TMR_1, 10, 0);
	wykres.wczytajDane(SCIEZKA);
	return 0;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;


	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DRAW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAW));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAW));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL; //(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DRAW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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
	srand(time(NULL));
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd;
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	OnCreate(hWnd);

	if (!hWnd)
	{
		return FALSE;
	}

	PrzyciskWyswietlajA = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Wyswietlaj A"), WS_CHILD | WS_VISIBLE, ClientRect.left + 5, ClientRect.bottom + 40, 90, 30, hWnd, (HMENU)PRZYCISK_WYSWIETLAJ_A, hInstance, NULL);
	PrzyciskWyswietlajV = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Wyswietlaj V"), WS_CHILD | WS_VISIBLE, ClientRect.left + 105, ClientRect.bottom + 40, 90, 30, hWnd, (HMENU)PRZYCISK_WYSWIETLAJ_V, hInstance, NULL);
	PrzyciskWyswietlajS = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Wyswietlaj S"), WS_CHILD | WS_VISIBLE, ClientRect.left + 205, ClientRect.bottom + 40, 90, 30, hWnd, (HMENU)PRZYCISK_WYSWIETLAJ_S, hInstance, NULL);
	PrzyciskKrokPlus = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Zwieksz krok t"), WS_CHILD | WS_VISIBLE, ClientRect.left + 305, ClientRect.bottom + 40, 90, 30, hWnd, (HMENU)PRZYCISK_KROK_PLUS, hInstance, NULL);
	PrzyciskKrokMinus = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Zmniejsz krok t"), WS_CHILD | WS_VISIBLE, ClientRect.left + 405, ClientRect.bottom + 40, 90, 30, hWnd, (HMENU)PRZYCISK_KROK_MINUS, hInstance, NULL);
	PrzyciskWybierzA = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Wybierz a"), WS_CHILD | WS_VISIBLE, ClientRect.left + 5, ClientRect.bottom + 80, 90, 30, hWnd, (HMENU)PRZYCISK_WYBIERZ_A, hInstance, NULL);
	PrzyciskWybierzV = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Wybierz v"), WS_CHILD | WS_VISIBLE, ClientRect.left + 105, ClientRect.bottom + 80, 90, 30, hWnd, (HMENU)PRZYCISK_WYBIERZ_V, hInstance, NULL);
	PrzyciskWybierzS = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Wybierz s"), WS_CHILD | WS_VISIBLE, ClientRect.left + 205, ClientRect.bottom + 80, 90, 30, hWnd, (HMENU)PRZYCISK_WYBIERZ_S, hInstance, NULL);
	PrzyciskPionPlus = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Zwieksz wysokosc"), WS_CHILD | WS_VISIBLE, ClientRect.left + 305, ClientRect.bottom + 80, 90, 30, hWnd, (HMENU)PRZYCISK_ROZSZERZ_PION, hInstance, NULL);
	PrzyciskWybierzS = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Zmniejsz wysokosc"), WS_CHILD | WS_VISIBLE, ClientRect.left + 405, ClientRect.bottom + 80, 90, 30, hWnd, (HMENU)PRZYCISK_ZMINIEJSZ_PION, hInstance, NULL);




	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;

	//OnCreate(hWnd,wParam,lParam);
	//OnTimer(hWnd,wParam,lParam);

	switch (message)
	{
	case WM_COMMAND:
		switch (wParam)
		{
		case PRZYCISK_WYSWIETLAJ_A:
			wykres.wyswietlaj_a ^= true;
			break;
		case PRZYCISK_WYSWIETLAJ_V:
			wykres.wyswietlaj_v ^= true;
			break;
		case PRZYCISK_WYSWIETLAJ_S:
			wykres.wyswietlaj_s ^= true;
			break;
		case PRZYCISK_KROK_PLUS:
			wykres.dlugosc += 20;
			break;
		case PRZYCISK_KROK_MINUS:
			wykres.dlugosc -= 20;
			if (wykres.dlugosc <= 100)
				wykres.dlugosc = 100;

			break;
		case PRZYCISK_WYBIERZ_A:
			wykres.wybierzMnoznik(MNOZNIK_A); break;
		case PRZYCISK_WYBIERZ_V:
			wykres.wybierzMnoznik(MNOZNIK_V); break;
		case PRZYCISK_WYBIERZ_S:
			wykres.wybierzMnoznik(MNOZNIK_S); break;
		case PRZYCISK_ROZSZERZ_PION:
			wykres.modyfikujMnoznik(0.1); break;
			wykres.przetwarzajDane();
		case PRZYCISK_ZMINIEJSZ_PION:
			wykres.modyfikujMnoznik(-0.1); break;
			wykres.przetwarzajDane();
		}

		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
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
		break;
	case WM_PAINT:
		GetClientRect(hWnd, &ClientRect);
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		MyOnPaint(hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case TMR_1:
			//force window to repaint
			InvalidateRect(hWnd, NULL, TRUE);
			hdc = BeginPaint(hWnd, &ps);

			//Analizator.wczytaj(); //WCZYTYWANIE KOLEJNYCH DANYCH

			MyOnPaint(hdc);
			EndPaint(hWnd, &ps);
			break;
		}
	case WM_KEYDOWN: //STEROWANIE KLAWIATURA
	{

	}

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
