#include <SFML/Graphics.hpp>

struct player{
    string username;
    bool status;
    sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(35, 260));

    float pos;
    bool side;
    int score = 0;
};

struct ball{
	int size = 35;

	float dx = 0.5, dy = 0.5;
	float px = 50,
		  py = 50;

    sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(size, size));
} ball;

class Game{
    public:
        Game(player &client, player &opponent);
        sf::RenderWindow *get_window();
        void update();
        void draw_text();
        void display();
        void recalc_pos();
    private:
        sf::RenderWindow window;
        sf::Font score_font, name_font;
        sf::Text score_l, score_r;
        sf::Text name_l, name_r;
        player *left = new player, *right = new player;

        float right_x;
        float left_x;

        int BOUNCE_FACTOR = 100; // determines rebound angle magnitude
        float rebound(float, float, float);
};