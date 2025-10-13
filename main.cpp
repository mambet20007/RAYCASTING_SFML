#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

unsigned int windowSizeX = 1000;
unsigned int windowSizeY = 800;

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
    {1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1}
};

double posX = 3.0, posY = 3.0;
double dirX = -1.0, dirY = 0.0;
double planeX = 0.0, planeY = 0.66;
void procesInput(){
    const double moveSpeed = 0.01;
    const double rotSpeed = 0.007;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        if (worldMap[int(posX + dirX * moveSpeed)][int(posY)] == 0) posX += dirX * moveSpeed;
        if (worldMap[int(posX)][int(posY + dirY * moveSpeed)] == 0) posY += dirY * moveSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    if (worldMap[int(posX - dirX * moveSpeed)][int(posY)] == 0) posX -= dirX * moveSpeed;
    if (worldMap[int(posX)][int(posY - dirY * moveSpeed)] == 0) posY -= dirY * moveSpeed;
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

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        procesInput();
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


        if (side == 0) perWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
            else perWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;

        float lineHeight = (windowSizeY / perWallDist);
        float drawStart = -lineHeight / 2 + windowSizeY / 2;
        if (drawStart < 0) drawStart = 0;
        float deltaColor = 255 - perWallDist * 50;
        if(deltaColor < 0) deltaColor = 0;
        sf::RectangleShape line(sf::Vector2f(1.f, lineHeight));
        line.setFillColor(sf::Color(deltaColor,deltaColor,deltaColor));
        line.setPosition(x, drawStart); // Позиция линии (X, Y)
        window.draw(line);

    }
        window.display();
        window.clear();
}
    return 0;
}