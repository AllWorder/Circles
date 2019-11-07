class DrawableObject
{
    public:
        virtual void draw(sf::RenderWindow* window)
        {
            std::cout << "s1" << "\n";
        }
};

class GraphicsManager
{
    private:

        int len = 250;
        DrawableObject** Objects = new DrawableObject*[len];
        int pos = 0;

    public:

        void registrate(DrawableObject* obj)
        {
            if(pos > len - 1)
            {
                this->len = this->len * 2;
                Objects = (DrawableObject**) realloc(Objects, this->len * sizeof(DrawableObject*));
            }
            Objects[this->pos] = obj;
            this->pos += 1;
            
        }

        void unregistrate(DrawableObject* obj)
        {
            for(int i = 0; i < pos - 1; i++)
                if(Objects[i] == obj)
                {
                    for(int j = i; j < pos - 1; j++)
                         Objects[j] = Objects[j+1];
                     Objects[pos - 1] = NULL;
                     return;
                }
        }

        void drawAll(sf::RenderWindow* window)
        {
            for(int i = 0; i < pos; i++)
                Objects[i]->draw(window);
        }

};

