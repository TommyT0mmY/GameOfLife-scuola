#include <iostream>
#include <vector>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <math.h>

using namespace std;

int const ConsoleHeight = 40, ConsoleWidth = 120;
int const UpdatesPerSecond = 1;

vector<vector<bool>> game(ConsoleWidth-2/*x*/, vector<bool>(ConsoleHeight-3/*y*/));
vector<char> command_line;

int wrap(int n , int max){
    if(n < 0){
        return max - (abs(n) % max);
    }
    return n % max;
}

VOID update() {
vector<vector<bool> > newGame(ConsoleWidth-2, vector<bool>(ConsoleHeight-3));
    for(int x = 0; x < game.size();x++){
        for(int y = 0; y < game[x].size();y++){
            //for each cell
            int aliveCell = 0;

            //dumb code
            if(game[wrap(x - 1,game.size())][wrap(y - 1,game[x].size())]){
                aliveCell++;
            }
            if(game[wrap(x,game.size())][wrap(y - 1,game[x].size())]){
                aliveCell++;
            }
            if(game[wrap(x + 1,game.size())][wrap(y - 1,game[x].size())]){
                aliveCell++;
            }
            if(game[wrap(x - 1,game.size())][wrap(y,game[x].size())]){
                aliveCell++;
            }
            if(game[wrap(x + 1,game.size())][wrap(y,game[x].size())]){
                aliveCell++;
            }
            if(game[wrap(x - 1,game.size())][wrap(y + 1,game[x].size())]){
                aliveCell++;
            }
            if(game[wrap(x,game.size())][wrap(y + 1,game[x].size())]){
                aliveCell++;
            }
            if(game[wrap(x + 1,game.size())][wrap(y + 1,game[x].size())]){
                aliveCell++;
            }
            //big dumb

            if(aliveCell < 2){
                newGame[x][y] = 0;
            }
            if((aliveCell == 2 || aliveCell == 3) && game[x][y]){
                newGame[x][y] = 1;
            }
            if(aliveCell == 3 && !game[x][y]){
                newGame[x][y] = 1;
            }
            if(aliveCell > 3){
                newGame[x][y] = 0;
            }
        }
    }
    game = newGame;
}

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

VOID RunCommand()
{
    //TODO
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
            else if (virtualKeyCode == VK_RETURN)
            {
                RunCommand();
                command_line.clear();
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
    chrono::nanoseconds NanosecondsPerFrame((int)(1.f/(float)UpdatesPerSecond*1e9));

    chrono::nanoseconds Counter(0);

    game[4][4] = true;
    game[5][4] = true;
    game[6][4] = true;
    game[7][4] = true;
    game[8][4] = true;
    game[9][4] = true;
    game[10][4] = true;
    game[11][4] = true;
    //12
    game[13][4] = true;
    game[14][4] = true;
    game[15][4] = true;
    game[16][4] = true;
    game[17][4] = true;
    //18
    //19
    //20
    game[21][4] = true;
    game[22][4] = true;
    game[23][4] = true;
    //24
    //25
    //26
    //27
    //28
    //29
    game[30][4] = true;
    game[31][4] = true;
    game[32][4] = true;
    game[33][4] = true;
    game[34][4] = true;
    game[35][4] = true;
    game[36][4] = true;
    //37
    game[38][4] = true;
    game[39][4] = true;
    game[40][4] = true;
    game[41][4] = true;
    game[42][4] = true;
    while(1)
    {
        t1 = chrono::high_resolution_clock::now();

        //Pulizia buffer
        fill_n(screen, ConsoleWidth * ConsoleHeight, ' ');

        //lettura e gestione degli input
        ReadInput(hInput);

        if (Counter >= NanosecondsPerFrame) {
            Counter = chrono::nanoseconds(0);
            cout << "----\n";
            update();

        }

        //draw gioco

        for (int x = 0; x < game.size(); ++x)
        {
            for (int y = 0; y < game[0].size(); ++y)
            {
                if (game[x][y])
                    WriteToScreen(screen, x + 1, y + 1, '*');
            }
        }

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

        Sleep(3);

        t2 = chrono::high_resolution_clock::now();

        std::chrono::nanoseconds delta_time = t2-t1;

        Counter+=delta_time;
    }

    return 0;
}
