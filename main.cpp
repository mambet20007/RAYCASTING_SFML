#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <unistd.h>

unsigned int windowSizeX = 1024;
unsigned int windowSizeY = 768;

const int mapWidth = 8;
const int mapHeight = 15;
int worldMap[mapWidth][mapHeight] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1}
};

const double playerRadius = 0.1;
double posX = 3.0, posY = 3.0;
double dirX = -1.0, dirY = 0.0;
double planeX = 0.0, planeY = 0.66;
bool canMoveTo(double newX, double newY) {
    if (newX - playerRadius < 0 || newX + playerRadius >= mapWidth ||
        newY - playerRadius < 0 || newY + playerRadius >= mapHeight) {
        return false;
    }
    if (worldMap[static_cast<int>(newX - playerRadius)][static_cast<int>(newY)] > 0 ||
        worldMap[static_cast<int>(newX + playerRadius)][static_cast<int>(newY)] > 0 ||
        worldMap[static_cast<int>(newX)][static_cast<int>(newY - playerRadius)] > 0 ||
        worldMap[static_cast<int>(newX)][static_cast<int>(newY + playerRadius)] > 0) {
        return false;
    }

    return true;
}
double shakeValue = 0;
const int shakeDist = 50;
bool shakeTop, shakeBot;
const double moveSpeed = 0.003;
int inventory = 1;
int inventoryValue = 2;
void procesInput(){

const double rotSpeed = 0.004;
double newPosX = posX + dirX * moveSpeed;
double newPosY = posY + dirY * moveSpeed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        if (canMoveTo(newPosX, posY)) posX = newPosX;
        if (canMoveTo(posX, newPosY)) posY = newPosY;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        newPosX = posX - dirX * moveSpeed;
        newPosY = posY - dirY * moveSpeed;
        if (canMoveTo(newPosX, posY)) posX = newPosX;
        if (canMoveTo(posX, newPosY)) posY = newPosY;
    }


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        double oldDirX = dirX;
        dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
        dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
        double oldPlaneX = planeX;
        planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
        planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        double oldDirX = dirX;
        dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
        dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
        double oldPlaneX = planeX;
        planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
        planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    }
}

