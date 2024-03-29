#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>

#include "vector.h" // own new lib with vectors and operations on them
#include "GraphicsManeger.h"

const int SCREEN_Y = 600;
const int SCREEN_X = 900;
const float DT = 0.05;
const float RESISTANCE_COEF = 0.0005;


class Circle: public DrawableObject
{
    public:
    
        vector2f Q;
        vector2f V;

        float r = 15;

        int red = 50;
        int green = 255;
        int blue = 100;

        Circle(vector2f Q, vector2f V, float r, int red, int green, int blue); //new consctructor 
        Circle();

        void draw(sf::RenderWindow* window)
        {
            sf::CircleShape shape(this->r);
        
            for(int i = this->r;i > 0; i = i - 2)
            {
                int currentRed = this->red - this->red*0.85*i/this->r;
                int currentGreen = this->green - this->green*0.85*i/this->r;
                int currentBlue = this->blue - this->blue*0.85*i/this->r;

                shape.setRadius(i);
                shape.setFillColor(sf::Color(currentRed, currentGreen, currentBlue));
                shape.setPosition(this->Q.x - i, this->Q.y - i);
                window->draw(shape);
            }

        }

        void move(float DT, int SCREEN_Y, int SCREEN_X)
        {
            if ( this->Q.y > (SCREEN_Y-this->r) or this->Q.y < this->r)
                this->V.y = -this->V.y;
            if ( this->Q.x > (SCREEN_X-this->r) or this->Q.x < this->r)
                this->V.x = -this->V.x;

            this->Q.x += this->V.x * DT;
            this->Q.y += this->V.y * DT;
        }

        void addResistance(float resistanceCoef)
        {
            this->V.x = this->V.x - resistanceCoef * this->V.x;
            this->V.y = this->V.y - resistanceCoef * this->V.y;
        }
    
} ;

Circle::Circle(vector2f Q, vector2f V, float r, int red, int green, int blue) //new consctructor 
{
    this->Q = Q;
    this->V = V;
    this->r = r;
    this->red = red;
    this->green = green;
    this->blue = blue;
}

Circle::Circle()
{
}

bool checkCollision(Circle ball1, Circle ball2)
{
    return((ball1.Q.x-ball2.Q.x)*(ball1.Q.x-ball2.Q.x) + (ball1.Q.y-ball2.Q.y)*(ball1.Q.y-ball2.Q.y) <= (ball1.r+ball2.r)*(ball1.r+ball2.r));
}

void offCentreStrike(Circle* ball1, Circle* ball2)
{
    if (checkCollision(*ball1, *ball2))
     {
        vector2f O1O2 = ball2->Q - ball1->Q; // vector between centres
        vector2f V1proextion;
        vector2f V2proextion;
        if(O1O2.len() != 0)
        {   
            V1proextion = normalize(O1O2)*((scale(ball1->V, O1O2)) / O1O2.len() );  // proextions on O1O2
            V2proextion = normalize(O1O2)*((scale(ball2->V, O1O2)) / O1O2.len() );
        }
        ball1->V = ball1->V - V1proextion;
        ball1->V = ball1->V + V2proextion;

        ball2->V = ball2->V - V2proextion;
        ball2->V = ball2->V + V1proextion;

        if ( O1O2.len() < (ball1->r + ball2->r) )  // devides stucked balls
            {
                ball2->Q = ball2->Q + normalize(O1O2)*((ball1->r + ball2->r -  O1O2.len() )/2);
                ball1->Q = ball1->Q - normalize(O1O2)*((ball1->r + ball2->r -  O1O2.len() )/2);
            }
    }

}

void moveAllBalls(Circle* Balls, int partNumber)
{
    for(int i = 0; i < partNumber; i++)
        Balls[i].move(DT, SCREEN_Y, SCREEN_X);
}

void strikeAllBalls(Circle* Balls, int partNumber)
{
    for(int i = 0; i < partNumber; i++)
    {
        for(int j = i+1; j < partNumber; j++)
            offCentreStrike(&Balls[i], &Balls[j]);
    }
}

/*void drawAllBalls(Circle* Balls, int partNumber, sf::RenderWindow* window)
{
    for(int i = 0; i < partNumber; i++)
    {
        Balls[i].draw(window);
    }
}*/

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
    //CONTEXT:
    
    int partNumber = 5;
    std::cout << "Enter part number" << '\n';
    std::cin >> partNumber;

    sf::RenderWindow window(sf::VideoMode(SCREEN_X, SCREEN_Y), "test");

    Circle* Balls = new Circle[partNumber];             //generating balls with random speed and position
    spawnBalls( Balls, partNumber, SCREEN_X, SCREEN_Y);

    
    GraphicsManager manager;

    for(int i = 0; i < partNumber; i++)
        manager.registrate(&Balls[i]);

    while (window.isOpen())
    {
        //PHISICS:
        moveAllBalls(Balls, partNumber);
        strikeAllBalls(Balls, partNumber);

        //EVENT HANDLER:
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //LOGICS:

        //GRAPHICS:
        window.clear();
        manager.drawAll(&window);

        window.display();
    }

    delete[] Balls;
    return 0;
}