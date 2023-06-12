// MAJOR TODO
// game handling client-side causes a lot of desync.
// pass dydx and other important values and let server 
// calculate position of ball for next frame? or something
#include <cmath>
#include "game.hpp"

Game::Game(player &client, player &opponent, float step){
    window.create({1100, 650, 32}, "pong", sf::Style::Close);
    window.setFramerateLimit(30);
    this->step = step;

    if(client.side){
        left = &opponent;
        right = &client;
    }
    else{
        left = &client;
        right = &opponent;
    }

    right_x = window.getSize().x - (2 * right->shape.getSize().x);
    left_x = left->shape.getSize().x;

    score_font.loadFromFile("../res/squarechunks.ttf");
        score_l.setFont(score_font);
        score_l.setCharacterSize(72);
        score_l.setString("0");
        score_l.setFillColor(sf::Color(105, 105, 105));
        score_l.setPosition((float)window.getSize().x / 3 - (score_l.getLocalBounds().width), 10);

        score_r.setFont(score_font);
        score_r.setCharacterSize(72);
        score_r.setString("0");
        score_r.setFillColor(sf::Color(105, 105, 105));
        score_r.setPosition(((float)2 * window.getSize().x / 3) + (score_r.getLocalBounds().width), 10);

    name_font.loadFromFile("../res/arial.ttf");
        name_l.setFont(name_font);
        name_l.setString(left->username);
        name_l.setCharacterSize(36);
        name_l.setFillColor(sf::Color(105, 105, 105));
        // score_M - (name.width / 2)
        name_l.setPosition((score_l.getPosition().x + score_l.getLocalBounds().width / 2) - (name_l.getLocalBounds().width / 2), score_l.getLocalBounds().height + name_l.getLocalBounds().height);

        name_r.setFont(name_font);
        name_r.setString(right->username);
        name_r.setCharacterSize(36);
        name_r.setFillColor(sf::Color(105, 105, 105));
        name_r.setPosition((score_r.getPosition().x + score_r.getLocalBounds().width / 2) - (name_r.getLocalBounds().width / 2), score_r.getLocalBounds().height + name_r.getLocalBounds().height);
}

sf::RenderWindow *Game::get_window(){
    return &window;
}

void Game::display(){
    window.display();
    window.clear(sf::Color::Black);
}

//todo fix pos when resizing window
void Game::draw_text(){
    score_l.setString(std::to_string(left->score));
    score_r.setString(std::to_string(right->score));
    if(left->pos >= 0){
        window.draw(score_l);
        window.draw(name_l);
    }
    if(right->pos >= 0){
        window.draw(score_r);
        window.draw(name_r);
    }
}

float Game::middle_point(sf::RectangleShape shape){
    return ((2 * shape.getPosition().y) + shape.getSize().y) / 2;
}

float Game::rebound(float paddle_y, float paddle_size, float ball_y){
    float M_ball = (ball_y + ball.size) + ball_y / 2;
    float M_paddle = (paddle_y + paddle_size) + paddle_y / 2;
    float diff = M_paddle - M_ball;

    // https://www.desmos.com/calculator/ks0lsoofuh, sigmoid function used for normalization
    // different paddle/ball sizes may need a different exponential coefficient ( change 0.02 )
    float normalized_rebound = ( -4 / (1 + exp(0.02 * diff)) ) + 2;
    return normalized_rebound;
}

void Game::update(){
    // Draw Players and Usernames
    float right_y = (right->pos * window.getSize().y) / 100;
    float left_y = (left->pos * window.getSize().y) / 100;

    if(left->pos >= 0){
        left->shape.setPosition(left_x, left_y);
        window.draw(left->shape);
    }

    if(right->pos >= 0){
        right->shape.setPosition(right_x, right_y);
        window.draw(right->shape);
    }

    //Handle Ball Collision
    float rx = (ball.px * window.getSize().x) / 100;
    float ry = (ball.py * window.getSize().y) / 100;
    if(rx + ball.size >= right_x){
        if(ry >= right_y - ball.size && ry + ball.size <= right_y + right->shape.getSize().y + ball.size){ 
            ball.dy = ball.defdy * rebound(right_y, right->shape.getSize().y, ry);
            ball.dx *= -1;
            ball.dx -= 0.1;
        }
        else{
            ball.px = 50 - (((float)ball.size / window.getSize().x) * 100);
            ball.py = 50 - (((float)ball.size / window.getSize().y) * 100);

            ball.dx = -0.5;

            left->score++;
        }
    }
    if(rx - ball.size <= left_x){
        if(ry >= left_y - ball.size && ry + ball.size <= left_y + left->shape.getSize().y + ball.size){
            ball.dy = ball.defdy * rebound(left_y, left->shape.getSize().y, ry);
            ball.dx *= -1;
            ball.dx += 0.1;
        }
        else{
            ball.px = 50 - (((float)ball.size / window.getSize().x) * 100);
            ball.py = 50 - (((float)ball.size / window.getSize().y) * 100);

            ball.dx = 0.5;

            right->score++;
        }
    }
    if(ry + ball.size >= window.getSize().y) ball.dy *= -1;
    if(ry < (window.getSize().y * step) / 100) ball.dy *= -1;

    ball.px += ball.dx;
    ball.py -= ball.dy;
    rx = (ball.px * window.getSize().x) / 100;
    ry = (ball.py * window.getSize().y) / 100;
    ball.shape.setPosition(rx, ry);

    window.draw(ball.shape);
}
