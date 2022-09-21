// note that because the game is handled client-side,
// it is very easy to modify it and get an advantage over
// your opponent
#include "game.hpp"

Game::Game(player &client, player &opponent){
    window.create({1300, 800, 32}, "pong");
    window.setFramerateLimit(30);

    if(client.side){
        left = &opponent;
        right = &client;
    }
    else{
        left = &client;
        right = &opponent;
    }

    font.loadFromFile("../res/squarechunks.ttf");
        score_l.setFont(font);
        score_l.setCharacterSize(72);
        score_l.setFillColor(sf::Color(105, 105, 105));
        score_l.setPosition(window.getSize().x / 3, 10);

        score_r.setFont(font);
        score_r.setCharacterSize(72);
        score_r.setFillColor(sf::Color(105, 105, 105));
        score_r.setPosition(2 * window.getSize().x / 3, 10);
}

sf::RenderWindow *Game::get_window(){
    return &window;
}

void Game::display(){
    window.display();
    window.clear(sf::Color::Black);
}

void Game::draw_score(){
    score_l.setString(std::to_string(left->score));
    score_r.setString(std::to_string(right->score));
    if(left->pos >= 0)  window.draw(score_l);
    if(right->pos >= 0) window.draw(score_r);
}

void Game::update(){
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
    //todo display names
    //todo change dy based on where the ball hits the player
    float rx = (ball.px * window.getSize().x) / 100;
    float ry = (ball.py * window.getSize().y) / 100;
    if(rx + ball.size >= right_x){
        if(ry >= right_y - ball.size && ry + ball.size <= right_y + right->shape.getSize().y + ball.size){ 
            ball.shape.setFillColor(sf::Color::White);
            ball.dx *= -1;
            ball.dx -= 0.1;
        }
        else{
            ball.px = 50 - ((ball.size / window.getSize().x) * 100);
            ball.py = 50 - ((ball.size / window.getSize().y) * 100);

            ball.dx = -0.5;

            left->score++;
        }
    }
    if(rx - ball.size <= left_x){
        if(ry >= left_y - ball.size && ry + ball.size <= left_y + left->shape.getSize().y + ball.size){
            ball.shape.setFillColor(sf::Color::White);
            ball.dx *= -1;
            ball.dx += 0.1;
        }
        else{
            ball.px = 50 - ((ball.size / window.getSize().x) * 100);
            ball.py = 50 - ((ball.size / window.getSize().y) * 100);

            ball.dx = 0.5;

            right->score++;
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