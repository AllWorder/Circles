#include <iostream>
#include "TXLib.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>

const float Dt = 0.05;
const float RESISTANCE_COEF = 0.005;
const float G = 0.01;
const int SCREEN_Y = 600;
const int SCREEN_X = 800;

struct vector2f
{
    float x = 0;
    float y = 0;
} ;

vector2f add(vector2f v1, vector2f v2)
{
    vector2f result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    return result;
}

vector2f sub(vector2f v1, vector2f v2)   // first minus second
{
    vector2f result;
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    return result;
}

vector2f mul(vector2f v1, float a)
{
    vector2f result;
    result.x = a*v1.x;
    result.y = a*v1.y;
    return result;
}

vector2f div(vector2f v1, float a)
{
    if (a == 0)             // return same vector if div on 0
        return v1;
    vector2f result;
    result.x = v1.x/a;
    result.y = v1.y/a;
    return result;
}

float scale(vector2f v1, vector2f v2)
{
    return(v1.x*v2.x + v1.y*v2.y);
}

float len(vector2f v1)
{
    return(sqrt(v1.x*v1.x + v1.y*v1.y));
}

vector2f normalize(vector2f v1)
{
    vector2f result;
    result = div(v1, len(v1));
    return result;
}

struct Circle
{
    vector2f Q;

    vector2f V;

    float R = 15;

    int Red = 50;
    int Green = 255;
    int Blue = 0;

    bool ExistFactor = true;
} ;

void drawCircle(Circle Ball )
{
    COLORREF Color = txGetFillColor();
    for(int i = Ball.R;i > 0; i = i - 2)
        {
            int Redf = Ball.Red - Ball.Red*0.85*i/Ball.R;
            int Greenf = Ball.Green - Ball.Green*0.85*i/Ball.R;
            int Bluef = Ball.Blue - Ball.Blue*0.85*i/Ball.R;

            txSetColor(RGB( Redf, Greenf , Bluef ));
            txSetFillColor(RGB( Redf, Greenf ,Bluef ));
            txCircle( Ball.Q.x , Ball.Q.y , i);
        }
    txSetFillColor( Color);
}


void moveCircle(Circle* Ball, float Dt, int SCREEN_Y, int SCREEN_X)
{

    if ( Ball->Q.y > (SCREEN_Y-Ball->R) or Ball->Q.y < Ball->R)
        Ball->V.y = -Ball->V.y;
    if ( Ball->Q.x > (SCREEN_X-Ball->R) or Ball->Q.x < Ball->R)
        Ball->V.x = -Ball->V.x;

    Ball->Q.x += Ball->V.x * Dt;
    Ball->Q.y += Ball->V.y * Dt;

}


void controlCircle(Circle* Ball)
{
    if (GetAsyncKeyState(VK_LEFT))
        Ball->V.x = Ball->V.x - 0.35;
    if (GetAsyncKeyState(VK_RIGHT))
        Ball->V.x = Ball->V.x + 0.35;
    if (GetAsyncKeyState(VK_UP))
        Ball->V.y = Ball->V.y - 0.35;
    if (GetAsyncKeyState(VK_DOWN))
        Ball->V.y = Ball->V.y + 0.35;
}


void addResistance(Circle* Ball, float K)
{
    Ball->V.x = Ball->V.x - K * Ball->V.x;
    Ball->V.y = Ball->V.y - K * Ball->V.y;
}

bool checkCollision(Circle Ball1, Circle Ball2)
{
    if ( (Ball1.Q.x-Ball2.Q.x)*(Ball1.Q.x-Ball2.Q.x) + (Ball1.Q.y-Ball2.Q.y)*(Ball1.Q.y-Ball2.Q.y) <= (Ball1.R+Ball2.R)*(Ball1.R+Ball2.R))
        return true;
    else
        return false;
}

