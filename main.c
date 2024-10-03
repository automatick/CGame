#include <stdio.h>
#include <raylib.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define uint unsigned int

#define WINDOW_WIDTH        800
#define WINDOW_HEIGHT       600
#define FPS                  60
#define STANDARD_HP          20
#define STANDARD_SPEED        3

typedef struct {
    Rectangle bound     ;
    Color currentColor  ;
    Color hoveredColor  ;
    Color standardColor ;
    char text[50]       ;
} Button;

typedef struct {
    Rectangle bound     ;
    Color currentColor  ;
    uint hp             ;
    uint speed          ;
} Player;

typedef struct {
    Rectangle bound     ;
    Color currentColor  ;
} Wall;

typedef void (*Callback)(void);

Color brightness(Color color, const char mod) {
    color.r = (color.r + mod > 255) ? 255 : (color.r + mod < 0) ? 0 : color.r + mod; color.g = (color.g + mod > 255) ? 255 : (color.g + mod < 0) ? 0 : color.g + mod; color.b = (color.b + mod > 255) ? 255 : (color.b + mod < 0) ? 0 : color.b + mod;
    return color;
}
Color darken(Color const color) {
    return brightness(color, -30);
}
Color lighten(Color const color) {
    return brightness(color, 30);
}

Color invertColor(Color const color) {
    return (Color) {
        255 - color.r,
        255 - color.g,
        255 - color.b,
        color.a
    };
}

Wall wallInit( Rectangle const *bound, Color const *currentColor ) {
    Wall wall                               ;
    wall.bound = *bound                     ;
    wall.currentColor = *currentColor       ;
    return wall                             ;
}

Player playerInit( Rectangle const *bound, Color const *currentColor, const uint hp, const uint speed ) {
    Player player                               ;
    player.bound            = *bound            ;
    player.currentColor     = *currentColor     ;
    player.hp               = hp                ;
    player.speed            = speed             ;
    return player                               ;
}

Vector2 calculatePlayerCenter(Player const *player) {
    return (Vector2){ player->bound.x + player->bound.width / 2.0f, player->bound.y + player->bound.height / 2.0f };
}

Button buttonInit(const Rectangle *bound, const Color *color,const Color *hoveredColor, const char text[]) {
    Button button                                   ;
    button.bound                = *bound            ;
    button.currentColor         = *color            ;
    button.hoveredColor         = *hoveredColor     ;
    button.standardColor        = *color            ;
    strcpy( button.text,          text )            ;
    return button                                   ;
}

bool updateButton(Button *button) {

    const Vector2   mousePos  = GetMousePosition()                                  ;
    const bool      isHovered = CheckCollisionPointRec(mousePos, button->bound)     ;
    const bool      isPressed = IsMouseButtonDown(MOUSE_BUTTON_LEFT)                ;

    if (isHovered) button->currentColor = button->hoveredColor      ;
    else button->currentColor = button->standardColor               ;
    if (isPressed && isHovered) return true                         ;
    return false                                                    ;
}

void drawButton(const Button *button) {
    const unsigned int      textWidth           = MeasureText(button->text, 20)            ;
    const Color             outlineColor        = darken(button->currentColor)             ;
    DrawRectangle(button->bound.x, button->bound.y, button->bound.width, button->bound.height, button->currentColor);
    DrawRectangleLines(button->bound.x, button->bound.y, button->bound.width, button->bound.height, outlineColor);
    DrawText(button->text, button->bound.x + (button->bound.width - textWidth) / 2, button->bound.y + (button->bound.height - 20) / 2, 20, invertColor(button->currentColor));
}

void mainMenu (const int scrWidth, const int scrHeight) {
    const Rectangle     buttonPosS      = {scrWidth / 2 - 50 , scrHeight / 2 - 50, 100, 50    }       ;
    const Rectangle     buttonPosE      = {scrWidth / 2 - 50 , scrHeight / 2, 100, 50         }       ;
    Button              start           = buttonInit( &buttonPosS, &BLACK, &DARKGRAY, "Start" )       ;
    Button              exit            = buttonInit( &buttonPosE, &BLACK, &DARKGRAY, "Exit"  )       ;
    bool                gameRunning     = false                                                       ;
    while (!gameRunning && !WindowShouldClose()) {
        gameRunning = updateButton(&start);
        if (updateButton(&exit)) {
            CloseWindow();
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);
        drawButton(&start);
        drawButton(&exit);
        EndDrawing();
    }
}

void drawPlayer(const Player *player) {
    DrawRectangleRec(player->bound, RED);
}

void keyHandle(Player *player, Wall walls[5]) {

    Vector2 nextStep = { player->bound.x, player->bound.y }     ;

    if (IsKeyDown(KEY_A)) nextStep.x += (5 * player->speed)     ;
    if (IsKeyDown(KEY_D)) nextStep.x -= (5 * player->speed)     ;
    if (IsKeyDown(KEY_S)) nextStep.y -= (5 * player->speed)     ;
    if (IsKeyDown(KEY_W)) nextStep.y += (5 * player->speed)     ;
    for (uint i = 0; i < 5; i++) {
        if (CheckCollisionRecs(player->bound, walls[i].bound)) {
            return;
        }
    }
    player->bound = (Rectangle){nextStep.x, nextStep.y, player->bound.width, player->bound.height};
}

void mainGameLoop(const int scrWidth, const int scrHeight) {

    Rectangle const pBound          = {100, 100, 50, 50}                                            ;
    Player player                   = playerInit(&pBound, &BLACK, STANDARD_HP, STANDARD_SPEED)      ;
    Wall walls[5]                   = {wallInit(&(Rectangle){20, 20, 30, 30}, &DARKGRAY)}           ;

    Camera2D camera             = { 0 }                                             ;
    camera.rotation             = 0.0f                                              ;
    camera.target               = calculatePlayerCenter(&player)                    ;
    camera.offset               = (Vector2){ scrWidth / 2.0f, scrHeight / 2.0f }    ;
    camera.zoom                 = 1.0f                                              ;

    while (!WindowShouldClose()) {
        keyHandle(&player, walls)                                   ;
        camera.target = calculatePlayerCenter(&player)              ;
        for (uint i = 0; i < 5; i++) {
            DrawRectangleRec(walls[i].bound, walls[i].currentColor) ;
        }
        BeginDrawing()                                              ;
        ClearBackground(RAYWHITE)                                   ;
        BeginMode2D(camera)                                         ;
        drawPlayer(&player)                                         ;
        DrawRectangleRec((Rectangle){100, 100, 50, 50}, BLUE)       ;
        EndMode2D()                                                 ;
        EndDrawing()                                                ;
    }
}

int main(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game");
    SetTargetFPS(FPS);
    mainMenu(WINDOW_WIDTH, WINDOW_HEIGHT);
    mainGameLoop(WINDOW_WIDTH, WINDOW_HEIGHT);
    CloseWindow();
    return 0;
}
