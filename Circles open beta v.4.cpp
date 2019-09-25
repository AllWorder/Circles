#include <iostream>
#include "TXLib.h"

const float Dt = 0.05;
const float RESISTANCE_COEF = 0.005;
const float G = 0.01;
const int SCREEN_Y = 600;
const int SCREEN_X = 800;

struct Circle
{
    float X = 0;
    float Y = 0;

    float Vx = 0;
    float Vy = 0;

    float R = 50;

    int Red = 255;
    int Green = 255;
    int Blue = 255;

    bool ExistFactor = true;
} ;

void drawCircle(Circle Ball )
{
    COLORREF Color = txGetFillColor();
    for(int i = Ball.R;i > 0; i = i - 2)
        {
            int Redf = Ball.Red - Ball.Red*0.9*i/Ball.R;
            int Greenf = Ball.Green - Ball.Green*0.9*i/Ball.R;
            int Bluef = Ball.Blue - Ball.Blue*0.9*i/Ball.R;

            txSetColor(RGB( Redf, Greenf , Bluef ));
            txSetFillColor(RGB( Redf, Greenf ,Bluef ));
            txCircle( Ball.X , Ball.Y , i);
        }
    txSetFillColor( Color);
}


void moveCircle(Circle* Ball, float Dt, int SCREEN_Y, int SCREEN_X)
{
    if ( Ball->Y > (SCREEN_Y-Ball->R) or Ball->Y < Ball->R)
        Ball->Vy = -Ball->Vy;
    if ( Ball->X > (SCREEN_X-Ball->R) or Ball->X < Ball->R)
        Ball->Vx = -Ball->Vx;

    Ball->X += Ball->Vx * Dt;
    Ball->Y += Ball->Vy * Dt;

}


void controlCircle(Circle* Ball)
{
    if (GetAsyncKeyState(VK_LEFT))
        Ball->Vx = Ball->Vx - 0.35;
    if (GetAsyncKeyState(VK_RIGHT))
        Ball->Vx = Ball->Vx + 0.35;
    if (GetAsyncKeyState(VK_UP))
        Ball->Vy = Ball->Vy - 0.35;
    if (GetAsyncKeyState(VK_DOWN))
        Ball->Vy = Ball->Vy + 0.35;
}


void addResistance(Circle* Ball, float K)
{
    Ball->Vx = Ball->Vx - K * Ball->Vx;
    Ball->Vy = Ball->Vy - K * Ball->Vy;
}

bool checkCollision(Circle Ball1, Circle Ball2)
{
    if ( (Ball1.X-Ball2.X)*(Ball1.X-Ball2.X) + (Ball1.Y-Ball2.Y)*(Ball1.Y-Ball2.Y) <= (Ball1.R+Ball2.R)*(Ball1.R+Ball2.R))
        return true;
    else
        return false;
}

void kickCircle(Circle* Ball1, Circle* Ball2)
{
    if (checkCollision(*Ball1, *Ball2))
        {
            float CopyVx2 = Ball2->Vx;
            float CopyVy2 = Ball2->Vy;

            Ball2->Vx = Ball1->Vx;
            Ball2->Vy = Ball1->Vy;

            Ball1->Vx = CopyVx2;
            Ball1->Vy = CopyVy2;
        }
}
void kickCircleByObject(Circle* Obj, Circle* Ball, float* WinFactor)
{
    if (Obj->ExistFactor)
    {
        if (checkCollision(*Ball, *Obj))
            {
                Ball->Vx = -1 * Ball->Vx + Obj->Vx;
                Ball->Vy = Ball->Vy + Obj->Vy;

                Obj->ExistFactor = false; // about existing on screen and in process
                *WinFactor += 1;
            }
     }
}
void endGame(int WinFactor, Circle Ball)
{
    if ( WinFactor == 5 )
        {
         txDrawText( 300, 200, 500, 400, "You Win!");
         txSleep(10000000);
        }
    if ( Ball.X <= Ball.R )
        {
         txDrawText( 300, 200, 500, 400, "You lose!");
         txSleep(10000000);
        }
}
int main()
{
    txCreateWindow(800,600);

    float WinFactor = 0;

    Circle Player;
    Player.X = 100;
    Player.Y = 300;
    Player.Green = 0;
    Player.Blue = 0;

    Circle Ball;
    Ball.X = 300;
    Ball.Y = 300;
    Ball.Red = 50;
    Ball.Blue = 50;

    Circle Obj1;
    Obj1.X = 650;
    Obj1.Y = 300;
    Obj1.Vy = 15;

    Circle Obj2;
    Obj2.X = 750;
    Obj2.Y = 400;
    Obj2.Vy = 30;

    Circle Obj3;
    Obj3.X = 550;
    Obj3.Y = 50;

    Circle Obj4;
    Obj4.X = 500;
    Obj4.Y = 250;

    Circle Obj5;
    Obj5.X = 525;
    Obj5.Y = 450;

    txFillColor(0,0,0);

    for(;;)
        {
            controlCircle(&Player);
            moveCircle(&Player, Dt, SCREEN_Y, SCREEN_X);
            addResistance(&Player, 0.7*RESISTANCE_COEF);

            moveCircle(&Ball, Dt, SCREEN_Y, SCREEN_X);
            addResistance(&Ball, 0.1*RESISTANCE_COEF);

            moveCircle(&Obj1, Dt, SCREEN_Y, SCREEN_X);
            moveCircle(&Obj2, Dt, SCREEN_Y, SCREEN_X);
            moveCircle(&Obj3, Dt, SCREEN_Y, SCREEN_X);
            moveCircle(&Obj4, Dt, SCREEN_Y, SCREEN_X);
            moveCircle(&Obj5, Dt, SCREEN_Y, SCREEN_X);

            kickCircle(&Player, &Ball);
            kickCircleByObject(&Obj1, &Ball, &WinFactor);
            kickCircleByObject(&Obj2, &Ball, &WinFactor);
            kickCircleByObject(&Obj3, &Ball, &WinFactor);
            kickCircleByObject(&Obj4, &Ball, &WinFactor);
            kickCircleByObject(&Obj5, &Ball, &WinFactor);

            txClear();

            drawCircle(Player);
            drawCircle(Ball);

            if ( Obj1.ExistFactor )
                drawCircle(Obj1);

            if ( Obj2.ExistFactor )
                drawCircle(Obj2);

            if ( Obj3.ExistFactor )
                drawCircle(Obj3);

            if ( Obj4.ExistFactor )
                drawCircle(Obj4);

            if ( Obj5.ExistFactor )
                drawCircle(Obj5);

            txDrawText( 1, 10, 500, 50, " Don't touch left border");
            txDrawText( 1, 41, 500, 70, " Destroy white balls");

            endGame( WinFactor, Ball);

            txSleep(Dt);
        }

    return 0;
}

