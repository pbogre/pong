#include <SFML/Graphics.hpp>
#include <string>

float get_rand(){
   float max = 3.0;
   return static_cast<float>(rand()) / (static_cast <float> (RAND_MAX/max));
}

struct player{
    string username;
    bool status;
    sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(35, 300));

    float pos;
    bool side;
    int score = 0;
};

struct ball{
	int size = 35;

	float dx = 0.5, dy = 0.5;
	float px = 50 - size,
		  py = 50 - size;

    sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(size, size));
} ball;

class Game{
    public:
        Game(player &client, player &opponent);
        sf::RenderWindow *get_window();
        void update();
        void draw_score();
        void display();
    private:
        sf::RenderWindow window;
        sf::Font font;
        sf::Text score_l, score_r;
        player *left = new player, *right = new player;
};