int main(){

    sf::RenderWindow window(sf::VideoMode({windowSizeX, windowSizeY}), "RAYCASTING");
        sf::Event event;
        sf::Texture textureHand1, textureGun1, textureWall1;
        if (!textureHand1.loadFromFile("textures/sig.png") ||
            !textureGun1.loadFromFile("textures/gun1.png") ||
            !textureWall1.loadFromFile("textures/wall1.jpg")) {
            std::cout << "ERROR::CAN_NOT_LOAD_TEXTURES" << std::endl;
            return -1;
        }
        sf::Sprite spriteGun1;
        spriteGun1.setTexture(textureGun1);
        sf::Sprite spriteWall1;
        spriteWall1.setTexture(textureWall1);
        sf::Sprite spriteHand1;
        spriteHand1.setTexture(textureHand1);  
        

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)){

        }
        bool isKeyPressed = 0;
        if (event.type == sf::Event::KeyPressed && !isKeyPressed) {
            if (event.key.code == sf::Keyboard::Tab) {
                inventory += 1;
                if (inventory > inventoryValue){inventory = 1;} 
                isKeyPressed = true;
            }
        }
        if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::Tab) {
                isKeyPressed = false;
            }
        }
        sf::RectangleShape roof(sf::Vector2f(windowSizeX, windowSizeY / 2));
        sf::Color roofColor (10, 26, 43);
        roof.setFillColor(roofColor);
        roof.setPosition(0, 0);
        window.draw(roof);

        sf::RectangleShape flor(sf::Vector2f(windowSizeX, windowSizeY / 2));
        sf::Color florColor (21, 26, 31);
        flor.setFillColor(florColor);
        flor.setPosition(0, windowSizeY / 2);
        window.draw(flor);

        procesInput();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            if (shakeValue >= shakeDist || shakeBot == 1 && shakeValue > 0){
                shakeBot = 1;
                shakeTop = 0; 
                shakeValue -= moveSpeed * 100;
            }
            if (shakeValue <= 0 || shakeTop == 1 && shakeValue < shakeDist){
                shakeBot = 0;
                shakeTop = 1;
                shakeValue += moveSpeed * 100;
            }
        }

        for (int x = 0; x < windowSizeX; x++) {
        double cameraX = 2 * x / double(windowSizeX) - 1;
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;

        int mapX = int(posX);
        int mapY = int(posY);

        double sideDistX, sideDistY;
        double deltaDistX = fabs(1 / rayDirX);
        double deltaDistY = fabs(1 / rayDirY);
        double perWallDist;

        int stepX, stepY;
        int hit = 0;
        int side;

        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (posX - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - posX) * deltaDistX;
        }

        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (posY - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - posY) * deltaDistY;
        }

        while (hit == 0) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            if (mapX < 0 || mapX >= mapWidth || mapY < 0 || mapY >= mapHeight) {
                hit = 1;
            } else if (worldMap[mapX][mapY] > 0) {
                hit = 1;
            }
        }

        if (side == 0) {
            perWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
        } else {
            perWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;
        }

        int lineHeight = int(windowSizeY / perWallDist);
        float drawStart = -lineHeight / 2 + windowSizeY / 2;
        if (drawStart < 0) drawStart = 0;

        double wallX;
        if (side == 0) {
        wallX = posY + perWallDist * rayDirY;
        } else {
            wallX = posX + perWallDist * rayDirX;
        }
        wallX -= floor(wallX);
        int texX = int(wallX * double(textureWall1.getSize().x));
        if (texX >= textureWall1.getSize().x) texX = textureWall1.getSize().x - 1;
        if (texX < 0) texX = 0;
        
        
        
    if (lineHeight > windowSizeY) {
        float cutRatio = float(windowSizeY) / float(lineHeight);
        int textureHeight = int(textureWall1.getSize().y * cutRatio);
        int cutTop = (textureWall1.getSize().y - textureHeight) / 2;
        lineHeight = windowSizeY;

        spriteWall1.setTextureRect(sf::IntRect(texX, cutTop, 1, textureHeight));
        spriteWall1.setPosition(x, -lineHeight / 2 + windowSizeY / 2);

        float scaleY = float(lineHeight) / float(textureHeight);
        spriteWall1.setScale(1.0f, scaleY);

        float darkValue = 15.0f;
        float deltaColor = 255 - perWallDist * darkValue;
        if(deltaColor < 0) deltaColor = 0;
        sf::Color dark(deltaColor,deltaColor,deltaColor);
        spriteWall1.setColor(dark);
        
        window.draw(spriteWall1);
    } else {

        spriteWall1.setTextureRect(sf::IntRect(texX, 0, 1, textureWall1.getSize().y));
        spriteWall1.setPosition(x, -lineHeight / 2 + windowSizeY / 2);

        float scaleY = float(lineHeight) / float(textureWall1.getSize().y);
        spriteWall1.setScale(1.0f, scaleY);
        float darkValue = 15.0f;
        float deltaColor = 255 - perWallDist * darkValue;
        if(deltaColor < 0) deltaColor = 0;
        sf::Color dark(deltaColor,deltaColor,deltaColor);
        spriteWall1.setColor(dark);

        window.draw(spriteWall1);
    }
}
           /* if (inventory == 1) {
                spriteHand1.setPosition(windowSizeX - textureHand1.getSize().x, windowSizeY - textureHand1.getSize().y + shakeValue);
                window.draw(spriteHand1);
            }
            if (inventory == 2) {
                spriteGun1.setPosition(0, windowSizeY - textureGun1.getSize().y + shakeValue);
                window.draw(spriteGun1);
            } */

        window.display();
        window.clear();
} 
    return 0;
}
