#pragma once

#include "SDL2/SDL.h"
#include "Vec2.h"

class Renderer
{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Color clearColor;

public:
    Renderer();

    ~Renderer();

    void Clear();

    void Present();

    void SetDrawColor(const SDL_Color& color);

    void DrawLine(const Vec2& v1, const Vec2& v2, const SDL_Color& color);

    void DrawFilledTriangle(const Vec2& v1, const Vec2& v2, const Vec2& v3, const SDL_Color& color1, const SDL_Color& color2, const SDL_Color& color3);

    void DrawRect(const Vec2& pos, const Vec2& size, const SDL_Color& color);

    void DrawCircle(const Vec2& center, float radius, const SDL_Color& color);

    void DrawFilledCircle(const Vec2& center, float radius, const SDL_Color& color);

    SDL_Texture* LoadTexture(const char* fileName);

    void DrawTexture(SDL_Texture* texture, const Vec2& pos, const Vec2& size, float angle);
};

