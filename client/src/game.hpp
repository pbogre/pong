// Reorganize this file with a Game class, with update(), draw_score()
// so window is only passed once and not for every iteration
#include <SFML/Graphics.hpp>

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

void update(sf::RenderWindow &window, player &client, player &opponent){
    player* left = new player;
    player* right = new player;
    if(client.side){
        left = &opponent;
        right = &client;
    }
    else{
        left = &client;
        right = &opponent;
    }

    // Draw Players
    float right_x = window.getSize().x - 80;
    float right_y = (right->pos * window.getSize().y) / 100;
    float left_x = 50;
    float left_y = (left->pos * window.getSize().y) / 100;

    left->shape.setPosition(left_x, left_y);
    window.draw(left->shape);

    if(right->pos >= 0){
        right->shape.setPosition(right_x, right_y);
        window.draw(right->shape);
    }

    //Handle Ball Collision
    //todo ball gets fucked when resizing
    //todo score - cannot change properties in loop from within here??
    //todo display names
    //todo change dy based on where the ball hits the player
    float rx = (ball.px * window.getSize().x) / 100;
    float ry = (ball.py * window.getSize().y) / 100;
    if(rx + ball.size >= right_x){
        if(ry >= right_y && ry + ball.size <= right_y + right->shape.getSize().y){ 
            ball.shape.setFillColor(sf::Color::White);
            ball.dx *= -1;
            ball.dx += 0.01;
        }
        else{
            ball.px = 50 - ((ball.size / window.getSize().x) * 100);
            ball.py = 50 - ((ball.size / window.getSize().y) * 100);

            ball.dx = 0.5;

            if(client.side) client.score++;
            else opponent.score++;
        }
    }
    if(rx - ball.size <= left_x){
        if(ry >= left_y && ry + ball.size <= left_y + left->shape.getSize().y){
            ball.shape.setFillColor(sf::Color::White);
            ball.dx *= -1;
            ball.dx += 0.05;
        }
        else{
            ball.px = 50 - ((ball.size / window.getSize().x) * 100);
            ball.py = 50 - ((ball.size / window.getSize().y) * 100);

            ball.dx = -0.5;

            if(client.side) opponent.score++;
            else client.score++;
        }
    }
    if(ry + ball.size >= window.getSize().y) ball.dy *= -1;
    if(ry <= 0) ball.dy *= -1;

    ball.px += ball.dx;
    ball.py += ball.dy;
    rx = (ball.px * window.getSize().x) / 100;
    ry = (ball.py * window.getSize().y) / 100;
    ball.shape.setPosition(rx, ry);

    window.draw(ball.shape);
}

void draw_score(){

}