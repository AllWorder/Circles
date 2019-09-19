#include <iostream>
#include "TXLib.h"

const float Dt = 0.05;
const float RESISTANCE_COEF = 0.005;
const float G = 0.01;
const int SCREEN_Y = 600;
const int SCREEN_X = 800;

void drawCircle(int X, int Y, int R, int Red, int Green, int Blue)  // координаты, радиус, цвета в RGB
{
    COLORREF Color = txGetFillColor();
    for(int i = R;i > 0; i = i - 2)
        {
            int Redf = Red - Red*0.9*i/R;
            int Greenf = Green - Green*0.9*i/R;
            int Bluef = Blue - Blue*0.9*i/R;

            txSetColor(RGB( Redf, Greenf , Bluef ));
            txSetFillColor(RGB( Redf, Greenf ,Bluef ));
            txCircle( X , Y , i);
        }
    txSetFillColor( Color);
}


void moveCircle(float* X, float* Y, float* Vx, float* Vy, float R, float Dt, int SCREEN_Y, int SCREEN_X)
{
    if ( *Y > (SCREEN_Y-R) or *Y < R)
        *Vy = -*Vy;
    if ( *X > (SCREEN_X-R) or *X < R)
        *Vx = -*Vx;

    *X += *Vx * Dt;
    *Y += *Vy * Dt;

}


void controlCircle(float* Vx, float* Vy)
{
    if (GetAsyncKeyState(VK_LEFT))
        *Vx = *Vx - 0.35;
    if (GetAsyncKeyState(VK_RIGHT))
        *Vx = *Vx + 0.35;
    if (GetAsyncKeyState(VK_UP))
        *Vy = *Vy - 0.35;
    if (GetAsyncKeyState(VK_DOWN))
        *Vy = *Vy + 0.35;
}


void addResistance(float* Vx, float* Vy, float K)
{
    *Vx = *Vx - K * *Vx;
    *Vy = *Vy - K * *Vy;
}

bool checkCollision(float X1, float Y1, float X2, float Y2, float R1, float R2)
{
    if ( (X1-X2)*(X1-X2) + (Y1-Y2)*(Y1-Y2) <= (R1+R2)*(R1+R2))
        return true;
    else
        return false;
}

