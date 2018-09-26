// ScreenCapture.cpp : 定义应用程序的入口点。
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

// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

HWND hWnd;
HDC g_memDC;
int g_ScreenW, g_ScreenH;
RECT rect;
BOOL Iselect = FALSE;
BOOL Isdowmn = FALSE;

// 此代码模块中包含的函数的前向声明: 
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

	// TODO:  在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CAPTURESCREEN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance(hInstance, nCmdShow))
	{

		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CAPTURESCREEN));

	// 主消息循环: 
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
//  函数:  MyRegisterClass()
//
//  目的:  注册窗口类。设计自己的窗口
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
//   函数:  InitInstance(HINSTANCE, int)
//
//   目的:  保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	

	hInst = hInstance; // 将实例句柄存储在全局变量中
	//创建自己的窗口
	hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUP,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}
	//显示和更新窗口
	ShowWindow(hWnd, SW_MAXIMIZE);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	//定义一个画刷
	LOGBRUSH brush;
	brush.lbStyle = BS_NULL;
	brush.lbColor = DIB_RGB_COLORS;
	brush.lbHatch = HS_HORIZONTAL;
	HBRUSH hBrush = CreateBrushIndirect(&brush);   //创建一个画刷句柄
	//定义一个句柄
	LOGPEN pen;
	POINT pt;
	pt.x = 1;
	pt.y = 1;
	pen.lopnColor = RGB(0, 152, 152);  //画笔的颜色
	pen.lopnStyle = PS_INSIDEFRAME;        //画笔的风格
	pen.lopnWidth = pt;              //画笔的宽度
	HPEN hpen = CreatePenIndirect(&pen);           //创建一个画笔的句柄

	switch (message)
	{
		//	case WM_LBUTTONDOWN:
		//		MessageBox(hWnd, "全屏截图！", "张一西", MB_YESNOCANCEL | MB_ICONINFORMATION);
	case WM_LBUTTONDBLCLK:
		if (Iselect == TRUE)
		{
			int iNum = MessageBox(hWnd, "截图成功！", "张一西", MB_OKCANCEL | MB_ICONINFORMATION);
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
		// 分析菜单选择: 
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
		// TODO:  在此添加任意绘图代码...
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

// “关于”框的消息处理程序。
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

	if (OpenClipboard(0))                        //打开粘贴板
	{
		EmptyClipboard();                        //清空粘贴板
		SetClipboardData(CF_BITMAP, hNewBmap);   //把图片放入粘贴板
		CloseClipboard();                        //关闭粘贴板
	}
}