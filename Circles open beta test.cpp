#include <iostream>
#include "TXLib.h"

const float Dt = 0.05;
const float K = 0.005;   // коэф. сопротивления
const float G = 0.01;

void drawCircle(int X, int Y, int R, int Red, int Green, int Blue)  // координаты, радиус, цвета в RGB
{
    int staticR = R;

    for(;R > 0; R = R - 2)
        {
            int Redf = Red - Red*0.9*R/staticR;
            int Greenf = Green - Green*0.9*R/staticR;
            int Bluef = Blue - Blue*0.9*R/staticR;

            txSetColor(RGB( Redf, Greenf , Bluef ));    // контур
            txSetFillColor(RGB( Red - Red*0.9*R/staticR, Green - Green*0.9*R/staticR ,Blue - Blue*0.9*R/staticR )); // цвет для следующего заполнения
            txCircle( X , Y , R);

        }
}


void moveCircle(float* X, float* Y, float* Vx, float* Vy, float R, float Dt)
{
    if ( *Y > (600-R) or *Y < R)
        *Vy = -*Vy;
    if ( *X > (800-R) or *X < R)
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


void addResistance(float* Vx, float* Vy, float K)    // добавляет сопротивление среды
{
    *Vx = *Vx - K * *Vx;
    *Vy = *Vy - K * *Vy;
}

void kickCircle(float X1, float Y1, float* Vx1, float* Vy1, float R1, float X2, float Y2, float* Vx2, float* Vy2, float R2)
{
    if ( (X1-X2)*(X1-X2) + (Y1-Y2)*(Y1-Y2) <= (R1+R2)*(R1+R2))
        {
            float Cx2 = *Vx2;
            float Cy2 = *Vy2;

            *Vx2 = *Vx1;
            *Vy2 = *Vy1;

            *Vx1 = Cx2;
            *Vy1 = Cy2;
        }
}
void kickCircleByObject(float X1, float Y1, float Vx1, float Vy1, float R1, float X2, float Y2, float* Vx2, float* Vy2, float R2, bool* ExistFactor, float* WinFactor) // координаты, скорости, радиус объекта окр., потом тоже самое "мяча"
{
    if (*ExistFactor)
    {
        if ( (X1-X2)*(X1-X2) + (Y1-Y2)*(Y1-Y2) <= (R1+R2)*(R1+R2))
            {
                *Vx2 = -1 * *Vx2 + Vx1;
                *Vy2 = *Vy2 + Vy1;

                *ExistFactor = false; // если false, то в дальнейшем объект исчезает и перестаёт обрабат.
                *WinFactor += 1;      // увеличиваем очки до победы
            }
     }
}
int main()
{
    txCreateWindow(800,600);

    float WinFactor = 0;

    float X1 = 100;     // игрок
    float Y1 = 300;
    float Vx1 = 0;
    float Vy1 = 0;

    float R = 50;

    float X2 = 300;    // мяч
    float Y2 = 300;
    float Vx2 = 0;
    float Vy2 = 0;

    float X3 = 650;    // объект 1
    float Y3 = 300;
    float Vx3 = 0;
    float Vy3 = 15;
    bool ExistFactor1 = true;

    float X4 = 750;    // объект 2
    float Y4 = 400;
    float Vx4 = 0;
    float Vy4 = 30;
    bool ExistFactor2 = true;

    float X5 = 550;    // объект 3
    float Y5 = 50;
    float Vx5 = 0;
    float Vy5 = 0;
    bool ExistFactor3 = true;

    float X6 = 500;    // объект 4
    float Y6 = 250;
    float Vx6 = 0;
    float Vy6 = 0;
    bool ExistFactor4 = true;

    float X7 = 525;    // объект 5
    float Y7 = 450;
    float Vx7 = 0;
    float Vy7 = 0;
    bool ExistFactor5 = true;











    for(;;)
        {
            controlCircle(&Vx1, &Vy1);                 // игрок
            moveCircle(&X1, &Y1, &Vx1, &Vy1, R, Dt);
            addResistance(&Vx1, &Vy1, 0.7*K);

            moveCircle(&X2, &Y2, &Vx2, &Vy2, R, Dt);  // "мяч"
            addResistance(&Vx2, &Vy2, 0.1*K);

            moveCircle(&X3, &Y3, &Vx3, &Vy3, R, Dt); // объект 1
            moveCircle(&X4, &Y4, &Vx4, &Vy4, R, Dt); // объект 2
            moveCircle(&X5, &Y5, &Vx5, &Vy5, R, Dt); // объект 3
            moveCircle(&X6, &Y6, &Vx6, &Vy6, R, Dt); // объект 4
            moveCircle(&X7, &Y7, &Vx7, &Vy7, R, Dt); // объект 5



            kickCircle(X1, Y1, &Vx1, &Vy1, R, X2, Y2, &Vx2, &Vy2, R);
            kickCircleByObject(X3, Y3, Vx3, Vy3, R, X2, Y2, &Vx2, &Vy2, R, &ExistFactor1, &WinFactor);
            kickCircleByObject(X4, Y4, Vx4, Vy4, R, X2, Y2, &Vx2, &Vy2, R, &ExistFactor2, &WinFactor);
            kickCircleByObject(X5, Y5, Vx5, Vy5, R, X2, Y2, &Vx2, &Vy2, R, &ExistFactor3, &WinFactor);
            kickCircleByObject(X6, Y6, Vx6, Vy6, R, X2, Y2, &Vx2, &Vy2, R, &ExistFactor4, &WinFactor);
            kickCircleByObject(X7, Y7, Vx7, Vy7, R, X2, Y2, &Vx2, &Vy2, R, &ExistFactor5, &WinFactor);


            txFillColor(0,0,0);
            txClear();

            drawCircle( X1, Y1, R, 255, 50, 50);
            drawCircle( X2, Y2, R, 0, 255, 0);

            if ( ExistFactor1 )
                drawCircle( X3, Y3, R, 100, 0, 255);   // объект 1

            if ( ExistFactor2 )
                drawCircle( X4, Y4, R, 100, 0, 255);   // объект 2

            if ( ExistFactor3 )
                drawCircle( X5, Y5, R, 100, 0, 255);   // объект 3

            if ( ExistFactor4 )
                drawCircle( X6, Y6, R, 100, 0, 255);   // объект 4

            if ( ExistFactor5 )
                drawCircle( X7, Y7, R, 100, 0, 255);   // объект 5

            txDrawText( 1, 10, 500, 50, " Don't touch left border");
            txDrawText( 1, 41, 500, 70, " Destroy blue balls");


            if ( WinFactor == 5 )     // победа
                {
                 txDrawText( 300, 200, 500, 400, "You Win!");
                 txSleep(10000000);
                }
            if ( X2 <= R )    // поражение при прикосновении к левой границе
                {
                 txDrawText( 300, 200, 500, 400, "You lose!");
                 txSleep(10000000);
                }


            txSleep(Dt);


            //Vy = Vy + G;


        }

    return 0;
}

