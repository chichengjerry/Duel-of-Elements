/***********************************************
 * [3D_GAME]
 * メイン処理
 * GP11A341 24 張馳騁
 ***********************************************/

/***********************************************
 * インクルードファイル
 ***********************************************/
#include "main.h"
#include "core.h"
#include "d3d.h"

/***********************************************
 * マクロ定義
 ***********************************************/
#define APP_CLASSNAME	_T("DuelOfElements")
#define APP_TITLE		_T("Duel Of Elements")

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void GetWindowSize(DWORD* width, DWORD* height);

DWORD g_nCountFPS;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

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

	// 初期化処理(ウィンドウを作成してから行う)
	if (FAILED(D3D::Init(hInstance, hWnd)))
	{
		return -1;
	}

	//フレームカウント初期化
	timeBeginPeriod(1);				// 分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime();
	dwCurrentTime = dwFrameCount = 0;

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// メッセージループ
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
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

	// ウィンドウクラスの登録を解除
	UnregisterClass(APP_CLASSNAME, wcex.hInstance);

	// 終了処理
	D3D::Destroy();

	timeEndPeriod(1);				// 分解能を戻す

	return (int)msg.wParam;
}

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
		case VK_ESCAPE:					// [ESC]キーが押された
			DestroyWindow(hWnd);		// ウィンドウを破棄するよう指示する
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void GetWindowSize(DWORD* width, DWORD* height) {
	RECT rect = { 0, 0, CL_WIDTH, CL_HEIGHT };
	DWORD flags = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	AdjustWindowRect(&rect, flags, FALSE);

	*width = rect.right - rect.left;
	*height = rect.bottom - rect.top;
}
