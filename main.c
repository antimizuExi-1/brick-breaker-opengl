#include "brick/Circle.h"
#include "brick/Font.h"
#include "brick/Macro.h"
#include "brick/Rectangle.h"
#include "brick/Sprite.h"
#include "brick/Window.h"
#include "brick/Shape.h"

const int width = 1200;
const int height = 800;

BrkRectangle bricks[12][6];

void Init_Map(int bricksW, int bricksH);

int main(void)
{
    Brk_Window_Init(width, height, "brick breaker");

    BrkCamera2D camera = Brk_Camera_Create((BrkVec2){0.0f, 0.0f}, (float)width, (float)height);

    BrkCircle ball = Brk_Circle_Create((BrkVec2){610.0f, 500.0f}, 10.0f);

    BrkFont font = {0};
    if (Brk_Font_Load(&font, "../res/arial.ttf"))
    {
        Brk_Text_LoadCharacterSet(font);
    }
    BrkSprite bat = {0};
    Brk_Sprite_Load(&bat, "../res/bat.jpg", (BrkVec2){600.0f, 680.0f}, (BrkVec2){200.0f, 30.0f});

    const int bricksW = 12;
    const int bricksH = 6;
    Init_Map(bricksW, bricksH);

    float speedX = 0.0f;
    float speedY = 5.0f;

    int score = 0;
    char score_str[20] = {0};
    sprintf(score_str, "Score:%d", score);

    while (!Brk_Window_ShouldClose())
    {
        // event
        Brk_Window_PollEvent();
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
        if (Brk_Window_KeyPressed('R'))
        {
            Init_Map(bricksW, bricksH);
            speedX = 0.0f;
            ball.position[0] = 610.0f;
            ball.position[1] = 500.0f;
            bat.position[0] = 600.0f;
            bat.position[1] = 680.0f;

            score = 0;
            sprintf(score_str, "Score:%d", score);
        }

        // ball collision player bat
        if (Brk_Rectangle_CheckCollision(
                (BrkRectangle)
                {
                    bat.position[0], bat.position[1],
                    bat.size[0], bat.size[1]
                },
                ball.position
            )
        )
        {
            speedX = (ball.position[0] - bat.position[0]) / 20;
            speedY = -speedY;
        }
        // ball collision brick
        for (int i = 0; i < bricksH; i++)
        {
            for (int j = 0; j < bricksW; j++)
            {
                if (Brk_Rectangle_CheckCollision(bricks[j][i], ball.position))
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
        ball.position[0] += speedX;
        ball.position[1] += speedY;

        int w, h;
        Brk_Window_GetSize(&w, &h);
        Brk_Camera_UpdateSize(&camera, w, h);

        // draw
        Brk_Window_ClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // draw bricks
        for (int i = 0; i < bricksH; i++)
        {
            for (int j = 0; j < bricksW; j++)
            {
                if (j % 2 == i % 2)
                    Brk_Rectangle_Draw(bricks[j][i], Brk_GRAY, camera);
                else
                    Brk_Rectangle_Draw(bricks[j][i], Brk_LIGHTGRAY, camera);
            }
        }

        Brk_Circle_Draw(ball, Brk_WHITE, camera);
        Brk_Sprite_Draw(bat, camera);
        Brk_Text_Draw(score_str, Brk_WHITE, (BrkVec2){1000.0f, 100.0f}, 0.5f, camera);

        Brk_Window_SwapBuffer();
    }
    Brk_Window_Close();
    Brk_Sprite_Unload(bat);
    Brk_Font_Unload(font);
    return 0;
}

void Init_Map(int bricksW, int bricksH)
{
    for (int i = 0; i < bricksH; i++)
    {
        for (int j = 0; j < bricksW; j++)
        {
            bricks[j][i] = Brk_Rectangle_Create((BrkVec2){j * 100 + 50, i * 30 + 15}, (BrkVec2){100.0f, 30.0f});
        }
    }
}
