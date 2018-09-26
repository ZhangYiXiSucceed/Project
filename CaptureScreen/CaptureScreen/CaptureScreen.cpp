// ScreenCapture.cpp : ����Ӧ�ó������ڵ㡣
//
#include "easyx.h"
#include "graphics.h"
#include "stdafx.h"
#include "CaptureScreen.h"
#include "windows.h"
#include<Mmsystem.h>
#include<time.h>
#include<iostream>
using namespace std;
#pragma comment(lib,"winmm.lib")

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];            // ����������

HWND hWnd;
HDC g_memDC;
int g_ScreenW, g_ScreenH;
RECT rect;
BOOL Iselect = FALSE;
BOOL Isdowmn = FALSE;

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void                ScreenDisplay();
void                CopyToCliboard();
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

	//PlaySound("yixi.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	mciSendString("open ./abc.mp3 alias bk",
		0, 0, 0);
	mciSendString("play bk repeat", 0, 0, 0);


	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO:  �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CAPTURESCREEN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance(hInstance, nCmdShow))
	{

		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CAPTURESCREEN));

	// ����Ϣѭ��: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  ����:  MyRegisterClass()
//
//  Ŀ��:  ע�ᴰ���ࡣ����Լ��Ĵ���
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_CAPTURESCREEN));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����:  InitInstance(HINSTANCE, int)
//
//   Ŀ��:  ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	

	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����
	//�����Լ��Ĵ���
	hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUP,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}
	//��ʾ�͸��´���
	ShowWindow(hWnd, SW_MAXIMIZE);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ����:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	//����һ����ˢ
	LOGBRUSH brush;
	brush.lbStyle = BS_NULL;
	brush.lbColor = DIB_RGB_COLORS;
	brush.lbHatch = HS_HORIZONTAL;
	HBRUSH hBrush = CreateBrushIndirect(&brush);   //����һ����ˢ���
	//����һ�����
	LOGPEN pen;
	POINT pt;
	pt.x = 1;
	pt.y = 1;
	pen.lopnColor = RGB(0, 152, 152);  //���ʵ���ɫ
	pen.lopnStyle = PS_INSIDEFRAME;        //���ʵķ��
	pen.lopnWidth = pt;              //���ʵĿ��
	HPEN hpen = CreatePenIndirect(&pen);           //����һ�����ʵľ��

	switch (message)
	{
		//	case WM_LBUTTONDOWN:
		//		MessageBox(hWnd, "ȫ����ͼ��", "��һ��", MB_YESNOCANCEL | MB_ICONINFORMATION);
	case WM_LBUTTONDBLCLK:
		if (Iselect == TRUE)
		{
			int iNum = MessageBox(hWnd, "��ͼ�ɹ���", "��һ��", MB_OKCANCEL | MB_ICONINFORMATION);
			if (iNum == 1)
			{
				CopyToCliboard();
				Iselect = FALSE;
				PostQuitMessage(0);
			}
			else
			{
				Iselect = FALSE;
			}
		}
		break;
	case WM_LBUTTONDOWN:
	{
						   if (!Iselect)
						   {
							   POINT pt;
							   GetCursorPos(&pt);
							   rect.left = pt.x;
							   rect.top = pt.y;
							   rect.right = pt.x;
							   rect.bottom = pt.x;
							   InvalidateRgn(hWnd, 0, FALSE);
							   Isdowmn = TRUE;
						   }


	}
		break;
	case WM_LBUTTONUP:
	{
						 if (Isdowmn == TRUE&&!Iselect)
						 {
							 POINT pt;
							 GetCursorPos(&pt);
							 rect.right = pt.x;
							 rect.bottom = pt.y;
							 InvalidateRgn(hWnd, 0, FALSE);
							 Isdowmn = FALSE;
							 Iselect = TRUE;
						 }
	}
		break;
	case WM_MOUSEMOVE:
	{
						 if (Isdowmn == TRUE&&!Iselect)
						 {
							 POINT pt;
							 GetCursorPos(&pt);
							 rect.right = pt.x;
							 rect.bottom = pt.y;
							 InvalidateRgn(hWnd, 0, FALSE);
						 }
	}
		break;
	case WM_CREATE:
		ScreenDisplay();
		break;
	case WM_KEYDOWN:
	{
					   switch (wParam)
					   {
					   case VK_LEFT:
					   {
									   SendMessage(hWnd, WM_CLOSE, 0, 0);
					   }
						   break;
					   }
	}
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��: 
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
		hdc = BeginPaint(hWnd, &ps);
		// TODO:  �ڴ���������ͼ����...
		SelectObject(hdc, hpen);
		SelectObject(hdc, hBrush);
		BitBlt(hdc, 0, 0, g_ScreenW, g_ScreenH, g_memDC, 0, 0, SRCCOPY);
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
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


void ScreenDisplay()
{
	HDC disDc = ::CreateDC("DISPLAY", NULL, NULL, NULL);
	g_memDC = ::CreateCompatibleDC(disDc);
	g_ScreenW = GetDeviceCaps(disDc, HORZRES);
	g_ScreenH = GetDeviceCaps(disDc, VERTRES);
	//cout << "g_ScreenW=" << g_ScreenW << "g_ScreenH=" << g_ScreenH<< endl;
	HBITMAP hbitmap = CreateCompatibleBitmap(disDc, g_ScreenW, g_ScreenH);
	SelectObject(g_memDC, hbitmap);

	//DEVMODE curDevMode;
	//memset(&curDevMode, 0, sizeof(curDevMode));
	//curDevMode.dmSize = sizeof(DEVMODE);
	//BOOL bEnumRet = ::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &curDevMode);
	//if (bEnumRet && g_ScreenW < curDevMode.dmPelsWidth)
	//{
	//	//bProcessed = TRUE;
	//	::SetStretchBltMode(g_memDC, STRETCH_HALFTONE);
	//	::StretchBlt(g_memDC, 0, 0, g_ScreenW, g_ScreenH, disDc, 0, 0, curDevMode.dmPelsWidth,
	//		curDevMode.dmPelsHeight, SRCCOPY);
	//}
	////else
	BitBlt(g_memDC, 0, 0, g_ScreenW, g_ScreenH, disDc, 0, 0, SRCCOPY);
}


void CopyToCliboard()
{
	HDC hScreenDC = ::CreateDC("DISPLAY", 0, 0, 0);
	HDC memDC = ::CreateCompatibleDC(hScreenDC);
	int Width = rect.right - rect.left-2;
	int	Height = rect.bottom - rect.top-2;
	HBITMAP hBmap = CreateCompatibleBitmap(hScreenDC, Width, Height);
	HBITMAP hOldBmap = (HBITMAP)SelectObject(memDC, hBmap);


	//DEVMODE curDevMode;
	//memset(&curDevMode, 0, sizeof(curDevMode));
	//curDevMode.dmSize = sizeof(DEVMODE);
	//BOOL bEnumRet = ::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &curDevMode);
	//if (bEnumRet && Width < curDevMode.dmPelsWidth)
	//{
	//	//bProcessed = TRUE;
	//	::SetStretchBltMode(memDC, STRETCH_HALFTONE);
	//	::StretchBlt(memDC, 0, 0, Width, Height, hScreenDC, rect.left, rect.top, curDevMode.dmPelsWidth,
	//		curDevMode.dmPelsHeight, SRCCOPY);
	//}
	//else
	BitBlt(memDC, 0, 0, Width, Height, hScreenDC, rect.left+1, rect.top+1, SRCCOPY);


	HBITMAP hNewBmap = (HBITMAP)SelectObject(memDC, hOldBmap);

	if (OpenClipboard(0))                        //��ճ����
	{
		EmptyClipboard();                        //���ճ����
		SetClipboardData(CF_BITMAP, hNewBmap);   //��ͼƬ����ճ����
		CloseClipboard();                        //�ر�ճ����
	}
}