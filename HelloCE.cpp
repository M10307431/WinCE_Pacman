//======================================================================
// HelloCE - A simple application for Windows CE
//
// Written for the book Programming Windows CE
// Copyright (C) 2007 Douglas Boling
//======================================================================
#include <windows.h>                 // For all that Windows stuff
#include "helloce.h"                 // Program-specific stuff
#include "resource1.h"

#define xbd	600
#define ybd 400
//----------------------------------------------------------------------
// Global data
//
const TCHAR szAppName[] = TEXT("HelloCE");
HINSTANCE hInst;                     // Program instance handle
int x=70;
int y=364;
int g1x=300;
int g1y=202;
int g2x=22;
int g2y=122;
int g3x=534;
int g3y=280;
RECT rectm,rectg1,rectg2,rectg3;
int master=IDB_MRO;
int g1 = IDB_GHOST1;
int g2 = IDB_G2;
int g3 = IDB_GHOST1;
bool delpt[10] = {false};
HANDLE gThread1;
HANDLE gThread2;
HANDLE gThread3;
bool status=false;
int score =0;
int sc[11] = {IDB_SCZERO,IDB_SC1,IDB_SC2,IDB_SC3,IDB_SC4,IDB_SC5,
				IDB_SC6,IDB_SC7,IDB_SC8,IDB_SC9,IDB_SC10};

// Message dispatch table for MainWindowProc
const struct decodeUINT MainMessages[] = {
    WM_CREATE, DoCreateMain,
	WM_PAINT, DoPaintMain,
	WM_KEYDOWN, DoKeysMain,
    WM_DESTROY, DoDestroyMain,
};

