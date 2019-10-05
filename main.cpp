#include <SFML/Graphics.hpp>

const int SCREEN_Y = 600;
const int SCREEN_X = 800;

struct vector2f
{
    float x = 0;
    float y = 0;
} ;

struct Circle
{
    vector2f Q;

    vector2f V;

    float r = 50;

    int red = 50;
    int green = 255;
    int blue = 0;

    bool ExistFactor = true;
} ;

void drawBall(Circle ball, sf::RenderWindow* window)
{
    sf::CircleShape shape(ball.r);

    for(int i = ball.r;i > 0; i = i - 2)
    {
        int currentRed = ball.red - ball.red*0.85*i/ball.r;
        int currentGreen = ball.green - ball.green*0.85*i/ball.r;
        int currentBlue = ball.blue - ball.blue*0.85*i/ball.r;

        shape.setFillColor(sf::Color(currentRed, currentGreen, currentBlue));
        shape.setPosition(ball.Q.x, ball.Q.y);
        window->draw(shape);
    }
}




int main()
{
    Circle ball;


    sf::RenderWindow window(sf::VideoMode(SCREEN_X, SCREEN_Y), "Game");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        drawBall(ball, &window);
        window.display();
    }

    return 0;
}
