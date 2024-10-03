#include <stdio.h>
#include <raylib.h>
#include <stdbool.h>
#include <string.h>

#define uint unsigned int

#define FPS                  60
#define WINDOW_WIDTH        800
#define WINDOW_HEIGHT       600
#define STANDARD_HP          20
#define STANDARD_SPEED       10

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

Player playerInit( Rectangle const *bound, Color const *currentColor, const uint hp, const uint speed ) {
    Player player                               ;
    player.bound            = *bound            ;
    player.currentColor     = *currentColor     ;
    player.hp               = hp                ;
    player.speed            = speed             ;
    return player                               ;
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
    const unsigned int      textWidth           = MeasureText(button->text, 20)     ;
    const Color             outlineColor        = darken(button->currentColor)             ;
    DrawRectangle(button->bound.x, button->bound.y, button->bound.width, button->bound.height, button->currentColor);
    DrawRectangleLines(button->bound.x, button->bound.y, button->bound.width, button->bound.height, outlineColor);
    DrawText(button->text, button->bound.x + (button->bound.width - textWidth) / 2, button->bound.y + (button->bound.height - 20) / 2, 20, invertColor(button->currentColor));
}

void mainMenu (const int scrWidth, const int scrHeight) {
    const Rectangle     buttonPosS      = {scrWidth / 2 - 50 , scrHeight / 2 - 50, 100, 50 }        ;
    const Rectangle     buttonPosE      = {scrWidth / 2 - 50 , scrHeight / 2, 100, 50      }        ;
    Button              start           = buttonInit( &buttonPosS, &BLACK, &DARKGRAY, "Start")   ;
    Button              exit            = buttonInit( &buttonPosE, &BLACK, &DARKGRAY, "Exit")    ;
    bool                gameRunning     = false                                                                     ;
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

void inline drawPlayer(const Player *player) {}

void mainGameLoop(const int scrWidth, const int scrHeight) {
    Rectangle const pBound = {100, 100, 100, 50};
    Player player = playerInit(&pBound, &BLACK, STANDARD_HP, STANDARD_SPEED);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
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
