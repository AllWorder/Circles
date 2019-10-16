struct vector2f
{
    float x = 0;
    float y = 0;

    vector2f operator*(float a)
    {
        return vector2f(this->x * a, this->y * a);
    }

    vector2f operator/(float a)
    {
        if (a == 0)             // return same vector if div on 0
            return *this;

        return vector2f(this->x / a, this->y / a);
    }

    vector2f operator+(vector2f v1)
    {
        return vector2f(this->x + v1.x, this->y + v1.y);
    }

    vector2f operator-(vector2f v1)
    {
        return vector2f(this->x - v1.x, this->y - v1.y);
    }

    float len()
    {
        return(sqrt(this->x * this->x + this->y * this->y));
    }

    float scale(vector2f v2)
    {
        return(this->x * v2.x + this->y * v2.y);
    }

    vector2f normalize()
    {
        return *this / this->len();
    }

    vector2f(float x, float y);
    vector2f();
} ;

vector2f::vector2f(float x, float y) //new consctructor 
{
    this->x = x;
    this->y = y;
}

vector2f::vector2f()
{
}

// old functions
//
//

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
    result.x = v1.x / a;
    result.y = v1.y / a;
    return result;
}

float len(vector2f v1)
{
    return(sqrt(v1.x*v1.x + v1.y*v1.y));
}

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

float scale(vector2f v1, vector2f v2)
{
    return(v1.x*v2.x + v1.y*v2.y);
}
        
vector2f normalize(vector2f v1)
{
    vector2f result;
    result = v1 / v1.len();
    return result;
}
