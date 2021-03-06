//-----------------------------------------------
// [main.cpp]
// メイン処理
// 作者 Chicheng Zhang
//-----------------------------------------------
#include "main.h"
#include "core.h"
#include "d3d.h"

#define APP_CLASSNAME	_T("DuelOfElements")
#define APP_TITLE		_T("Duel Of Elements")

//
// グローバル変数
//
DWORD g_nCountFPS;

//
// アプリケーションの入り口。
//
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		APP_CLASSNAME,
		NULL
	};
	HWND hWnd;
	MSG msg;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウスタイル設定
	DWORD width, height;
	DWORD flags = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;
	GetWindowSize(&width, &height);

	// ウィンドウの作成
	hWnd = CreateWindow(APP_CLASSNAME,
		APP_TITLE,
		flags,
		(GetSystemMetrics(SM_CXSCREEN) - width) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - height) / 2,
		width,
		height,
		NULL,
		NULL,
		hInstance,
		NULL);

	srand((unsigned)time(NULL));

	// Direct3Dの初期化
	if (FAILED(D3D::Init(hInstance, hWnd)))
	{
		return -1;
	}

	//フレームカウント初期化
	timeBeginPeriod(1);
	dwExecLastTime = dwFPSLastTime = timeGetTime();
	dwCurrentTime = dwFrameCount = 0;

	// ウインドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// メッセージループ
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// メッセージ処理を行う
			if (msg.message == WM_QUIT)
			{
				//ループ終了
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{	// DirectXの処理を行う
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5秒ごとに実行
			{
#ifdef _DEBUG
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				// 更新処理
				D3D::Update();

				// 描画処理
				D3D::Render();

				dwFrameCount++;
			}
		}
	}

	// 終了処理
	UnregisterClass(APP_CLASSNAME, wcex.hInstance);
	D3D::Destroy();
	timeEndPeriod(1);

	return (int)msg.wParam;
}

//
// メッセージプロセス関数。
// 実際のキー入力はDINPUTで行う。
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:	
			DestroyWindow(hWnd);
			break;
		}
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//
// ウィンドウスタイルにより実際のサイズを計算する。
//
void GetWindowSize(DWORD* width, DWORD* height) {
	RECT rect = { 0, 0, CL_WIDTH, CL_HEIGHT };
	DWORD flags = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	AdjustWindowRect(&rect, flags, FALSE);

	*width = rect.right - rect.left;
	*height = rect.bottom - rect.top;
}
