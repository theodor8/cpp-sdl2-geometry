#include "Renderer.h"

#include <math.h>
#include <string>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "Vec2.h"
#include "common.h"


Renderer::Renderer()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
}

Renderer::~Renderer()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Renderer::Clear()
{
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_RenderClear(renderer);
}

void Renderer::Present()
{
    SDL_RenderPresent(renderer);
}

void Renderer::SetDrawColor(const SDL_Color& color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}


void Renderer::DrawLine(const Vec2& v1, const Vec2& v2, const SDL_Color& color)
{
    SetDrawColor(color);
    SDL_RenderDrawLineF(renderer, v1.x, v1.y, v2.x, v2.y);
}

void Renderer::DrawFilledTriangle(const Vec2& v1, const Vec2& v2, const Vec2& v3, const SDL_Color& color1, const SDL_Color& color2, const SDL_Color& color3)
{
    SDL_Vertex vs[] = {
        {{v1.x, v1.y}, color1},
        {{v2.x, v2.y}, color2},
        {{v3.x, v3.y}, color3},
    };

    SDL_RenderGeometry(renderer, nullptr, vs, 3, nullptr, 0);
}

void Renderer::DrawRect(const Vec2& pos, const Vec2& size, const SDL_Color& color)
{
    SDL_FRect rect;
    rect.x = pos.x; rect.y = pos.y; rect.w = size.x; rect.h = size.y;
    SetDrawColor(color);
    SDL_RenderDrawRectF(renderer, &rect);
}

void Renderer::DrawCircle(const Vec2& center, float radius, const SDL_Color& color)
{
    const float k_segments = 32.0f;
    const float k_increment = 2.0f * M_PI / k_segments;
    float sinInc = sinf(k_increment);
    float cosInc = cosf(k_increment);
    Vec2 r1(1.0f, 0.0f);
    Vec2 v1 = center + r1 * radius;
    for (int i = 0; i < k_segments; ++i)
    {
        Vec2 r2;
        r2.x = cosInc * r1.x - sinInc * r1.y;
        r2.y = sinInc * r1.x + cosInc * r1.y;
        Vec2 v2 = center + r2 * radius;
        DrawLine(v1, v2, color);
        r1 = r2;
        v1 = v2;
    }
}

void Renderer::DrawFilledCircle(const Vec2& center, float radius, const SDL_Color& color)
{
    const float k_segments = 32.0f;
    const float k_increment = 2.0f * M_PI / k_segments;
    float sinInc = sinf(k_increment);
    float cosInc = cosf(k_increment);
    Vec2 v0 = center;
    Vec2 r1(cosInc, sinInc);
    Vec2 v1 = center + r1 * radius;
    for (int i = 0; i < k_segments; ++i)
    {
        // Perform rotation to avoid additional trigonometry.
        Vec2 r2;
        r2.x = cosInc * r1.x - sinInc * r1.y;
        r2.y = sinInc * r1.x + cosInc * r1.y;
        Vec2 v2 = center + r2 * radius;
        DrawFilledTriangle(v0, v1, v2, color, color, color);
        r1 = r2;
        v1 = v2;
    }
}

SDL_Texture* Renderer::LoadTexture(const char* fileName)
{
    std::string path = "src/assets/";
    path += fileName;
    SDL_Surface* surface = IMG_Load(path.c_str());
    
    if(!surface)
        //return EXIT_FAILURE;
        return 0;
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void Renderer::DrawTexture(SDL_Texture* texture, const Vec2& pos, const Vec2& size, float angle)
{
    SDL_Rect dsrect;
    dsrect.x = pos.x; dsrect.y = pos.y; dsrect.w = size.x; dsrect.h = size.y;
    SDL_RenderCopyEx(renderer, texture, NULL, &dsrect, angle * 180.0f * M_1_PI, NULL, SDL_FLIP_NONE);
}