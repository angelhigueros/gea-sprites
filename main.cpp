#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float GROUND_LEVEL = 500.0f;

struct Cloud {
    float x, y;
    float speed;
};

class Background {
public:
    Background(SDL_Renderer* renderer) : renderer(renderer) {
        castleRect = {100, 200, 600, 300};

        clouds = {
            {50, 100, 30},
            {300, 150, 20},
            {600, 120, 25}
        };
    }

    void renderBackground() {
        SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255);
        SDL_Rect groundRect = {0, WINDOW_HEIGHT - 100, WINDOW_WIDTH, 100};
        SDL_RenderFillRect(renderer, &groundRect);

        SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);
        SDL_RenderFillRect(renderer, &castleRect);
    }

    void renderClouds(float deltaTime) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (auto& cloud : clouds) {
            cloud.x += cloud.speed * deltaTime;
            if (cloud.x > WINDOW_WIDTH) {
                cloud.x = -100;
            }
            SDL_Rect cloudRect = {static_cast<int>(cloud.x), static_cast<int>(cloud.y), 100, 50};
            SDL_RenderFillRect(renderer, &cloudRect);
        }
    }

private:
    SDL_Renderer* renderer;
    SDL_Rect castleRect;
    std::vector<Cloud> clouds;
};

class Character {
public:
    Character(SDL_Renderer* renderer, float x, float y)
        : renderer(renderer), x(x), y(y), originalY(y), velocity(0.0f), isJumping(false), animationFrame(0) {}

    void startJump() {
        if (!isJumping) {
            isJumping = true;
            velocity = initialJumpVelocity;
        }
    }

    void moveLeft(float deltaTime) {
        x -= horizontalSpeed * deltaTime;
        if (x < 0) {
            x = 0;
        }
    }

    void moveRight(float deltaTime) {
        x += horizontalSpeed * deltaTime;
        if (x > WINDOW_WIDTH - characterWidth) {
            x = WINDOW_WIDTH - characterWidth;
        }
    }

    void update(float deltaTime) {
        if (isJumping) {
            velocity += gravity * deltaTime;
            y += velocity * deltaTime;

            animationFrame = (animationFrame + 1) % 20;

            if (y >= originalY) {
                y = originalY;
                isJumping = false;
                velocity = 0.0f;
                animationFrame = 0;
            }
        }
    }

    void renderCharacter() {
        int renderX = static_cast<int>(x);
        int renderY = static_cast<int>(y);

        int armOffset = 0;
        int legOffset = 0;

        if (isJumping) {
            armOffset = (animationFrame % 10) - 5;
            legOffset = ((animationFrame / 2) % 5) - 2;
        }

        SDL_SetRenderDrawColor(renderer, 255, 223, 0, 255);
        SDL_Rect crownRect = {renderX + 8, renderY - 8, 16, 8};
        SDL_RenderFillRect(renderer, &crownRect);

        SDL_SetRenderDrawColor(renderer, 255, 182, 193, 255);
        SDL_Rect headRect = {renderX + 8, renderY, 16, 16};
        SDL_RenderFillRect(renderer, &headRect);

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_Rect bodyRect = {renderX + 8, renderY + 16, 16, 24};
        SDL_RenderFillRect(renderer, &bodyRect);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect leftArmRect = {renderX + armOffset, renderY + 16, 8, 16};
        SDL_Rect rightArmRect = {renderX + 24 - armOffset, renderY + 16, 8, 16};
        SDL_RenderFillRect(renderer, &leftArmRect);
        SDL_RenderFillRect(renderer, &rightArmRect);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect leftLegRect = {renderX + 8 + legOffset, renderY + 40, 8, 16};
        SDL_Rect rightLegRect = {renderX + 16 - legOffset, renderY + 40, 8, 16};
        SDL_RenderFillRect(renderer, &leftLegRect);
        SDL_RenderFillRect(renderer, &rightLegRect);
    }

private:
    SDL_Renderer* renderer;
    float x, y;
    float originalY;
    float velocity;
    bool isJumping;
    int animationFrame;
    const float gravity = 2000.0f;
    const float initialJumpVelocity = -900.0f;
    const float horizontalSpeed = 800.0f;
    const int characterWidth = 32;
};

