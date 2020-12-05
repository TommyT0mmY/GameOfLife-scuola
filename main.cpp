#include <iostream>
#include <vector>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <math.h>

using namespace std;

int const ConsoleHeight = 40, ConsoleWidth = 120;
int const UpdatesPerSecond = 5;

vector<vector<bool>> game(ConsoleWidth-2/*x*/, vector<bool>(ConsoleHeight-3/*y*/));
vector<char> command_line;

VOID WriteToScreen(wchar_t* screenBuffer, int x, int y, char c)
{
    screenBuffer[y * ConsoleWidth + x] = c;
}

VOID DrawCommandLine(wchar_t* screenBuffer)
{
    WriteToScreen(screenBuffer, 1, ConsoleHeight - 2, '>');
    for (int x = 0; x < ((command_line.size() <= ConsoleWidth - 1) ? command_line.size() : ConsoleWidth - 1); ++x)
        WriteToScreen(screenBuffer, x + 2, (ConsoleHeight - 2), command_line[x]);
}

VOID ReadInput(HANDLE inputHandle)
{
    DWORD numberOfInputs;
    GetNumberOfConsoleInputEvents(inputHandle, &numberOfInputs);
    if (numberOfInputs <= 0)
        return;

    INPUT_RECORD irInBuf[128];
    ReadConsoleInput(inputHandle, irInBuf, 128, &numberOfInputs);

    for (int i = 0; i < numberOfInputs; ++i)
    {
        //tasto premuto
        if (irInBuf[i].EventType == KEY_EVENT && irInBuf[i].Event.KeyEvent.bKeyDown)
        {
            CHAR pressedKey = irInBuf[i].Event.KeyEvent.uChar.AsciiChar;
            WORD virtualKeyCode = irInBuf[i].Event.KeyEvent.wVirtualKeyCode;
            if (virtualKeyCode == VK_BACK) //backspace
            {
                if (command_line.size() > 0)
                    command_line.pop_back();   
            }
            else if (isalnum(pressedKey) || virtualKeyCode == VK_SPACE) //alfanumerico o spazio
            {
                if (command_line.size() < ConsoleWidth - 3)
                    command_line.push_back(pressedKey);
            }
        }
    }
}

int main()
{
    //debug, lo stdout scriverà a file
    freopen("logs.txt", "w", stdout);

    //buffer dello schermo
    wchar_t* screen = new wchar_t[ConsoleWidth * ConsoleHeight];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    //titolo della finestra
    SetConsoleTitleA("Game Of Life");

    //rendo il cursore invisibile
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false; // set the cursor visibility
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    //handle per l'input
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

    auto t1 = chrono::high_resolution_clock::now();
    auto t2 = chrono::high_resolution_clock::now();
    chrono::nanoseconds NanosecondsPerFrame((int)(1e6/UpdatesPerSecond));

    chrono::nanoseconds Counter(0);

    while(1)
    {
        t1 = chrono::high_resolution_clock::now();

        //Pulizia buffer
        fill_n(screen, ConsoleWidth * ConsoleHeight, ' ');

        //lettura e gestione degli input
        ReadInput(hInput);

        if (Counter.count() >= NanosecondsPerFrame.count()) {
            Counter = chrono::nanoseconds(0);

            /*

            UPDATE GIOCO

            */
        }

        //draw gioco
        //TODO

        //draw bordi

        for (int x = 0; x < ConsoleWidth; ++x)
        {
            WriteToScreen(screen, x, 0, '#');
            WriteToScreen(screen, x, ConsoleHeight-1, '#');
        }
        for (int y = 0; y < ConsoleHeight; ++y)
        {
            WriteToScreen(screen, 0, y, '|');
            WriteToScreen(screen, ConsoleWidth-1, y, '|');
        }
        
        //draw linea di comando
        DrawCommandLine(screen);

        //scrittura buffer schermo
        screen[ConsoleWidth * ConsoleHeight - 1] = '\0';
        WriteConsoleOutputCharacterW(hConsole, screen, ConsoleWidth * ConsoleHeight, { 0,0 }, &dwBytesWritten);

        t2 = chrono::high_resolution_clock::now();
        
        auto delta_time = t2-t1;

        Counter+=delta_time;
        Sleep(3);
    }

    return 0;
}
