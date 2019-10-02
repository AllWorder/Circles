#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

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
int main()
{
        vector2f v1;
        v1.x = 5;
        v1.y = 0;

        vector2f v2;
        v2.x = 10;
        v2.y = 3;

        vector2f v3;

        float a = 5;

        v3 = normalize(v1);
        std::cout << v3.x;
        std::cout << v3.y;
}