void endGame(int WinFactor, Circle Ball)
{
    if ( WinFactor == 5 )
        {
         txDrawText( 300, 200, 500, 400, "You Win!");
         txSleep(10000000);
        }
    if ( Ball.Q.x <= Ball.R )
        {
         txDrawText( 300, 200, 500, 400, "You lose!");
         txSleep(10000000);
        }
 }
void offCentreStrike(Circle* Ball1, Circle* Ball2)
{
    if (checkCollision(*Ball1, *Ball2))
     {
        vector2f O1O2 = sub(Ball2->Q, Ball1->Q); // vector between centres
        vector2f V1proextion = mul(normalize(O1O2), scale(Ball1->V, O1O2) / len(O1O2) );  // proextions on O1O2
        vector2f V2proextion = mul(normalize(O1O2), scale(Ball2->V, O1O2) / len(O1O2) );

        Ball1->V = sub(Ball1->V, V1proextion);
        Ball1->V = add(Ball1->V, V2proextion);

        Ball2->V = sub(Ball2->V, V2proextion);
        Ball2->V = add(Ball2->V, V1proextion);

        if ( len(O1O2) < (Ball1->R +Ball2->R) )  // devides stucked balls
            {
                Ball2->Q = add(Ball2->Q, mul(normalize(O1O2), (Ball1->R + Ball2->R -  len(O1O2))/2));
                Ball1->Q = sub(Ball1->Q, mul(normalize(O1O2), (Ball1->R + Ball2->R -  len(O1O2))/2));
            }
    }

}
void moveAllBalls(Circle* Balls, int PartNumber)
{
    for(int i = 0; i < PartNumber; i++)
        {
            moveCircle(&Balls[i], Dt, SCREEN_Y, SCREEN_X);
        }
}

void strikeAllBalls(Circle* Balls, int PartNumber)
{
    for(int i = 0; i < PartNumber; i++)
    {
        for(int j = i+1; j < PartNumber; j++)
            offCentreStrike(&Balls[i], &Balls[j]);
    }
}

void drawAllBalls(Circle* Balls, int PartNumber)
{
    txBegin();

    for(int i = 0; i < PartNumber; i++)
    {
        drawCircle(Balls[i]);
    }

    txEnd();
}

void addGravity(Circle* Balls, int PartNumber)
{
    for(int i = 0; i < PartNumber; i++)
        Balls[i].V.y = Balls[i].V.y + 5;
}

float countAverageVelocity(Circle* Balls, int PartNumber)
{
    float summa;
    for(int i = 0; i < PartNumber; i++)
       summa += len(Balls[i].V) ;

    return(summa / PartNumber);
}

void spawnBalls(Circle* Balls, int PartNumber, int SCREEN_X, int SCREEN_Y)
{
    srand(time(NULL));
    for(int i = 0; i < PartNumber; i ++)
    {
        Balls[i].Q.x = rand() % (SCREEN_X - 100) + 65;
        Balls[i].Q.y = rand() % (SCREEN_Y - 100) + 65;
        Balls[i].V.x = rand() % 150;
        Balls[i].V.y = rand() % 150;

    }


}
int main()
{

    int n = 250;
    float average;
    txCreateWindow(800,600);

    float WinFactor = 0;
    Circle* Balls = new Circle[n];

    //Balls[0].Q.x = 100;
    //Balls[0].Q.y = 300;
    //Balls[0].V.x = 20;
    //Balls[0].V.y = 500;
    Balls[0].R = 15;


    txFillColor(0,0,0);
    spawnBalls( Balls, n, SCREEN_X, SCREEN_Y);


    for(;;)
        {
            moveAllBalls(Balls, n);
            strikeAllBalls(Balls, n);

            txClear();
            drawAllBalls(Balls, n);

            txDrawText( 1, 10, 700, 50, " Look at this amaing gas! Press down to add gravity");

            if (GetAsyncKeyState(VK_DOWN))
                addGravity(Balls, n);


            txSleep(Dt);
        }

    return 0;
}

