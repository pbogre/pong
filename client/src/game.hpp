#include <SFML/Graphics.hpp>

struct player{
    string username;
    bool status;
    sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(35, 300));
    float pos;
};

void draw_players(sf::RenderWindow &window, player local, player opponent){
    local.shape.setPosition(50, (local.pos * window.getSize().y) / 100);
    window.draw(local.shape);

    if(opponent.pos >= 0){
        opponent.shape.setPosition(window.getSize().x - 80, (opponent.pos * window.getSize().y) / 100);
        window.draw(opponent.shape);
    }
}

void draw_ball(sf::RenderWindow &window, float x, float y){
    sf::CircleShape ball = sf::CircleShape(15);
        ball.setPosition(sf::Vector2f(x, y));

    window.draw(ball);
}