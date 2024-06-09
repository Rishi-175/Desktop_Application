#include <windows.h>
#include <string>
#include <vector>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void AddItem(HWND hwnd);
void RemoveItem(HWND hwnd);

std::vector<std::wstring> items;

#define IDC_MAIN_EDIT 101
#define IDC_MAIN_BUTTON 102
#define IDC_MAIN_LISTBOX 103

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"TodoApp";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

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

    if (hwnd == nullptr) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hEdit, hButton, hButton2, hListBox;

    switch (uMsg) {
    case WM_CREATE: {
        hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
            20, 20, 200, 25, hwnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);

        hButton = CreateWindowEx(0, L"BUTTON", L"Add Item",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            240, 20, 100, 25, hwnd, (HMENU)IDC_MAIN_BUTTON, GetModuleHandle(NULL), NULL);

        hButton2 = CreateWindowEx(0, L"BUTTON", L"Remove Item",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            340, 20, 100, 25, hwnd, (HMENU)IDC_MAIN_BUTTON, GetModuleHandle(NULL), NULL);

        hListBox = CreateWindowEx(WS_EX_CLIENTEDGE, L"LISTBOX", NULL,
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL,
            20, 60, 320, 200, hwnd, (HMENU)IDC_MAIN_LISTBOX, GetModuleHandle(NULL), NULL);
        break;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_MAIN_BUTTON:
            AddItem(hwnd);
            RemoveItem(hwnd);
            break;
        }

       
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void AddItem(HWND hwnd) {
    wchar_t buffer[256];
    HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
    HWND hListBox = GetDlgItem(hwnd, IDC_MAIN_LISTBOX);

    GetWindowText(hEdit, buffer, 256);
    std::wstring item(buffer);

    if (!item.empty()) {
        items.push_back(item);
        SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)item.c_str());
        SetWindowText(hEdit, L"");
    }
}

void RemoveItem(HWND hwnd) { //tahnks nick
    HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
    HWND hListBox = GetDlgItem(hwnd, IDC_MAIN_LISTBOX);

int iCurSel = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
    for (int i = 0; i < items.size(); i++) {
        if (i == iCurSel) {
            items.erase(items.begin() + i);
            
            SendMessage(hListBox, LB_DELETESTRING, iCurSel, 0);
        }
    }

}
