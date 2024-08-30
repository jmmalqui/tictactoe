#include "mayaa.hpp"
#define WINWIDTH 640
#define WINHEIGHT 320
#define WINTITLE "tictactoe"

enum CellState {
    kEmpty,
    kCircle,
    kCross,
};

enum TEAM {
    kPlayer,
    kComputer,
};
void InitializeWindow(void)
{
    InitWindow(WINWIDTH, WINHEIGHT, WINTITLE);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
}

std::vector<int> MakeTable(void)
{
    std::vector<int> table;
    table.reserve(9);
    for (int i = 0; i < 9; i++) {
        table.push_back(kEmpty);
    }
    return table;
}

class MenuScene : public Scene {
    using Scene::Scene;

    void Update()
    {
        if (IsKeyPressed(KEY_ENTER))
            m_SceneManager->ChangeSceneTo("game");
    }
    void Render()
    {
        ClearBackground(BLUE);
        DrawText("TicTacToe", 10, (int)(GetScreenHeight() / 2) - 10, 20, WHITE);
        DrawText("Press enter to play", 10, (int)(GetScreenHeight() / 2) - 10 + 30, 20, GREEN);
    }
};

class GameScene : public Scene {
    using Scene::Scene;

    int m_WindowHeight = 0;
    int m_WindowWidth = 0;
    int m_TableSize = 100;

    Rectangle m_TableRec;
    std::vector<int> m_TableData = MakeTable();

    float m_ComputerMoveTime = 1.0f;
    bool m_ComputerMoving = false;
    float m_computerTimer = 0.0f;

    bool m_PlayerWin = false;
    bool m_ComputerWin = false;

    void RestartGame()
    {
    }

    void AddMoveToTable(CellState move, int x, int y)
    {
        m_TableData[y * 3 + x] = move;
    }
    Vector2 GetVectorIndexFromMousePress()
    {
        Vector2 vec;
        if (CheckCollisionPointRec(GetMousePosition(), m_TableRec)) {
            int clickRelX = GetMousePosition().x - m_TableRec.x;
            int clickRelY = GetMousePosition().y - m_TableRec.y;

            vec.x = (int)(3 * clickRelX / m_TableSize);
            vec.y = (int)(3 * clickRelY / m_TableSize);

            return vec;

        } else {
            vec.x = -1;
            vec.y = -1;
            return vec;
        }
    }

    bool CheckWinner(int lastMove, CellState cell)
    {
        int rowCheck = lastMove - lastMove % 3;
        int count = 0;
        for (int i = rowCheck; i < rowCheck + 3; i++) {
            if (m_TableData[i] == cell)
                count += 1;
        }
        if (count == 3) {
            return true;
        } else
            count = 0;
        int columnCheck = lastMove % 3;
        for (int i = 0; i < 3; i++) {
            if (m_TableData[columnCheck + i * 3] == cell)
                count += 1;
        }
        if (count == 3) {
            return true;
        } else
            count = 0;

        if (lastMove % 2 == 0) {
            std::vector<int> rightDiagonal = { 0, 4, 8 };
            for (int i = 0; i < 3; i++) {
                if (m_TableData[rightDiagonal[i]] == cell) {
                    count += 1;
                }
            }
            if (count == 3)
                return true;
            else
                count = 0;
            std::vector<int> leftDiagonal = { 2, 4, 6 };
            for (int i = 0; i < 3; i++) {
                if (m_TableData[leftDiagonal[i]] == cell) {
                    count += 1;
                }
            }
            if (count == 3)
                return true;
            else
                count = 0;
        }
        return false;
    }
    bool isVectorIndexValid(Vector2 vec)
    {
        return vec.x != -1 && vec.y != -1;
    }

    bool isGameTie()
    {
        int count = 0;
        for (int i = 0; i < 9; i++) {
            if (m_TableData[i] == kEmpty)
                count += 1;
        }
        if (count == 0 && (!m_PlayerWin || m_ComputerWin))
            return true;
        return false;
    }
    bool isGameOver()
    {
        return (m_PlayerWin || m_ComputerWin) || isGameTie();
    }

