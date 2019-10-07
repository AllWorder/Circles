#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>

const int SCREEN_Y = 600;
const int SCREEN_X = 800;
const float Dt = 0.05;
const float RESISTANCE_COEF = 0.0005;

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

    float r = 15;

    int red = 50;
    int green = 255;
    int blue = 100;

    bool ExistFactor = true;
} ;

void drawCircle(Circle ball, sf::RenderWindow* window)
{

    for(int i = ball.r;i > 0; i = i - 2)
    {
        int currentRed = ball.red - ball.red*0.85*i/ball.r;
        int currentGreen = ball.green - ball.green*0.85*i/ball.r;
        int currentBlue = ball.blue - ball.blue*0.85*i/ball.r;

        sf::CircleShape shape(i);
        shape.setFillColor(sf::Color(currentRed, currentGreen, currentBlue));
        shape.setPosition(ball.Q.x - i, ball.Q.y - i);
        window->draw(shape);
    }
}

void moveCircle(Circle* ball, float Dt, int SCREEN_Y, int SCREEN_X)
{

    if ( ball->Q.y > (SCREEN_Y-ball->r) or ball->Q.y < ball->r)
        ball->V.y = -ball->V.y;
    if ( ball->Q.x > (SCREEN_X-ball->r) or ball->Q.x < ball->r)
        ball->V.x = -ball->V.x;

    ball->Q.x += ball->V.x * Dt;
    ball->Q.y += ball->V.y * Dt;

}

void addResistance(Circle* ball, float resistanceCoef)
{
    ball->V.x = ball->V.x - resistanceCoef * ball->V.x;
    ball->V.y = ball->V.y - resistanceCoef * ball->V.y;
}

bool checkCollision(Circle ball1, Circle ball2)
{
    if ( (ball1.Q.x-ball2.Q.x)*(ball1.Q.x-ball2.Q.x) + (ball1.Q.y-ball2.Q.y)*(ball1.Q.y-ball2.Q.y) <= (ball1.r+ball2.r)*(ball1.r+ball2.r))
        return true;
    else
        return false;
}

void offCentreStrike(Circle* ball1, Circle* ball2)
{
    if (checkCollision(*ball1, *ball2))
     {
        vector2f O1O2 = sub(ball2->Q, ball1->Q); // vector between centres
        vector2f V1proextion = mul(normalize(O1O2), scale(ball1->V, O1O2) / len(O1O2) );  // proextions on O1O2
        vector2f V2proextion = mul(normalize(O1O2), scale(ball2->V, O1O2) / len(O1O2) );

        ball1->V = sub(ball1->V, V1proextion);
        ball1->V = add(ball1->V, V2proextion);

        ball2->V = sub(ball2->V, V2proextion);
        ball2->V = add(ball2->V, V1proextion);

        if ( len(O1O2) < (ball1->r +ball2->r) )  // devides stucked balls
            {
                ball2->Q = add(ball2->Q, mul(normalize(O1O2), (ball1->r + ball2->r -  len(O1O2))/2));
                ball1->Q = sub(ball1->Q, mul(normalize(O1O2), (ball1->r + ball2->r -  len(O1O2))/2));
            }
    }

}

void moveAllBalls(Circle* Balls, int partNumber)
{
    for(int i = 0; i < partNumber; i++)
        {
            moveCircle(&Balls[i], Dt, SCREEN_Y, SCREEN_X);
        }
}

void strikeAllBalls(Circle* Balls, int partNumber)
{
    for(int i = 0; i < partNumber; i++)
    {
        for(int j = i+1; j < partNumber; j++)
            offCentreStrike(&Balls[i], &Balls[j]);
    }
}

void drawAllBalls(Circle* Balls, int partNumber, sf::RenderWindow* window)
{
    for(int i = 0; i < partNumber; i++)
    {
        drawCircle(Balls[i], window);
    }
}

void spawnBalls(Circle* Balls, int partNumber, int SCREEN_X, int SCREEN_Y)
{
    srand(time(NULL));
    for(int i = 0; i < partNumber; i ++)
    {
        Balls[i].Q.x = rand() % (SCREEN_X - 100) + 65;
        Balls[i].Q.y = rand() % (SCREEN_Y - 100) + 65;
        Balls[i].V.x = rand() % 150;
        Balls[i].V.y = rand() % 150;

    }
}

int main()
{
 /* Circle ball1;
    ball1.Q.x = 400;
    ball1.Q.y = 300;
    ball1.V.x = 25;
    ball1.V.y = 25;
*/ 
    
    int partNumber = 5;
    std::cout << "Enter part number" << '\n';
    std::cin >> partNumber;

    sf::RenderWindow window(sf::VideoMode(SCREEN_X, SCREEN_Y), "test");

    Circle* Balls = new Circle[partNumber];

    spawnBalls( Balls, partNumber, SCREEN_X, SCREEN_Y);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        moveAllBalls(Balls, partNumber);
        strikeAllBalls(Balls, partNumber);

        window.clear();
        drawAllBalls(Balls, partNumber, &window);
        
         window.display();
    }

    return 0;
}