void kickCircle(float X1, float Y1, float* Vx1, float* Vy1, float R1, float X2, float Y2, float* Vx2, float* Vy2, float R2)
{
    if (checkCollision(X1, Y1, X2, Y2, R1, R2))
        {
            float CopyVx2 = *Vx2;
            float CopyVy2 = *Vy2;

            *Vx2 = *Vx1;
            *Vy2 = *Vy1;

            *Vx1 = CopyVx2;
            *Vy1 = CopyVy2;
        }
}
void kickCircleByObject(float X1, float Y1, float Vx1, float Vy1, float R1, float X2, float Y2, float* Vx2, float* Vy2, float R2, bool* ExistFactor, float* WinFactor) // координаты, скорости, радиус объекта окр., потом тоже самое "м€ча"
{
    if (*ExistFactor)
    {
        if (checkCollision(X1, Y1, X2, Y2, R1, R2))
            {
                *Vx2 = -1 * *Vx2 + Vx1;
                *Vy2 = *Vy2 + Vy1;

                *ExistFactor = false; // если false, то в дальнейшем объект исчезает и перестаЄт обрабат.
                *WinFactor += 1;      // увеличиваем очки до победы
            }
     }
}
void endGame(int WinFactor, float BallX, float R)
{
    if ( WinFactor == 5 )
        {
         txDrawText( 300, 200, 500, 400, "You Win!");
         txSleep(10000000);
        }
    if ( BallX <= R )
        {
         txDrawText( 300, 200, 500, 400, "You lose!");
         txSleep(10000000);
        }
}
int main()
{
    txCreateWindow(800,600);

    float WinFactor = 0;

    float PlayerX = 100;
    float PlayerY = 300;
    float PlayerVx = 0;
    float PlayerVy = 0;

    float R = 50;

    float BallX = 300;
    float BallY = 300;
    float BallVx = 0;
    float BallVy = 0;

    float ObX1 = 650;
    float ObY1 = 300;
    float ObVx1 = 0;
    float ObVy1 = 15;
    bool ExistFactor1 = true;

    float ObX2 = 750;
    float ObY2 = 400;
    float ObVx2 = 0;
    float ObVy2 = 30;
    bool ExistFactor2 = true;

    float ObX3 = 550;
    float ObY3 = 50;
    float ObVx3 = 0;
    float ObVy3 = 0;
    bool ExistFactor3 = true;

    float ObX4 = 500;
    float ObY4 = 250;
    float ObVx4 = 0;
    float ObVy4 = 0;
    bool ExistFactor4 = true;

    float ObX5 = 525;
    float ObY5 = 450;
    float ObVx5 = 0;
    float ObVy5 = 0;
    bool ExistFactor5 = true;

    txFillColor(0,0,0);

    for(;;)
        {
            controlCircle(&PlayerVx, &PlayerVy);
            moveCircle(&PlayerX, &PlayerY, &PlayerVx, &PlayerVy, R, Dt, SCREEN_Y, SCREEN_X);
            addResistance(&PlayerVx, &PlayerVy, 0.7*RESISTANCE_COEF);

            moveCircle(&BallX, &BallY, &BallVx, &BallVy, R, Dt, SCREEN_Y, SCREEN_X);
            addResistance(&BallVx, &BallVy, 0.1*RESISTANCE_COEF);

            moveCircle(&ObX1, &ObY1, &ObVx1, &ObVy1, R, Dt, SCREEN_Y, SCREEN_X);
            moveCircle(&ObX2, &ObY2, &ObVx2, &ObVy2, R, Dt, SCREEN_Y, SCREEN_X);
            moveCircle(&ObX3, &ObY3, &ObVx3, &ObVy3, R, Dt, SCREEN_Y, SCREEN_X);
            moveCircle(&ObX4, &ObY4, &ObVx4, &ObVy4, R, Dt, SCREEN_Y, SCREEN_X);
            moveCircle(&ObX5, &ObY5, &ObVx5, &ObVy5, R, Dt, SCREEN_Y, SCREEN_X);

            kickCircle(PlayerX, PlayerY, &PlayerVx, &PlayerVy, R, BallX, BallY, &BallVx, &BallVy, R);
            kickCircleByObject(ObX1, ObY1, ObVx1, ObVy1, R, BallX, BallY, &BallVx, &BallVy, R, &ExistFactor1, &WinFactor);
            kickCircleByObject(ObX2, ObY2, ObVx2, ObVy2, R, BallX, BallY, &BallVx, &BallVy, R, &ExistFactor2, &WinFactor);
            kickCircleByObject(ObX3, ObY3, ObVx3, ObVy3, R, BallX, BallY, &BallVx, &BallVy, R, &ExistFactor3, &WinFactor);
            kickCircleByObject(ObX4, ObY4, ObVx4, ObVy4, R, BallX, BallY, &BallVx, &BallVy, R, &ExistFactor4, &WinFactor);
            kickCircleByObject(ObX5, ObY5, ObVx5, ObVy5, R, BallX, BallY, &BallVx, &BallVy, R, &ExistFactor5, &WinFactor);

            txClear();

            drawCircle( PlayerX, PlayerY, R, 255, 50, 50);
            drawCircle( BallX, BallY, R, 0, 255, 0);

            if ( ExistFactor1 )
                drawCircle( ObX1, ObY1, R, 100, 0, 255);

            if ( ExistFactor2 )
                drawCircle( ObX2, ObY2, R, 100, 0, 255);

            if ( ExistFactor3 )
                drawCircle( ObX3, ObY3, R, 100, 0, 255);

            if ( ExistFactor4 )
                drawCircle( ObX4, ObY4, R, 100, 0, 255);

            if ( ExistFactor5 )
                drawCircle( ObX5, ObY5, R, 100, 0, 255);

            txDrawText( 1, 10, 500, 50, " Don't touch left border");
            txDrawText( 1, 41, 500, 70, " Destroy blue balls");

            endGame( WinFactor, BallX, R);

            txSleep(Dt);
        }

    return 0;
}

