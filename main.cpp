#include <iostream>
#include <vector>
#include <Windows.h>
#include <chrono>


int const ConsoleHeight = 40, ConsoleWidth = 120;

using namespace std;

vector<vector<bool>> game(ConsoleWidth/*x*/, vector<bool>(ConsoleHeight/*y*/));

VOID WriteToScreen(wchar_t* screenBuffer, int x, int y, char c)
{
    screenBuffer[y * ConsoleWidth + x] = c;
}

VOID KeyPressedEvent(KEY_EVENT_RECORD ker, wchar_t* screenBuffer)
{
    char a = ker.uChar.AsciiChar;
    WriteToScreen(screenBuffer, 10, 10, a);
    if (ker.bKeyDown)
        WriteToScreen(screenBuffer, 11, 11, 'B');
}

int main()
{
    //buffer dello schermo
    wchar_t* screen = new wchar_t[ConsoleWidth * ConsoleHeight];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;
    //rendo il cursore invisibile
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false; // set the cursor visibility
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    //inizializzazione buffer
    for(int x = 0; x < ConsoleWidth; ++x)
        for(int y = 0; y < ConsoleHeight; ++y)
            screen[y * ConsoleWidth + x] = ' ';
    freopen("logs.txt","w",stdout); //debug
    //handle per l'input
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);


    while(1)
    {
        Sleep(1);
        //lettura degli input
        DWORD numberOfInputs;
        GetNumberOfConsoleInputEvents(hInput, &numberOfInputs);
        if (numberOfInputs > 0){
            INPUT_RECORD irInBuf[128];

            ReadConsoleInput(hInput, irInBuf, 128, &numberOfInputs);
            for (int i = 0; i < numberOfInputs; ++i)
            {
                if (irInBuf[i].EventType == KEY_EVENT && irInBuf[i].Event.KeyEvent.bKeyDown)
                {
                    screen[10*ConsoleWidth+10]=irInBuf[i].Event.KeyEvent.uChar.AsciiChar;
                }
            }
        }
        

        //scrittura buffer schermo
        screen[ConsoleWidth * ConsoleHeight - 1] = '\0';
        WriteConsoleOutputCharacterW(hConsole, screen, ConsoleWidth * ConsoleHeight, { 0,0 }, &dwBytesWritten);
    }


    return 0;
}