class Dog {
public:
    Dog(SDL_Renderer* renderer, float x, float y)
        : renderer(renderer), x(x), y(y), movingRight(true) {}

    void update(float deltaTime) {
        // Mover el perro de un lado a otro
        if (movingRight) {
            x += speed * deltaTime;
            if (x > WINDOW_WIDTH - dogWidth) {
                x = WINDOW_WIDTH - dogWidth;
                movingRight = false;
            }
        } else {
            x -= speed * deltaTime;
            if (x < 0) {
                x = 0;
                movingRight = true;
            }
        }
    }

    void renderDog() {
        int renderX = static_cast<int>(x);
        int renderY = static_cast<int>(y);

        SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
        SDL_Rect headRect = {renderX, renderY, 16, 16};
        SDL_RenderFillRect(renderer, &headRect);

        SDL_SetRenderDrawColor(renderer, 160, 82, 45, 255);
        SDL_Rect bodyRect = {renderX - 8, renderY + 16, 32, 16};
        SDL_RenderFillRect(renderer, &bodyRect);

        SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); 
        SDL_Rect leftLeg = {renderX - 6, renderY + 32, 8, 8};
        SDL_Rect rightLeg = {renderX + 14, renderY + 32, 8, 8};
        SDL_RenderFillRect(renderer, &leftLeg);
        SDL_RenderFillRect(renderer, &rightLeg);

        SDL_SetRenderDrawColor(renderer, 160, 82, 45, 255);
        SDL_Rect tailRect = {renderX - 10, renderY + 20, 8, 4};
        SDL_RenderFillRect(renderer, &tailRect);
    }

private:
    SDL_Renderer* renderer;
    float x, y;
    bool movingRight;
    const float speed = 200.0f;  
    const int dogWidth = 32;
};

class DialogSystem {
public:
    DialogSystem(SDL_Renderer* renderer) : renderer(renderer), dialogIndex(0), showFinalScreen(false) {
        if (TTF_Init() < 0) {
            std::cerr << "Error inicializando SDL_ttf: " << TTF_GetError() << std::endl;
        }
        font = TTF_OpenFont("fuente.ttf", 24);
        if (!font) {
            std::cerr << "Error cargando la fuente: " << TTF_GetError() << std::endl;
        }
        dialogs = {
            "Aventurero, salvameee, estoy atrapada en el castillo",
            "Supera los desafios para poder salvarme",
            "Cuento contigo, suerte"
        };
    }

    ~DialogSystem() {
        TTF_CloseFont(font);
        TTF_Quit();
    }

    void advanceDialog() {
        if (dialogIndex < dialogs.size() - 1) {
            dialogIndex++;
        } else {
            showFinalScreen = true;
        }
    }

    void renderDialog() {
        if (showFinalScreen) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            return;
        }

        SDL_Color color = {255, 255, 255};
        SDL_Surface* surface = TTF_RenderText_Blended(font, dialogs[dialogIndex].c_str(), color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        int textWidth, textHeight;
        SDL_QueryTexture(texture, NULL, NULL, &textWidth, &textHeight);
        SDL_Rect textRect = {20, WINDOW_HEIGHT - 50, textWidth, textHeight};

        SDL_RenderCopy(renderer, texture, NULL, &textRect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    bool isFinalScreen() const { return showFinalScreen; }

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    std::vector<std::string> dialogs;
    int dialogIndex;
    bool showFinalScreen;
};

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Error inicializando SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Castle Background with Clouds and Jumping Character", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Background background(renderer);
    Character character(renderer, 350.0f, GROUND_LEVEL);
    Dog dog(renderer, 100.0f, GROUND_LEVEL);
    DialogSystem dialogSystem(renderer);

    bool running = true;
    Uint32 lastTime = SDL_GetTicks();
    SDL_Event event;

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_UP) {
                    character.startJump();
                } else if (event.key.keysym.sym == SDLK_RIGHT) {
                    character.moveRight(deltaTime);
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    character.moveLeft(deltaTime);
                }
            } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                dialogSystem.advanceDialog();
            }
        }

        if (dialogSystem.isFinalScreen()) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
        } else {
            character.update(deltaTime);
            dog.update(deltaTime);

            background.renderBackground();
            background.renderClouds(deltaTime);
            character.renderCharacter();
            dog.renderDog();
            dialogSystem.renderDialog();
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
