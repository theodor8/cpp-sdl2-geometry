#include <iostream>
#include <vector>
#include <math.h>

#include "SDL2/SDL.h"
#include "Vec2.h"
#include "common.h"
#include "Renderer.h"
#include "Shape.h"
#include "Circle.h"
#include "Polygon.h"
#include "BezierCurve.h"
//#include "Softbody.h"


// g++ *.cpp -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -lSDL2_image -std=c++17


std::pair<float, float> lineEquation(const Vec2& p1, const Vec2& p2)
{
    float k = (p2.y - p1.y) / (p2.x - p1.x);
    float m = p1.y - k * p1.x;
    return {k, m};
}


Renderer renderer;
std::vector<std::unique_ptr<Shape>> shapes;



void RayCast(Vec2 pos, float angle, float length)
{
    Vec2 endPos = pos + Vec2::FromPolar(length, angle);

    float closestDist = 0;
    Vec2 closestPoint = endPos;
    Vec2 closestNormal;
    for (auto& shape : shapes)
    {
        for (auto& p : shape->RayCast(pos, endPos))
        {
            //renderer.DrawFilledCircle(p.first, 5, {255, 0, 255});
            float dist = (pos - p.first).LengthSquared();

            if (!closestDist || dist < closestDist)
            {
                closestDist = dist;
                closestPoint = p.first;
                closestNormal = p.second;
            }
        }

    }

    // Draw ray
    renderer.DrawLine(pos, closestPoint, {255, 255, 255});

    if (!closestDist) return;
    
    // Draw normal
    renderer.DrawLine(closestPoint, closestPoint + closestNormal * 30, {100, 100, 100});

    // Bounce
    float bounceRayLength = length - (pos - closestPoint).Length();
    float bounceRayAngle = 2 * closestNormal.Angle() - angle + M_PI;
    Vec2 bounceRayPos = closestPoint + Vec2::FromPolar(1, bounceRayAngle);
    RayCast(bounceRayPos, bounceRayAngle, bounceRayLength);
}


int main()
{
    srand(time(NULL));

    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

    shapes.emplace_back(std::unique_ptr<Circle>(new Circle(30)));
    shapes.back()->pos = {100, 200};
    shapes.emplace_back(std::unique_ptr<Circle>(new Circle(20)));
    shapes.back()->pos = {350, 220};
    shapes.emplace_back(std::unique_ptr<Polygon>(new Polygon(Vec2(-150, -120), Vec2(-90, 100), Vec2(100, 110), Vec2(90, -90))));
    shapes.back()->pos = {300, 400};
    //BezierCurve bezierCurve(Vec2(100, 390), Vec2(80, 200), Vec2(300, 190), Vec2(350, 280), Vec2(440, 190));

    

    float angle = 0.0f;
    // float t = 0;

    float timeStep = 1.0f / 60.0f;
    Uint32 delay = timeStep * 1000;
    bool quit = false;
    while (!quit)
    {
        SDL_Delay(delay);
    
        int mouseScreenX, mouseScreenY;
        Uint32 mouseButtons = { SDL_GetMouseState(&mouseScreenX, &mouseScreenY) };
        Vec2 mousePos = {(float)mouseScreenX, (float)mouseScreenY};


        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    quit = SDL_TRUE;
                    break;

                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            quit = true;
                            break;
                            
                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }
        }

        if (keyboardState[SDL_SCANCODE_Q]) angle -= 0.05f;
        if (keyboardState[SDL_SCANCODE_E]) angle += 0.05f;

        // if (keyboardState[SDL_SCANCODE_A])
        // {
        //     t -= timeStep / 2;
        //     if (t < 0) t = 0;
        // }
        // if (keyboardState[SDL_SCANCODE_D])
        // {
        //     t += timeStep / 2;
        //     if (t > 1) t = 1;
        // }

        



        renderer.Clear();

        //bezierCurve.Draw(t);
        
        //shapes[2]->angle += 0.005f;

        for (auto& shape : shapes)
        {
            shape->Update(timeStep);
            shape->Draw();
            renderer.DrawFilledCircle(shape->GetWorldPoint(shape->com), 5, {255, 255, 0});
        }



        RayCast(mousePos, angle, 400.0f);

        //shapes[0]->pos = mousePos;

        for (int i = 0; i < shapes.size(); i++)
        {
            for (int j = i + 1; j < shapes.size(); j++)
            {
                auto pairs = shapes[i]->TestCollision(shapes[j].get());
                for (auto& p : pairs)
                {
                    renderer.DrawFilledCircle(p.first, 3, {255, 255, 255});
                    //renderer.DrawLine(p.first, p.first + p.second * 20, {0, 255, 0});
                }
            }
        }


        Vec2 endPos = mousePos + Vec2::FromPolar(100.0f, angle);
        renderer.DrawLine(mousePos, endPos, {255, 255, 255});
        Vec2 force = (endPos - mousePos).Normalized() * 100000;
        for (auto& shape : shapes)
        {
            for (auto& p : shape->RayCast(mousePos, endPos))
            {
                renderer.DrawFilledCircle(p.first, 5, {255, 0, 255});
                shape->ApplyForce(force, p.first);
            }
        }


        renderer.Present();
    }

    SDL_Quit();
    return EXIT_SUCCESS;
}