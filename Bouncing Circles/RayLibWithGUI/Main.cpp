#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"                 // Required for GUI controls


#include "raylib.h"                 //Main Raylib core library
#include "raymath.h"                //Raylib Math functions library
#include "rlgl.h"                   //RayLib OpenGL library

#include <vector>
#include <iostream>


class Circle
{
public:
    float size;
    Vector2 velocity;
    Vector2 position;
    float mass;
    float bounciness;               //How much velocity is converted backwards on a bounce. 0.0 to 1.0 is recommended - negative will have odd effects, above 1 will exponentially increase velocity.
    Color colour;

    Circle(float newSize,Vector2 newVelocity,Vector2 newPosition,float newMass,float newBounciness,Color newColour) {
        size = newSize;
        velocity = newVelocity;
        position = newPosition;
        mass = newMass;
        bounciness = newBounciness;
        colour = newColour;
    }
};

Circle bounceWalls(Circle circle, int screenHeight, int screenWidth)
{
    if (circle.position.y >= screenHeight - circle.size and circle.velocity.y >= 0)
    {
        circle.position.y = screenHeight - circle.size;
        circle.velocity.y *= -(circle.bounciness); // Reverse the y-velocity
    }
    else if (circle.position.y <= circle.size and circle.velocity.y <= 0)
    {
        circle.position.y = circle.size;
        circle.velocity.y *= -(circle.bounciness); // Reverse the y-velocity
    }

    if (circle.position.x >= screenWidth - circle.size and circle.velocity.x >= 0)
    {
        circle.position.x = screenWidth - circle.size;
        circle.velocity.x *= -(circle.bounciness); // Reverse the x-velocity
    }
    else if (circle.position.x <= circle.size and circle.velocity.x <= 0)
    {
        circle.position.x = circle.size;
        circle.velocity.x *= -(circle.bounciness); // Reverse the x-velocity
    }
    return circle;
}

bool collisionCheck(Circle mainCircle, Circle secondCircle)
{
    bool collision = false;

    double distX = mainCircle.position.x - secondCircle.position.x;
    double distY = mainCircle.position.y - secondCircle.position.y;

    double distance = sqrt((distX * distX) + (distY * distY));

    if (distance <= (mainCircle.size + secondCircle.size))
    {
        collision = true;
    }
    else
    {
        collision = false;
    }

    return collision;
}

Circle collisionCorrect(Circle mainCircle, Circle secondCircle)
{
    double distX = mainCircle.position.x - secondCircle.position.x;
    double distY = mainCircle.position.y - secondCircle.position.y;

    double distance = sqrt((distX * distX) + (distY * distY));

    if (distance <= (mainCircle.size + secondCircle.size))
    {
        double angle = atan2(mainCircle.position.y - secondCircle.position.y, mainCircle.position.x - secondCircle.position.x);         //Determines the angle between the two circles.

    }

    return mainCircle;
}

int main()
{

    int timeSlow = 10;
    const int screenWidth = 800;
    const int screenHeight = 450;

    bool collision = false;
    Vector2 gravity = { 0.0f, 9.8f }; // Gravity force (y-component)

    //Variables for randomly generated circles
    int numRandomCircles = 5;
    
    int minSize = 5;
    int maxSize = 50;
    
    int minVelocityX = -100;
    int maxVelocityX = 100;
    int minVelocityY = -100;
    int maxVelocityY = 100;
    
    int minPositionX = 0;
    int maxPositionX = screenWidth;
    int minPositionY = 0;
    int maxPositionY = screenHeight;

    int minMass = 5;
    int maxMass = 50;

    std::vector<Circle> Circles;
    
    Circle newCircle = { 0, { 0,0 }, { 0,0 }, 0, 0.8, BLUE };
    
    for (int i = 1; i <= numRandomCircles; i++)
    {
        newCircle.size = minSize + (std::rand() % (maxSize - minSize + 1));

        newCircle.velocity.x = minVelocityX + (std::rand() % (maxVelocityX - minVelocityX + 1));
        newCircle.velocity.y = minVelocityY + (std::rand() % (maxVelocityY - minVelocityY + 1));

        newCircle.position.x = minPositionX + (std::rand() % (maxPositionX - minPositionX + 1));
        newCircle.position.y = minPositionY + (std::rand() % (maxPositionY - minPositionY + 1));

        newCircle.mass = minMass + (std::rand() % (maxMass - minMass + 1));

        Circles.push_back(newCircle);
    }





    InitWindow(screenWidth, screenHeight, "Bouncing Circles");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        //Update all circles
        for (int i = 0; i < Circles.size(); i++)
        {
            // Update velocity
            Circles[i].velocity.y += (gravity.y / timeSlow);        // Add gravity to velocity

            //Update position according to velocity
            Circles[i].position.x += (Circles[i].velocity.x / timeSlow);
            Circles[i].position.y += (Circles[i].velocity.y / timeSlow);

            // Bounce back where necessary.
            Circles[i] = bounceWalls(Circles[i], screenHeight, screenWidth);
        }

        //Check for collision with other circles.
        for (int i = 0; i < Circles.size(); i++)
        {
            for (int j = 0; j < Circles.size(); j++)
            {
                collision = false;
                if (i != j)
                {
                    collision = collisionCheck(Circles[i], Circles[j]);

                    if (collision == true)
                    {
                        Circles[i].colour = RED;
                        break;
                    }
                    else
                    {
                        Circles[i].colour = BLUE;
                    }
                }
            }
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Draw circles
        for (int i = 0; i < Circles.size(); i++)
        {
            DrawCircle(Circles[i].position.x, Circles[i].position.y, Circles[i].size, Circles[i].colour);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}