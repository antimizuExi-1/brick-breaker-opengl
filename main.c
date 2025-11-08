#include "brick/Macro.h"
#include "brick/Utils.h"
#include "brick/Window.h"
#include "brick/shape/Circle.h"
#include "brick/shape/Rectangle.h"

const int width = 1200;
const int height = 800;

BrkRectangle bricks[12][6];

void Init_Map(int bricksW, int bricksH);

int main(void)
{
    Brk_Window_Init(width, height, "brick breaker");

    BrkCamera2D camera = Brk_Camera_Create((vec2){0.0f, 0.0f}, (float) width, (float) height);

    BrkRectangle bat = Brk_Rectangle_Create((vec2){600.0f, 680.0f}, 200.0f, 30.0f);
    BrkCircle ball = Brk_Circle_Create((vec2){610.0f, 500.0f}, 15.0f);

    const int bricksW = 12;
    const int bricksH = 6;
    Init_Map(bricksW, bricksH);

    float speedX = 0.0f;
    float speedY = 5.0f;

    while (!Brk_Window_ShouldClose())
    {
        // event
        Brk_Window_PollEvent();
        if (Brk_Window_KeyPressed('A'))
        {
            bat.position[0] -= 5.0f;
        } else if (Brk_Window_KeyPressed('D'))
        {
            bat.position[0] += 5.0f;
        }
        if (Brk_Window_KeyPressed('R'))
        {
            Init_Map(bricksW, bricksH);
            speedX = 0.0f;
            ball.position[0] = 610.0f;
            ball.position[1] = 500.0f;
            bat.position[0] = 600.0f;
            bat.position[1] = 680.0f;
        }

        // ball collision player bat
        if (Brk_Rectangle_CheckCollision(bat, ball.position))
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
                }
            }
        }
        // ball collision window border
        if (ball.position[0] > (float) width || ball.position[0] < 0)
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

        // draw
        Brk_Window_ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // draw bricks
        for (int i = 0; i < bricksH; i++)
        {
            for (int j = 0; j < bricksW; j++)
            {
                if (j % 2 == i % 2)
                    Brk_Rectangle_Draw(bricks[j][i], Brk_GRAY, camera);
                Brk_Rectangle_Draw(bricks[j][i], Brk_LIGHTGRAY, camera);
            }
        }

        Brk_Rectangle_Draw(bat, Brk_BLUE, camera);
        Brk_Circle_Draw(ball, Brk_WHITE, camera);

        Brk_Window_SwapBuffer();
    }
    Brk_Window_Close();
    return 0;
}

void Init_Map(int bricksW, int bricksH)
{
    for (int i = 0; i < bricksH; i++)
    {
        for (int j = 0; j < bricksW; j++)
        {
            bricks[j][i] = Brk_Rectangle_Create((vec2){j * 100 + 50, i * 30 + 15}, 100, 30);
        }
    }
}