    void Update()
    {
        m_WindowHeight = GetScreenHeight();
        m_WindowWidth = GetScreenWidth();
        m_TableSize = m_WindowHeight < m_WindowWidth ? m_WindowHeight * 0.9 : m_WindowWidth * 0.9;

        int centerX = (int)(m_WindowWidth / 2);
        int centerY = (int)(m_WindowHeight / 2);

        m_TableRec.x = centerX - (int)(m_TableSize / 2);
        m_TableRec.y = centerY - (int)(m_TableSize / 2);
        m_TableRec.height = m_TableSize;
        m_TableRec.width = m_TableSize;

        Vector2 vectorIndex = GetVectorIndexFromMousePress();

        if (IsMouseButtonPressed(0) && !m_ComputerMoving && isVectorIndexValid(vectorIndex) && !isGameOver()) {
            int tableDataIndex = vectorIndex.x + vectorIndex.y * 3;
            if (m_TableData[tableDataIndex] == kEmpty) {
                m_TableData[tableDataIndex] = kCircle;
                if (CheckWinner(tableDataIndex, kCircle)) {
                    m_PlayerWin = true;
                } else {
                    m_ComputerMoving = true;
                }
            }
        }

        if (m_ComputerMoving) {
            m_computerTimer += GetFrameTime();
            if (m_computerTimer >= m_ComputerMoveTime) {
                m_ComputerMoving = false;
                m_computerTimer = 0;
                int lastCheck = -1;
                bool picked;
                for (int i = 0; i < 9; i++) {
                    if (m_TableData[i] != kCircle && m_TableData[i] != kCross) {
                        int value = GetRandomValue(0, 1);
                        lastCheck = i;
                        if (value) {
                            picked = true;
                            m_TableData[i] = kCross;
                            if (CheckWinner(i, kCross)) {
                                m_ComputerWin = true;
                            }
                            break;
                        }
                    }
                }
                if (!picked && lastCheck != -1) {
                    m_TableData[lastCheck] = kCross;
                    if (CheckWinner(lastCheck, kCross)) {
                        m_ComputerWin = true;
                    }
                }
            }
        }
    }
    void Render()
    {
        RenderBackground();
        RenderTable();
        if (isGameOver()) {
            if (m_ComputerWin) {
                DrawText("Computer win", 10, 10, 20, BLUE);
            } else if (m_PlayerWin) {
                DrawText("Player win", 10, 10, 20, RED);
            } else if (isGameTie())
                DrawText("Game reached a Tie", 10, 10, 20, ORANGE);
            else
                DrawText("Something went really, really wrong", 10, 10, 20, GREEN);
        }
    }
    void RenderBackground()
    {
        ClearBackground(RED);
        int squareTileSize = 60;

        int tileXNum = (int)floor(m_WindowWidth / squareTileSize) + 1;
        int tileYNum = (int)floor(m_WindowHeight / squareTileSize) + 1;

        for (int x = 0; x < tileXNum; x++) {
            for (int y = 0; y < tileYNum; y++) {
                Color backgroundTileColor = BLACK;
                if (m_ComputerMoving)
                    backgroundTileColor = BLACK;
                if ((x + y) % 2 == 0)
                    if (m_ComputerMoving)
                        backgroundTileColor = DARKBLUE;
                    else
                        backgroundTileColor = DARKGRAY;
                DrawRectangle(x * squareTileSize, y * squareTileSize, squareTileSize, squareTileSize, backgroundTileColor);
            }
        }
    }
    void RenderTable()
    {
        DrawRectangleGradientV(m_TableRec.x, m_TableRec.y, m_TableSize, m_TableSize, WHITE, LIGHTGRAY);
        for (int i = 0; i < (int)m_TableData.size(); i++) {
            int cellSize = (int)(m_TableSize / 3);
            int xIndex = i % 3;
            int yIndex = (int)(i / 3);
            int xCoord = m_TableRec.x + (int)(cellSize / 2) + cellSize * xIndex;
            int yCoord = m_TableRec.y + (int)(cellSize / 2) + cellSize * yIndex;
            if (m_TableData[i] == kCircle)
                DrawCircle(xCoord, yCoord, (float)(cellSize * 0.4), RED);
            if (m_TableData[i] == kCross)
                DrawCircle(xCoord, yCoord, (float)(cellSize * 0.4), BLUE);
        }
        DrawRectangleLinesEx(m_TableRec, 5, BLACK);
    }
};

int main(int argc, char const* argv[])
{
    InitializeWindow();
    SetTargetFPS(60);
    SceneManager sceneManager;
    GameScene game("game", &sceneManager);
    MenuScene scene("scene", &sceneManager);
    sceneManager.ChangeSceneTo("scene");
    while (!WindowShouldClose()) {
        BeginDrawing();
        sceneManager.Run();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
