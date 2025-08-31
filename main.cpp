#include <iostream>
#include <windows.h>
#include <conio.h>
#include <atomic>
#include <thread>
using namespace std;

// -------------------
// Application constants
// -------------------
const string NAME = "Quill";
const string VERSION = "1.0.0";
const string APPINFO = NAME + " " + VERSION;
const char* APPTITLE = APPINFO.c_str();
enum Mods{
    Inset_Mode,
    Read_Mode,
    None_Mode
};
//--------------------
// Threads
//--------------------

atomic<Mods> currentMode(None_Mode);
// -------------------
// Structure to store console size
// -------------------
struct Window{
    short Column;
    short Row;
};



// -------------------
// Function declarations
// -------------------
void init();       // Initialize console (size, title, cursor)
void TitleApp();   // Display top bar / welcome message
void KeyShorcuts();
void CursorWatcher();
bool isAllowedChar(char c);
void DrawStatusBar();
// -------------------
// Main program loop
// -------------------
int main() {
    init();        // Setup console
    TitleApp();    // Show welcome message
    thread listener(KeyShorcuts);
    listener.detach();
    thread cursorGuard(CursorWatcher);
    cursorGuard.detach();

    char c;
    while ((c = getch()) != 'E') {
        if(currentMode == Inset_Mode){
            if(c == '\r'){
                cout<<"\n";
            }else if (c == 8) {
                // Backspace
                cout << "\b \b";
            }
            else if (isAllowedChar(c)) {
                cout << c;
            }
        }
    }
    return 0;
}

// -------------------
// Display welcome text at top of console
// -------------------
void TitleApp(){
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    Window WinSize = {120,0};

    if(GetConsoleScreenBufferInfo(hConsole,&csbi)){
        WinSize.Column = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        WinSize.Row = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }

    // Fill top line with colored background and display centered text
    SetConsoleTextAttribute(hConsole, 15 + (1 << 4)); // White on red
    COORD Pos = {0,0};
    SetConsoleCursorPosition(hConsole, Pos);
    cout << string(WinSize.Column , ' ');

    const string Title = "Welcome To Quill Editor";
    Pos.X = static_cast<SHORT>((WinSize.Column - Title.length()) / 2);
    Pos.Y = 0;
    SetConsoleCursorPosition(hConsole, Pos);
    cout << Title;

    // Move cursor to start of next line
    SetConsoleTextAttribute(hConsole, 15);
    Pos.X = 0;
    Pos.Y = 1;
    SetConsoleCursorPosition(hConsole, Pos);
}

// -------------------
// Initialize console settings (size, title, cursor)
// -------------------
void init() {
    HWND ConsoleHandler = GetConsoleWindow();
    SetConsoleTitleA(APPTITLE);

    // Disable maximize and resize
    LONG style = GetWindowLong(ConsoleHandler, GWL_STYLE);
    style &= ~WS_MAXIMIZEBOX;
    style &= ~WS_SIZEBOX;
    SetWindowLong(ConsoleHandler, GWL_STYLE, style);

    // Set console buffer size and window size
    HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD BufferSize = {120, 31};
    SetConsoleScreenBufferSize(ConsoleHandle, BufferSize);
    SMALL_RECT windowSize = {0, 0, 120, 30};
    SetConsoleWindowInfo(ConsoleHandle, TRUE, &windowSize);

    // Set cursor visibility and size
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(ConsoleHandle,&cci);
    cci.dwSize = 100;
    cci.bVisible = TRUE;
    SetConsoleCursorInfo(ConsoleHandle,&cci);
}

void KeyShorcuts(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DrawStatusBar();
    while (true){
        // ESC = خروج از مدها
        if(GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            currentMode = None_Mode;
            DrawStatusBar();
        }

        // Ctrl + I = Insert Mode
        if((currentMode == None_Mode || currentMode == Read_Mode) &&
           ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState('I') & 0x8000))){
            currentMode = Inset_Mode;
            DrawStatusBar();
        }

        // Ctrl + R = Read Mode
        if((currentMode == None_Mode || currentMode == Inset_Mode) &&
           ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState('R') & 0x8000))){
            currentMode = Read_Mode;
            DrawStatusBar();
        }

        // Ctrl + W = کرسر یک خط بالا
        if((currentMode == Read_Mode) && ((GetAsyncKeyState('W') & 0x8000))) {
            if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
                COORD newPos = csbi.dwCursorPosition;
                if (newPos.Y > 0) newPos.Y -= 1; // یک خط بالا
                SetConsoleCursorPosition(hConsole, newPos);
            }
            Sleep(200); // برای جلوگیری از پرش سریع چند خطی
        }
        if((currentMode == Read_Mode) && ( (GetAsyncKeyState('S') & 0x8000))) {
            if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
                COORD newPos = csbi.dwCursorPosition;
                newPos.Y += 1; // یک خط بالا
                SetConsoleCursorPosition(hConsole, newPos);
            }
            Sleep(200); // برای جلوگیری از پرش سریع چند خطی
        }
        if((currentMode == Read_Mode)  && (GetAsyncKeyState('A') & 0x8000)) {
            if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
                COORD newPos = csbi.dwCursorPosition;
                if (newPos.X > 0) newPos.X -= 1; // یک خط بالا
                SetConsoleCursorPosition(hConsole, newPos);
            }
            Sleep(200); // برای جلوگیری از پرش سریع چند خطی
        }
        if((currentMode == Read_Mode) && ((GetAsyncKeyState('D') & 0x8000))) {
            if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
                COORD newPos = csbi.dwCursorPosition;
                newPos.X += 1; // یک خط بالا
                SetConsoleCursorPosition(hConsole, newPos);
            }
            Sleep(10); // برای جلوگیری از پرش سریع چند خطی
        }

        Sleep(50); // کاهش مصرف CPU
    }
}

void CursorWatcher() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    while (true) {
        if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {

            if (csbi.dwCursorPosition.Y < 1) {
                COORD newPos = csbi.dwCursorPosition;
                newPos.Y = 1;
                SetConsoleCursorPosition(hConsole, newPos);
            }
        }
        Sleep(50);
    }
}


bool isAllowedChar(char c) {
    // اعداد
    if (c >= '0' && c <= '9') return true;
    // حروف بزرگ
    if (c >= 'A' && c <= 'Z') return true;
    // حروف کوچک
    if (c >= 'a' && c <= 'z') return true;
    if (c == ' ') return true;
    // کاراکترهای خاص مجاز
    string allowed = "{}[]\"':;+=-_?/><.\\|*&^%$#@!~";
    if (allowed.find(c) != string::npos) return true;

    return false;
}


void DrawStatusBar() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    // متن مد
    string modeText;
    switch (currentMode.load()) {
        case Inset_Mode: modeText = "Mode --Insert-- | Ctrl+S [Save]"; break;
        case Read_Mode:  modeText = "Mode --Read--   | [w,s,d,a] | Ctrl+S [Save]"; break;
        case None_Mode:  modeText = "Mode --None--   | Ctrl+S [Save]"; break;
    }

    string text = modeText;

    // نوار رنگی در آخرین خط
    COORD pos = {0, (short)(csbi.srWindow.Bottom)};
    SetConsoleCursorPosition(hConsole, pos);
    SetConsoleTextAttribute(hConsole, 0x70); // سفید روی خاکستری
    cout << string(width, ' '); // پاک کردن خط
    SetConsoleCursorPosition(hConsole, pos);
    cout << text;
    SetConsoleTextAttribute(hConsole, 15); // برگردوندن رنگ پیش‌فرض
    pos = {0,0};
    SetConsoleCursorPosition(hConsole, pos);
}



