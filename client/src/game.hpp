#include <SFML/Graphics.hpp>
using namespace std;

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

	float dx = 0.5, dy = 0.5, defdy = dy;
	float px = 50,
		  py = 50;

    sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(size, size));
} ball;

class Game{
    public:
        Game(player &client, player &opponent, float step);
        sf::RenderWindow *get_window();
        void update();
        void draw_text();
        void display();
        //void recalc_pos();
    private:
        sf::RenderWindow window;
        sf::Font score_font, name_font;
        sf::Text score_l, score_r;
        sf::Text name_l, name_r;
        player *left = new player, *right = new player;

        float right_x;
        float left_x;
        float step;

        float rebound(float, float, float);
        float middle_point(sf::RectangleShape shape);
};
