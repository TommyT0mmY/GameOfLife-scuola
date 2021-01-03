#include <Windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <math.h>
#include <thread>
#include <vector>

using namespace std;

int const ConsoleHeight = 40, ConsoleWidth = 120;
int const UpdatesPerSecond = 1; // FPS Simulazione

vector<vector<bool>> game(ConsoleWidth-2/*larghezza*/, vector<bool>(ConsoleHeight-3/*altezza*/));
vector<char> command_line;
bool esegui = false; // Indica se la simulazione deve eseguire

int wrap(int n , int max)
{
    if(n < 0) {
        return max - (abs(n) % max);
    }
    return n % max;
}

VOID update()
{
    vector<vector<bool>> newGame(ConsoleWidth-2, vector<bool>(ConsoleHeight-3));
    for(int x = 0; x < game.size(); x++){
        for(int y = 0; y < game[x].size(); y++) {

            // Per ogni cella

            int aliveCell = 0;

            for(int x1 = x - 1; x1 <= x + 1; x1++) {
                // ###
                // #*#
                // ###
                for(int y1 = y - 1; y1 <= y + 1; y1++) {
                    if(y1 == y && x1 == x) {
                        // Se è la cella stessa continua con il prossimo
                        continue;
                    }
                    if(game[wrap(x1,game.size())][wrap(y1,game[0].size())] == true) {
                        // Aumenta il numero di vicini
                        aliveCell++;
                    }
                }
            }

            if(aliveCell < 2) {
                newGame[x][y] = 0;
            }
            if((aliveCell == 2 || aliveCell == 3) && game[x][y]) {
                newGame[x][y] = 1;
            }
            if(aliveCell == 3 && !game[x][y]) {
                newGame[x][y] = 1;
            }
            if(aliveCell > 3) {
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

vector<string> split(const string &s, char delim)
{
    vector<string> result;
    stringstream ss(s);
    string item;

    while (getline(ss, item, delim)) {
        result.push_back(item);
    }
    return result;
}

VOID RunCommand()
{
    vector<string> args = split(std::string(command_line.begin(), command_line.end()), ' ');
    // Run
    if(args[0] == "run") {
        esegui = true;
        return;
    }
    // Stop
    if(args[0] == "stop") {
        esegui = false;
        return;
    }
    // Toggle
    if(args[0] == "t") {
        //stoi => string to int
        game[stoi(args[1])][stoi(args[2])] = !game[stoi(args[1])][stoi(args[2])];
        return;
    }
    // Save
    if(args[0] == "save") {
        if (args.size() != 2)
            return;
        ofstream file_stream;
        file_stream.open(args[1]);
        file_stream << game.size() << " " << game[0].size();
        for (int y = 0; y < game[0].size(); ++y) {
            file_stream << "\n";
            for (int x = 0; x < game.size(); ++x)
                file_stream << game[x][y];
        }
    }
    // Load
    if (args[0] == "load") {
        if (args.size() != 2)
            return;

        ifstream file_stream;
        file_stream.open(args[1]);
        int read_w, read_h;
        file_stream >> read_w >> read_h;
        for (int y = 0; y < read_h; ++y) {
            string curr_l;
            file_stream >> curr_l;
            for (int x = 0; x < read_w; ++x) {
                game[x][y] = curr_l[x] - '0';
            }
        }
    }
}

VOID ReadInput(HANDLE inputHandle)
{
    DWORD numberOfInputs;
    GetNumberOfConsoleInputEvents(inputHandle, &numberOfInputs);
    if (numberOfInputs <= 0)
        return;

    INPUT_RECORD irInBuf[128];
    ReadConsoleInput(inputHandle, irInBuf, 128, &numberOfInputs);

    for (int i = 0; i < numberOfInputs; ++i) {
        // Tasto premuto 
        if (irInBuf[i].EventType == KEY_EVENT && irInBuf[i].Event.KeyEvent.bKeyDown) {
            auto keyEvent = irInBuf[i].Event.KeyEvent;
            CHAR pressedKey = keyEvent.uChar.AsciiChar;
            WORD virtualKeyCode = keyEvent.wVirtualKeyCode;
            // Backspace premuto
            if (virtualKeyCode == VK_BACK) {
                if (command_line.size() > 0)
                    command_line.pop_back();
            }
            // Carattere alfanumerico inserito
            else if (isalnum(pressedKey) || virtualKeyCode == VK_SPACE) {
                if (command_line.size() < ConsoleWidth - 3)
                    command_line.push_back(pressedKey);
            }
            // Invio premuto
            else if (virtualKeyCode == VK_RETURN) {
                RunCommand();
                command_line.clear();
            }
        }
    }
}

int main()
{
    // Debug su file logs.txt
    freopen("logs.txt", "w", stdout);

    // buffer dello schermo
    wchar_t* screen = new wchar_t[ConsoleWidth * ConsoleHeight];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    // Titolo della finestra
    SetConsoleTitleA("Game Of Life");

    // Cursore invisibile
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    // Handle per l'input
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

    // Timing
    auto t1 = chrono::high_resolution_clock::now();
    auto t2 = chrono::high_resolution_clock::now();
    chrono::nanoseconds NanosecondsPerFrame((int)(1.f/(float)UpdatesPerSecond*1e9));

    chrono::nanoseconds Counter(0);

    // Game Loop
    while(1) {
        t1 = chrono::high_resolution_clock::now();

        // Pulizia buffer di output
        fill_n(screen, ConsoleWidth * ConsoleHeight, ' ');

        // Elaborazione degli Input
        ReadInput(hInput);

        // Update
        if (Counter >= NanosecondsPerFrame) {
            Counter = chrono::nanoseconds(0);
            if(esegui) {
                update();
            }
        }

        // Draw gioco
        for (int x = 0; x < game.size(); ++x) {
            for (int y = 0; y < game[0].size(); ++y) {
                if (game[x][y])
                    WriteToScreen(screen, x + 1, y + 1, '*');
            }
        }

        // Draw bordi
        for (int x = 0; x < ConsoleWidth; ++x) {
            WriteToScreen(screen, x, 0, '#');
            WriteToScreen(screen, x, ConsoleHeight-1, '#');
        }
        for (int y = 0; y < ConsoleHeight; ++y) {
            WriteToScreen(screen, 0, y, '|');
            WriteToScreen(screen, ConsoleWidth-1, y, '|');
        }

        // Draw linea di comando
        DrawCommandLine(screen);

        // Scrittura buffer di output
        screen[ConsoleWidth * ConsoleHeight - 1] = '\0';
        WriteConsoleOutputCharacterW(hConsole, screen, ConsoleWidth * ConsoleHeight, { 0,0 }, &dwBytesWritten);

        // Timing 
        Sleep(3);

        t2 = chrono::high_resolution_clock::now();
        std::chrono::nanoseconds delta_time = t2-t1;
        Counter+=delta_time;
    }

    return 0;
}