//======================================================================
// Program entry point
//
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPWSTR lpCmdLine, int nCmdShow) {
    MSG msg;
    int rc = 0;
    HWND hwndMain;

    // Initialize this instance.
    hwndMain = InitInstance (hInstance, lpCmdLine, nCmdShow);
    if (hwndMain == 0) return 0x10;

    // Application message loop
    while (GetMessage (&msg, NULL, 0, 0)) {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }
    // Instance cleanup
    return TermInstance (hInstance, msg.wParam);
}
//----------------------------------------------------------------------
// InitInstance - Instance initialization
//
HWND InitInstance (HINSTANCE hInstance, LPWSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc;
	HWND hWnd;

    // Save program instance handle in global variable.
    hInst = hInstance;

#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
    // If Windows Mobile, only allow one instance of the application
    hWnd = FindWindow (szAppName, NULL);
    if (hWnd) {
        SetForegroundWindow ((HWND)(((DWORD)hWnd) | 0x01));    
        return 0;
    }
#endif

    // Register application main window class.
    wc.style = 0;                             // Window style
    wc.lpfnWndProc = MainWndProc;             // Callback function
    wc.cbClsExtra = 0;                        // Extra class data
    wc.cbWndExtra = 0;                        // Extra window data
    wc.hInstance = hInstance;                 // Owner handle
    wc.hIcon = NULL,                          // Application icon
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);// Default cursor
    wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
    wc.lpszMenuName = NULL;                   // Menu name
    wc.lpszClassName = szAppName;             // Window class name

    if (RegisterClass (&wc) == 0) return 0;

    // Create main window.
    hWnd = CreateWindow (szAppName,           // Window class
                         TEXT("HelloCE"),     // Window title
                         // Style flags
                         WS_VISIBLE | WS_CAPTION | WS_SYSMENU,
                         CW_USEDEFAULT,       // x position
                         CW_USEDEFAULT,       // y position
                         CW_USEDEFAULT,       // Initial width
                         CW_USEDEFAULT,       // Initial height
                         NULL,                // Parent
                         NULL,                // Menu, must be null
                         hInstance,           // Application instance
                         NULL);               // Pointer to create
                                              // parameters
    if (!IsWindow (hWnd)) return 0;  // Fail code if not created.
	// Create thread
	gThread1 = CreateThread (NULL, 0, ghost1, hWnd, 0, NULL);
	gThread2 = CreateThread (NULL, 0, ghost2, hWnd, 0, NULL);
	gThread3 = CreateThread (NULL, 0, ghost3, hWnd, 0, NULL);
    /*if (gThread1)
        CloseHandle (gThread1);
    else {
        DestroyWindow (hWnd);
        return 0;
    }*/
    // Standard show and update calls
    ShowWindow (hWnd, nCmdShow);
    UpdateWindow (hWnd);
    return hWnd;
}
//----------------------------------------------------------------------
// TermInstance - Program cleanup
//
int TermInstance (HINSTANCE hInstance, int nDefRC) {
    return nDefRC;
}
//======================================================================
// Message handling procedures for main window
//
//----------------------------------------------------------------------
// MainWndProc - Callback function for application window
//
LRESULT CALLBACK MainWndProc (HWND hWnd, UINT wMsg, WPARAM wParam, 
                              LPARAM lParam) {
    INT i;
    //
    // Search message list to see if we need to handle this
    // message.  If in list, call procedure.
    //
    for (i = 0; i < dim(MainMessages); i++) {
        if (wMsg == MainMessages[i].Code)
            return (*MainMessages[i].Fxn)(hWnd, wMsg, wParam, lParam);
    }
    return DefWindowProc (hWnd, wMsg, wParam, lParam);
}
//----------------------------------------------------------------------
// DoCreateMain - Process WM_CREATE message for window.
//
LRESULT DoCreateMain (HWND hWnd, UINT wMsg, WPARAM wParam, 
                      LPARAM lParam) {
	LPCREATESTRUCT lpcs = (LPCREATESTRUCT) lParam;
	#if defined(WIN32_PLATFORM_PSPC)
    memset (&sai, 0, sizeof (sai));
    sai.cbSize = sizeof (sai);
    {
    SHMENUBARINFO mbi;                      // For WinMobile, create
    memset(&mbi, 0, sizeof(SHMENUBARINFO)); // menu bar so that we
    mbi.cbSize = sizeof(SHMENUBARINFO);     // have a sip button.
    mbi.hwndParent = hWnd;
    mbi.dwFlags = SHCMBF_EMPTYBAR;
    SHCreateMenuBar(&mbi);
    SetWindowPos (hWnd, 0, 0, 0, lpcs->cx, lpcs->cy-26, 
                  SWP_NOZORDER | SWP_NOMOVE);
    }
#endif
	return 0;
}
//----------------------------------------------------------------------
// DoPaintMain - Process WM_PAINT message for window.
//
LRESULT DoPaintMain (HWND hWnd, UINT wMsg, WPARAM wParam, 
                     LPARAM lParam) {
    PAINTSTRUCT ps;
    RECT rect;
    HDC hMemDc,hdc;
	HBITMAP hBitmap;
	TCHAR szHello[] = TEXT ("Hello Windows CE");
    // Get the size of the client rectangle
    GetClientRect (hWnd, &rect);
    hdc = BeginPaint (hWnd, &ps); 
	hBitmap=LoadBitmap( hInst, MAKEINTRESOURCE(IDB_MAP)); 
	hMemDc=CreateCompatibleDC(hdc);
	SelectObject(hMemDc,hBitmap);
	BitBlt(hdc, 0,0,rect.right,rect.bottom,hMemDc,0,0,SRCCOPY);
	hBitmap=LoadBitmap( hInst, MAKEINTRESOURCE(g1)); 
	SelectObject(hMemDc,hBitmap);
	BitBlt(hdc, g1x,g1y,rect.right,rect.bottom,hMemDc,0,0,SRCCOPY);
	hBitmap=LoadBitmap( hInst, MAKEINTRESOURCE(g2)); 
	SelectObject(hMemDc,hBitmap);
	BitBlt(hdc, g2x,g2y,rect.right,rect.bottom,hMemDc,0,0,SRCCOPY);
	hBitmap=LoadBitmap( hInst, MAKEINTRESOURCE(master)); 
	SelectObject(hMemDc,hBitmap);
	BitBlt(hdc, x,y,rect.right,rect.bottom,hMemDc,0,0,SRCCOPY);
	hBitmap=LoadBitmap( hInst, MAKEINTRESOURCE(sc[score])); 
	SelectObject(hMemDc,hBitmap);
	BitBlt(hdc, 4,2,rect.right,rect.bottom,hMemDc,0,0,SRCCOPY);
	hBitmap=LoadBitmap( hInst, MAKEINTRESOURCE(g3)); 
	SelectObject(hMemDc,hBitmap);
	BitBlt(hdc, g3x,g3y,rect.right,rect.bottom,hMemDc,0,0,SRCCOPY);
	if(!delpt[0]){
		hBitmap=LoadBitmap( hInst, MAKEINTRESOURCE(IDB_PT0)); 
		SelectObject(hMemDc,hBitmap);
		BitBlt(hdc, 42,336,rect.right,rect.bottom,hMemDc,0,0,SRCCOPY);
	}
	if(!delpt[1]){
		hBitmap=LoadBitmap( hInst, MAKEINTRESOURCE(IDB_PT0)); 
		SelectObject(hMemDc,hBitmap);
		BitBlt(hdc, 482,70,rect.right,rect.bottom,hMemDc,0,0,SRCCOPY);
	}
	if(!delpt[2]){
		hBitmap=LoadBitmap( hInst, MAKEINTRESOURCE(IDB_PT0)); 
		SelectObject(hMemDc,hBitmap);
		BitBlt(hdc, 148,70,rect.right,rect.bottom,hMemDc,0,0,SRCCOPY);
	}
	if(!delpt[3]){
		hBitmap=LoadBitmap( hInst, MAKEINTRESOURCE(IDB_PT0)); 
		SelectObject(hMemDc,hBitmap);
		BitBlt(hdc, 581,178,rect.right,rect.bottom,hMemDc,0,0,SRCCOPY);
	}
	if(!delpt[4]){
		hBitmap=LoadBitmap( hInst, MAKEINTRESOURCE(IDB_PT0)); 
		SelectObject(hMemDc,hBitmap);
		BitBlt(hdc, 40,178,rect.right,rect.bottom,hMemDc,0,0,SRCCOPY);
	}
	if(!delpt[5]){
		hBitmap=LoadBitmap( hInst, MAKEINTRESOURCE(IDB_PT0)); 
		SelectObject(hMemDc,hBitmap);
		BitBlt(hdc, 450,380,rect.right,rect.bottom,hMemDc,0,0,SRCCOPY);
	}
	if(!delpt[6]){
		hBitmap=LoadBitmap( hInst, MAKEINTRESOURCE(IDB_PT0)); 
		SelectObject(hMemDc,hBitmap);
		BitBlt(hdc, 318,224,rect.right,rect.bottom,hMemDc,0,0,SRCCOPY);
	}
	if(!delpt[7]){
		hBitmap=LoadBitmap( hInst, MAKEINTRESOURCE(IDB_PT0)); 
		SelectObject(hMemDc,hBitmap);
		BitBlt(hdc, 116,298,rect.right,rect.bottom,hMemDc,0,0,SRCCOPY);
	}
	if(!delpt[8]){
		hBitmap=LoadBitmap( hInst, MAKEINTRESOURCE(IDB_PT0)); 
		SelectObject(hMemDc,hBitmap);
		BitBlt(hdc, 518,298,rect.right,rect.bottom,hMemDc,0,0,SRCCOPY);
	}
	if(!delpt[9]){
		hBitmap=LoadBitmap( hInst, MAKEINTRESOURCE(IDB_PT0)); 
		SelectObject(hMemDc,hBitmap);
		BitBlt(hdc, 316,68,rect.right,rect.bottom,hMemDc,0,0,SRCCOPY);
	}
	if((((g1x>=x&&rectm.right>=g1x)&&(g1y>=y&&rectm.bottom>=g1y))||((g2x>=x&&rectm.right>=g2x)&&(g2y>=y&&rectm.bottom>=g2y))||((g3x>=x&&rectm.right>=g3x)&&(g3y>=y&&rectm.bottom>=g3y)))&&(!status)){
		status=true;
		CloseHandle (gThread1);
		CloseHandle (gThread2);
		CloseHandle (gThread3);
		int msgboxID = MessageBox(
        NULL,
        (LPCWSTR)L"You lose...",
        (LPCWSTR)L"LOSE",
        MB_ICONWARNING | MB_OK | MB_DEFBUTTON1
		);

		switch (msgboxID)
		{
		case IDOK:
			PostQuitMessage (0);
			return 0;
			break;
		}
		return 0;				   
	}
	if((score==10)&&(!status)){
		status=true;
		CloseHandle (gThread1);
		CloseHandle (gThread2);
		int msgboxID = MessageBox(
        NULL,
        (LPCWSTR)L"You win !!!",
        (LPCWSTR)L"WIN",
        MB_ICONWARNING | MB_OK | MB_DEFBUTTON1
		);

		switch (msgboxID)
		{
		case IDOK:
			PostQuitMessage (0);
			return 0;
			break;
		}
		return 0;
	}
	DeleteDC(hMemDc);
    EndPaint (hWnd, &ps);
	EnableHardwareKeyboard(false);
	Sleep(100);
	EnableHardwareKeyboard(true);
    return 0;
}
//----------------------------------------------------------------------
// DoDestroyMain - Process WM_DESTROY message for window.
//
LRESULT DoDestroyMain (HWND hWnd, UINT wMsg, WPARAM wParam, 
                       LPARAM lParam) {
    PostQuitMessage (0);
    return 0;
}
//----------------------------------------------------------------------
// DoKeysMain - Process all keyboard messages for window.
//
LRESULT DoKeysMain (HWND hWnd, UINT wMsg, WPARAM wParam, 
                    LPARAM lParam) {
	
	int preM = master;
	int preS = score;
	RECT rect;
	int prex=x;
	int prey=y;
	GetClientRect (hWnd, &rect);
	rectm.left = x;
	rectm.right = x+36;
	rectm.top = y;
	rectm.bottom = y+36;
	byte cenR, cenL, cenU, cenD, RU, RD, LU, LD;
	HDC hdc;
	hdc = GetDC (hWnd);

	switch(wParam) 
	{
		case VK_LEFT:
			master=IDB_MLO;
			cenL = GetBValue(GetPixel(hdc, rectm.left-8, (rectm.top+rectm.bottom)/2));
			LU = GetBValue(GetPixel(hdc, rectm.left-8, rectm.top));
			LD = GetBValue(GetPixel(hdc, rectm.left-8, rectm.bottom));
			if(cenL | LU | LD)
				break;
			if(x>8)
				x-=8;
			else
				x=0;
			rectm.right = prex+(rectm.right-rectm.left);
			rectm.left = prex-8;
			break;
		case VK_UP:
			master=IDB_MUO;
			cenU = GetBValue(GetPixel(hdc, (int)(rectm.left+rectm.right)/2, rectm.top-8));
			RU = GetBValue(GetPixel(hdc, rectm.right, rectm.top-8));
			LU = GetBValue(GetPixel(hdc, rectm.left, rectm.top-8));
			if(cenU | LU | RU)
				break;
			if(y>8)
				y-=8;
			else
				y=0;
			rectm.bottom = prey+(rectm.bottom-rectm.top);
			rectm.top = prey-8;
			break;
		case VK_RIGHT:
			master=IDB_MRO;
			cenR = GetBValue(GetPixel(hdc, rectm.right+8, (rectm.top+rectm.bottom)/2));
			RU = GetBValue(GetPixel(hdc, rectm.right+8, rectm.top));
			RD = GetBValue(GetPixel(hdc, rectm.right+8, rectm.bottom));
			if(cenR | RU | RD)
				break;
			if(rectm.right<rect.right-8)
				x+=8;
			else
				x=rect.right-(rectm.right-rectm.left);
			rectm.left = prex;
			rectm.right = rectm.right+8;
			break;
		case VK_DOWN:
			master=IDB_MDO;
			cenD = GetBValue(GetPixel(hdc, (rectm.left+rectm.right)/2, rectm.bottom+8));
			RD = GetBValue(GetPixel(hdc, rectm.right, rectm.bottom+8));
			LD = GetBValue(GetPixel(hdc, rectm.left, rectm.bottom+8));
			if(cenD | RD | LD)
				break;
			if(rectm.bottom<rect.bottom-8)
				y+=8;
			else
				y=rect.bottom-(rectm.bottom-rectm.top);
			rectm.top = prey;
			rectm.bottom = rectm.bottom+8;
			break;
	}
	if(preM!=IDB_MCL)
		master=IDB_MCL;
	delpt[0] = delpt[0]?delpt[0]:(42>x&&rectm.right>42)&&(336>y&&rectm.bottom>336);
	delpt[1] = delpt[1]?delpt[1]:(482>x&&rectm.right>482)&&(70>y&&rectm.bottom>70);
	delpt[2] = delpt[2]?delpt[2]:(148>x&&rectm.right>148)&&(70>y&&rectm.bottom>70);
	delpt[3] = delpt[3]?delpt[3]:(581>x&&rectm.right>581)&&(178>y&&rectm.bottom>178);
	delpt[4] = delpt[4]?delpt[4]:(40>x&&rectm.right>40)&&(178>y&&rectm.bottom>178);
	delpt[5] = delpt[5]?delpt[5]:(450>x&&rectm.right>450)&&(380>y&&rectm.bottom>380);
	delpt[6] = delpt[6]?delpt[6]:(318>x&&rectm.right>318)&&(224>y&&rectm.bottom>224);
	delpt[7] = delpt[7]?delpt[7]:(116>x&&rectm.right>116)&&(298>y&&rectm.bottom>298);
	delpt[8] = delpt[8]?delpt[8]:(518>x&&rectm.right>518)&&(298>y&&rectm.bottom>298);
	delpt[9] = delpt[9]?delpt[9]:(316>x&&rectm.right>316)&&(68>y&&rectm.bottom>68);
	score = 0;
	for(int i=0;i<10;i++){
		if(delpt[i])
			score++;
	}
	if(preS!=score){
		rect.top=2;
		rect.left=4;
		rect.right=84;
		rect.bottom=71;
		InvalidateRect (hWnd, &rect, false);
	}
	InvalidateRect (hWnd, &rectm, false);
	return 0;
}

