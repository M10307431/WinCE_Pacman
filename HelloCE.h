//======================================================================
// Header file
//
// Written for the book Programming Windows CE
// Copyright (C) 2007 Douglas Boling
//================================================================
// Returns number of elements
#define dim(x) (sizeof(x) / sizeof(x[0])) 

//----------------------------------------------------------------------
// Generic defines and data types
//
struct decodeUINT {                            // Structure associates
    UINT Code;                                 // messages 
                                               // with a function. 
    LRESULT (*Fxn)(HWND, UINT, WPARAM, LPARAM);
}; 
struct decodeCMD {                             // Structure associates
    UINT Code;                                 // menu IDs with a 
    LRESULT (*Fxn)(HWND, WORD, HWND, WORD);    // function
};

//----------------------------------------------------------------------
// Function prototypes
//
HWND InitInstance (HINSTANCE, LPWSTR, int);
int TermInstance (HINSTANCE, int);
DWORD WINAPI ghost1(PVOID pArg);
DWORD WINAPI ghost2(PVOID pArg);
DWORD WINAPI ghost3(PVOID pArg);

// Window procedures
LRESULT CALLBACK MainWndProc (HWND, UINT, WPARAM, LPARAM);

// Message handlers
LRESULT DoCreateMain (HWND, UINT, WPARAM, LPARAM);
LRESULT DoPaintMain (HWND, UINT, WPARAM, LPARAM);
LRESULT DoKeysMain (HWND, UINT, WPARAM, LPARAM);
LRESULT DoDestroyMain (HWND, UINT, WPARAM, LPARAM);
