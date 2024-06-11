// Include Different Packages
#include <windows.h>
#include <string>
#include <vector>

// Setting up window stuff for the application (the different parameters)
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Add item and Remove Item Functions
void AddItem(HWND hwnd);
void RemoveItem(HWND hwnd);

// setting up vector of items
std::vector<std::wstring> items;

// define the input type button thingy
#define IDC_MAIN_EDIT 101
#define IDC_MAIN_BUTTON 102
#define IDC_MAIN_LISTBOX 103

// Setup Desktop Window
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"TodoApp";

    // deefault setup settings
    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    // registers as windows OS
    RegisterClass(&wc);

    // Sizing, name, and other things for the window
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"To-Do List App",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    // checks if created successfully
    if (hwnd == nullptr) {
        return 0;
    }

    // displays the window
    ShowWindow(hwnd, nCmdShow);

    // gets the user input and displays it onto the desktop window
    MSG msg = { };
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// Function for each "object" properties
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // basic setup settings
    static HWND hEdit, hButton, hButton2, hListBox;

    // switch statement to go through each element
    switch (uMsg) {
    case WM_CREATE: {
        // create and setup its pos, size, and other properties
        hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
            20, 20, 200, 25, hwnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);

        // create and setup its pos, size, and other properties
        hButton = CreateWindowEx(0, L"BUTTON", L"Add Item",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            240, 20, 100, 25, hwnd, (HMENU)IDC_MAIN_BUTTON, GetModuleHandle(NULL), NULL);

        // create and setup its pos, size, and other properties
        hButton2 = CreateWindowEx(0, L"BUTTON", L"Remove Item",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            340, 20, 100, 25, hwnd, (HMENU)IDC_MAIN_BUTTON, GetModuleHandle(NULL), NULL);

        // create and setup its pos, size, and other properties
        hListBox = CreateWindowEx(WS_EX_CLIENTEDGE, L"LISTBOX", NULL,
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL,
            20, 60, 320, 200, hwnd, (HMENU)IDC_MAIN_LISTBOX, GetModuleHandle(NULL), NULL);
        break;
    }
    // if user input is gathered  from buttons
    case WM_COMMAND:
        // see if add or remove item is clicked
        switch (LOWORD(wParam)) {
        case IDC_MAIN_BUTTON:
            // run functions
            AddItem(hwnd);
            RemoveItem(hwnd);
            break;
        }

       
        break;
    case WM_DESTROY:
        // close message
        PostQuitMessage(0);
        return 0;
    }
    // close message
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Add the additem element
void AddItem(HWND hwnd) {
    // setup the different properties
    wchar_t buffer[256];
    HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
    HWND hListBox = GetDlgItem(hwnd, IDC_MAIN_LISTBOX);

    // text and other size
    GetWindowText(hEdit, buffer, 256);
    std::wstring item(buffer);

    // affect the string
    if (!item.empty()) {
        items.push_back(item);
        // display
        SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)item.c_str());
        SetWindowText(hEdit, L"");
    }
}

// Add the removeitem element
void RemoveItem(HWND hwnd) {                                        // thanks nick
    // setup the different properties
    HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
    HWND hListBox = GetDlgItem(hwnd, IDC_MAIN_LISTBOX);
    
    // gets currently selected item in the box
    int iCurSel = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
    // loop through until element is the selected one
    for (int i = 0; i < items.size(); i++) {
        // if its selected
        if (i == iCurSel) {
            // deletes the item
            items.erase(items.begin() + i);
            // deletes from display
            SendMessage(hListBox, LB_DELETESTRING, iCurSel, 0);
        }
    }

}
