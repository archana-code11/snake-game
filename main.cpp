// SFML Snake Game - CODTECH Task 3
// Developed by Archana Kumari for internship completion

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <ctime>

const int width = 800;
const int height = 600;
const int blockSize = 20;

enum Direction { Stop = 0, Left, Right, Up, Down };

class Snake {
public:
    std::vector<sf::RectangleShape> body;
    Direction dir = Stop;
    int speed = 150; // milliseconds

    Snake() {
        sf::RectangleShape head(sf::Vector2f(blockSize, blockSize));
        head.setFillColor(sf::Color::Green);
        head.setPosition(width / 2, height / 2);
        body.push_back(head);
        dir = Right;
    }

    void move() {
        for (int i = body.size() - 1; i > 0; --i) {
            body[i].setPosition(body[i - 1].getPosition());
        }
        sf::Vector2f pos = body[0].getPosition();
        if (dir == Left) pos.x -= blockSize;
        if (dir == Right) pos.x += blockSize;
        if (dir == Up) pos.y -= blockSize;
        if (dir == Down) pos.y += blockSize;
        body[0].setPosition(pos);
    }

    void grow() {
        sf::RectangleShape newBlock(sf::Vector2f(blockSize, blockSize));
        newBlock.setFillColor(sf::Color::Green);
        newBlock.setPosition(body.back().getPosition());
        body.push_back(newBlock);
    }

    bool checkCollision() {
        sf::Vector2f headPos = body[0].getPosition();
        if (headPos.x < 0 || headPos.y < 0 || headPos.x >= width || headPos.y >= height)
            return true;
        for (int i = 1; i < body.size(); ++i) {
            if (body[i].getPosition() == headPos)
                return true;
        }
        return false;
    }
};

class Food {
public:
    sf::RectangleShape food;

    Food() {
        food.setSize(sf::Vector2f(blockSize, blockSize));
        food.setFillColor(sf::Color::Red);
        respawn();
    }

    void respawn() {
        int x = (rand() % (width / blockSize)) * blockSize;
        int y = (rand() % (height / blockSize)) * blockSize;
        food.setPosition(x, y);
    }
};

int main() {
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode(width, height), "Snake Game - CODTECH Task 3");
    window.setFramerateLimit(60);

    Snake snake;
    Food food;

    sf::Clock clock;
    sf::Time timeSinceLastMove = sf::Time::Zero;

    // Sound (optional - file path may need to be adjusted)
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("assets/eat.wav")) {
        // fallback: no sound
    }
    sf::Sound eatSound;
    eatSound.setBuffer(buffer);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up && snake.dir != Down) snake.dir = Up;
                else if (event.key.code == sf::Keyboard::Down && snake.dir != Up) snake.dir = Down;
                else if (event.key.code == sf::Keyboard::Left && snake.dir != Right) snake.dir = Left;
                else if (event.key.code == sf::Keyboard::Right && snake.dir != Left) snake.dir = Right;
            }
        }

        timeSinceLastMove += clock.restart();
        if (timeSinceLastMove.asMilliseconds() >= snake.speed) {
            snake.move();
            timeSinceLastMove = sf::Time::Zero;

            if (snake.body[0].getGlobalBounds().intersects(food.food.getGlobalBounds())) {
                snake.grow();
                food.respawn();
                eatSound.play();

                // Increase speed (difficulty)
                if (snake.speed > 50)
                    snake.speed -= 5;
            }

            if (snake.checkCollision()) {
                window.close(); // Game over
            }
        }

        window.clear();
        window.draw(food.food);
        for (auto& block : snake.body) window.draw(block);
        window.display();
    }

    return 0;
}
