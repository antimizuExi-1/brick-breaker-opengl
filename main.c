#include "brick/Font.h"
#include "brick/Macro.h"
#include "brick/Sprite.h"
#include "brick/Window.h"
#include "brick/Renderer.h"

const int width = 1200;
const int height = 800;

BrkRectangle bricks[12][6] = {0};

void Init_Map(int bricksW, int bricksH);

bool checkSuccessful(int bricksW, int bricksH);

int main(void)
{
    Brk_Window_Init(width, height, "brick breaker");

    BrkCircle ball = {.position = {610.0f, 500.0f}, .radius = 10.0f};
    BrkFont font = {0};
    if (Brk_Font_Load(&font, "../res/arial.ttf"))
    {
        Brk_Text_LoadCharacterSet(font);
    }
    BrkSprite bat = {0};
    Brk_Sprite_CreateFromImg(&bat, "../res/bat.jpg", (BrkVec2){600.0f, 680.0f}, (BrkVec2){200.0f, 30.0f});

    const int bricksW = 12;
    const int bricksH = 6;
    Init_Map(bricksW, bricksH);

    float speedX = 0.0f;
    float speedY = 5.0f;
    int score = 0;
    char score_str[20] = {0};
    sprintf(score_str, "Score:%d", score);
    bool over = false;
    bool success = false;
    while (!Brk_Window_ShouldClose())
    {
        // event
        Brk_Window_PollEvent();

        // player controller
        if (!over && !success)
        {
            if (Brk_Window_KeyPressed('A'))
            {
                bat.position[0] -= 5.0f;
                if (bat.position[0] < 100.0f)
                {
                    bat.position[0] += 5.0f;
                }
            }
            else if (Brk_Window_KeyPressed('D'))
            {
                bat.position[0] += 5.0f;
                if (bat.position[0] > width - 100.0f)
                {
                    bat.position[0] -= 5.0f;
                }
            }
        }
        if (Brk_Window_KeyPressed('R'))
        {
            Init_Map(bricksW, bricksH);
            speedX = 0.0f;
            speedY = 5.0f;
            ball.position[0] = 610.0f;
            ball.position[1] = 500.0f;
            bat.position[0] = 600.0f;
            bat.position[1] = 680.0f;
            over = false;
            success = false;
            score = 0;
            sprintf(score_str, "Score:%d", score);
        }
        // ball collision player bat
        float batWidth = bat.size[0];
        float batHeight = bat.size[1];
        if (ball.position[0] >= bat.position[0] - batWidth / 2 &&
            ball.position[0] <= bat.position[0] + batWidth / 2 &&
            ball.position[1] >= bat.position[1] - batHeight / 2 &&
            ball.position[1] <= bat.position[1] + batHeight / 2)
        {
            speedX = (ball.position[0] - bat.position[0]) / 20;
            speedY = -speedY;
        }
        // ball collision brick
        for (int i = 0; i < bricksH; i++)
        {
            for (int j = 0; j < bricksW; j++)
            {
                float rectWidth = bricks[j][i].size[0];
                float rectHeight = bricks[j][i].size[1];
                if (ball.position[0] >= bricks[j][i].position[0] - rectWidth / 2 &&
                    ball.position[0] <= bricks[j][i].position[0] + rectWidth / 2 &&
                    ball.position[1] >= bricks[j][i].position[1] - rectHeight / 2 &&
                    ball.position[1] <= bricks[j][i].position[1] + rectHeight / 2)
                {
                    bricks[j][i].size[0] = 0;
                    bricks[j][i].size[1] = 0;
                    speedX = (ball.position[0] - bricks[j][i].position[0]) / 20;
                    speedY = -speedY;
                    score++;
                    sprintf(score_str, "Score:%d", score);
                }
            }
        }
        // ball collision window border
        if (ball.position[0] > (float)width || ball.position[0] < 0)
        {
            speedX = -speedX;
        }
        if (ball.position[1] < 0)
        {
            speedY = -speedY;
        }

        // update ball position
        if (!over && !success)
        {
            ball.position[0] += speedX;
            ball.position[1] += speedY;
        }

        if (ball.position[brkY] > height)
        {
            over = true;
        }
        success = checkSuccessful(bricksW, bricksH);

        // draw
        Brk_Window_ClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        Brk_Renderer_NewFrame();
        // draw bricks
        for (int i = 0; i < bricksH; i++)
        {
            for (int j = 0; j < bricksW; j++)
            {
                if (j % 2 == i % 2)
                    Brk_Renderer_DrawRect(bricks[j][i], Brk_GRAY);
                else
                    Brk_Renderer_DrawRect(bricks[j][i], Brk_LIGHTGRAY);
            }
        }

        Brk_Renderer_DrawCircle(ball, Brk_WHITE);
        Brk_Renderer_DrawSprite(bat);
        Brk_Renderer_DrawText(score_str, Brk_WHITE, (BrkVec2){1000.0f, 100.0f}, 0.5f);

        if (over)
        {
            Brk_Renderer_DrawText("Game Over", Brk_WHITE, (BrkVec2){500.0f, 400.0f}, 1.0f);
            Brk_Renderer_DrawText("press the r to restart", Brk_WHITE, (BrkVec2){480.0f, 450.0f}, 0.7f);
        }
        if (success)
        {
            Brk_Renderer_DrawText("You Win", Brk_WHITE, (BrkVec2){500.0f, 400.0f}, 1.0f);
            Brk_Renderer_DrawText("press the r to restart", Brk_WHITE, (BrkVec2){480.0f, 450.0f}, 0.7f);
        }

        Brk_Window_SwapBuffer();
    }
    Brk_Window_Close();
    Brk_Sprite_Destroy(bat);
    Brk_Text_UnloadCharacterSet();
    Brk_Font_Unload(font);
    return 0;
}

void Init_Map(int bricksW, int bricksH)
{
    for (int i = 0; i < bricksH; i++)
    {
        for (int j = 0; j < bricksW; j++)
        {
            bricks[j][i].position[brkX] = j * 100 + 50;
            bricks[j][i].position[brkY] = i * 30 + 15;
            bricks[j][i].size[brkWidth] = 100.0f;
            bricks[j][i].size[brkHeight] = 30.0f;
        }
    }
}

bool checkSuccessful(int bricksW, int bricksH)
{
    int x = 0, y = 0;
    for (x = 0; x < bricksH; x++)
    {
        for (y = 0; y < bricksW; y++)
        {
            if (bricks[y][x].size[0] > 0.1f)
            {
                return false;
            }
        }
    }
    return true;
}