DWORD WINAPI ghost1(PVOID pArg) {
	HWND hWnd=(HWND)pArg;;
	RECT rect;
	GetClientRect (hWnd, &rect);
	byte cenR, cenL, cenU, cenD, RU, RD, LU, LD;
	HDC hdc;
	hdc = GetDC (hWnd);
	int way=0;
	while(1){
		int prex=g1x;
		int prey=g1y;
		rectg1.left = g1x;
		rectg1.right = g1x+36;
		rectg1.top = g1y;
		rectg1.bottom = g1y+36;
		srand(GetTickCount());
		switch(way) 
		{
			case 0:
				cenL = GetBValue(GetPixel(hdc, rectg1.left-8, (rectg1.top+rectg1.bottom)/2));
				LU = GetBValue(GetPixel(hdc, rectg1.left-8, rectg1.top));
				LD = GetBValue(GetPixel(hdc, rectg1.left-8, rectg1.bottom));
				if(cenL | LU | LD)
				{way = rand()%4;break;}
				if(g1x>8)
					g1x-=8;
				else
					g1x=0;
				rectg1.right = prex+(rectg1.right-rectg1.left);
				rectg1.left = prex-8;
				break;
			case 1:
				cenU = GetBValue(GetPixel(hdc, (int)(rectg1.left+rectg1.right)/2, rectg1.top-8));
				RU = GetBValue(GetPixel(hdc, rectg1.right, rectg1.top-8));
				LU = GetBValue(GetPixel(hdc, rectg1.left, rectg1.top-8));
				if(cenU | LU | RU)
				{way = rand()%4;break;}
				if(g1y>8)
					g1y-=8;
				else
					g1y=0;
				rectg1.bottom = prey+(rectg1.bottom-rectg1.top);
				rectg1.top = prey-8;
				break;
			case 2:
				cenR = GetBValue(GetPixel(hdc, rectg1.right+8, (rectg1.top+rectg1.bottom)/2));
				RU = GetBValue(GetPixel(hdc, rectg1.right+8, rectg1.top));
				RD = GetBValue(GetPixel(hdc, rectg1.right+8, rectg1.bottom));
				if(cenR | RU | RD)
				{way = rand()%4;break;}
				if(rectg1.right<rect.right-8)
					g1x+=8;
				else
					g1x=rect.right-(rectg1.right-rectg1.left);
				rectg1.left = prex;
				rectg1.right = rectg1.right+8;
				break;
			case 3:
				cenD = GetBValue(GetPixel(hdc, (rectg1.left+rectg1.right)/2, rectg1.bottom+8));
				RD = GetBValue(GetPixel(hdc, rectg1.right, rectg1.bottom+8));
				LD = GetBValue(GetPixel(hdc, rectg1.left, rectg1.bottom+8));
				if(cenD | RD | LD)
				{way = rand()%4;break;}
				if(rectg1.bottom<rect.bottom-8)
					g1y+=8;
				else
					g1y=rect.bottom-(rectg1.bottom-rectg1.top);
				rectg1.top = prey;
				rectg1.bottom = rectg1.bottom+8;
				break;
		}
		
		if(g1==IDB_GHOST1)
			g1=IDB_GHOST11;
		else
			g1=IDB_GHOST1;
		InvalidateRect (hWnd, &rectg1, false);
		Sleep(100);
	}
	return 0;
}
DWORD WINAPI ghost2(PVOID pArg) {
	HWND hWnd=(HWND)pArg;;
	RECT rect;
	GetClientRect (hWnd, &rect);
	byte cenR, cenL, cenU, cenD, RU, RD, LU, LD;
	HDC hdc;
	hdc = GetDC (hWnd);
	int way=0;
	while(1){
		int prex=g2x;
		int prey=g2y;
		rectg2.left = g2x;
		rectg2.right = g2x+36;
		rectg2.top = g2y;
		rectg2.bottom = g2y+36;
		srand(GetTickCount());
		switch(way) 
		{
			case 0:
				cenL = GetBValue(GetPixel(hdc, rectg2.left-8, (rectg2.top+rectg2.bottom)/2));
				LU = GetBValue(GetPixel(hdc, rectg2.left-8, rectg2.top));
				LD = GetBValue(GetPixel(hdc, rectg2.left-8, rectg2.bottom));
				if(cenL | LU | LD)
				{way = rand()%4;break;}
				if(g2x>8)
					g2x-=8;
				else
					g2x=0;
				rectg2.right = prex+(rectg2.right-rectg2.left);
				rectg2.left = prex-8;
				break;
			case 1:
				cenU = GetBValue(GetPixel(hdc, (int)(rectg2.left+rectg2.right)/2, rectg2.top-8));
				RU = GetBValue(GetPixel(hdc, rectg2.right, rectg2.top-8));
				LU = GetBValue(GetPixel(hdc, rectg2.left, rectg2.top-8));
				if(cenU | LU | RU)
				{way = rand()%4;break;}
				if(g2y>8)
					g2y-=8;
				else
					g2y=0;
				rectg2.bottom = prey+(rectg2.bottom-rectg2.top);
				rectg2.top = prey-8;
				break;
			case 2:
				cenR = GetBValue(GetPixel(hdc, rectg2.right+8, (rectg2.top+rectg2.bottom)/2));
				RU = GetBValue(GetPixel(hdc, rectg2.right+8, rectg2.top));
				RD = GetBValue(GetPixel(hdc, rectg2.right+8, rectg2.bottom));
				if(cenR | RU | RD)
				{way = rand()%4;break;}
				if(rectg2.right<rect.right-8)
					g2x+=8;
				else
					g2x=rect.right-(rectg2.right-rectg2.left);
				rectg2.left = prex;
				rectg2.right = rectg2.right+8;
				break;
			case 3:
				cenD = GetBValue(GetPixel(hdc, (rectg2.left+rectg2.right)/2, rectg2.bottom+8));
				RD = GetBValue(GetPixel(hdc, rectg2.right, rectg2.bottom+8));
				LD = GetBValue(GetPixel(hdc, rectg2.left, rectg2.bottom+8));
				if(cenD | RD | LD)
				{way = rand()%4;break;}
				if(rectg2.bottom<rect.bottom-8)
					g2y+=8;
				else
					g2y=rect.bottom-(rectg2.bottom-rectg2.top);
				rectg2.top = prey;
				rectg2.bottom = rectg2.bottom+8;
				break;
		}
		
		if(g2==IDB_G2)
			g2=IDB_G22;
		else
			g2=IDB_G2;
		InvalidateRect (hWnd, &rectg2, false);
		Sleep(100);
	}
	return 0;
}
DWORD WINAPI ghost3(PVOID pArg) {
	HWND hWnd=(HWND)pArg;;
	RECT rect;
	GetClientRect (hWnd, &rect);
	byte cenR, cenL, cenU, cenD, RU, RD, LU, LD;
	HDC hdc;
	hdc = GetDC (hWnd);
	int way=0;
	while(1){
		int prex=g3x;
		int prey=g3y;
		rectg3.left = g3x;
		rectg3.right = g3x+36;
		rectg3.top = g3y;
		rectg3.bottom = g3y+36;
		srand(GetTickCount());
		switch(way) 
		{
			case 0:
				cenL = GetBValue(GetPixel(hdc, rectg3.left-8, (rectg3.top+rectg3.bottom)/2));
				LU = GetBValue(GetPixel(hdc, rectg3.left-8, rectg3.top));
				LD = GetBValue(GetPixel(hdc, rectg3.left-8, rectg3.bottom));
				if(cenL | LU | LD)
				{way = rand()%4;break;}
				if(g3x>8)
					g3x-=8;
				else
					g3x=0;
				rectg3.right = prex+(rectg3.right-rectg3.left);
				rectg3.left = prex-8;
				break;
			case 1:
				cenU = GetBValue(GetPixel(hdc, (int)(rectg3.left+rectg3.right)/2, rectg3.top-8));
				RU = GetBValue(GetPixel(hdc, rectg3.right, rectg3.top-8));
				LU = GetBValue(GetPixel(hdc, rectg3.left, rectg3.top-8));
				if(cenU | LU | RU)
				{way = rand()%4;break;}
				if(g3y>8)
					g3y-=8;
				else
					g3y=0;
				rectg3.bottom = prey+(rectg3.bottom-rectg3.top);
				rectg3.top = prey-8;
				break;
			case 2:
				cenR = GetBValue(GetPixel(hdc, rectg3.right+8, (rectg3.top+rectg3.bottom)/2));
				RU = GetBValue(GetPixel(hdc, rectg3.right+8, rectg3.top));
				RD = GetBValue(GetPixel(hdc, rectg3.right+8, rectg3.bottom));
				if(cenR | RU | RD)
				{way = rand()%4;break;}
				if(rectg3.right<rect.right-8)
					g3x+=8;
				else
					g3x=rect.right-(rectg3.right-rectg3.left);
				rectg3.left = prex;
				rectg3.right = rectg3.right+8;
				break;
			case 3:
				cenD = GetBValue(GetPixel(hdc, (rectg3.left+rectg3.right)/2, rectg3.bottom+8));
				RD = GetBValue(GetPixel(hdc, rectg3.right, rectg3.bottom+8));
				LD = GetBValue(GetPixel(hdc, rectg3.left, rectg3.bottom+8));
				if(cenD | RD | LD)
				{way = rand()%4;break;}
				if(rectg3.bottom<rect.bottom-8)
					g3y+=8;
				else
					g3y=rect.bottom-(rectg3.bottom-rectg3.top);
				rectg3.top = prey;
				rectg3.bottom = rectg3.bottom+8;
				break;
		}
		
		if(g3==IDB_GHOST1)
			g3=IDB_GHOST11;
		else
			g3=IDB_GHOST1;
		InvalidateRect (hWnd, &rectg3, false);
		Sleep(100);
	}
	return 0;
}