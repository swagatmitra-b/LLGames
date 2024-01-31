#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#include "header.h"

int main () {
   const int screenWidth = 870;
   const int screenHeight = 500;
//    const char screenTitle[] = "Breakout";

   InitWindow(screenWidth, screenHeight, NULL);

   SetTargetFPS(60);  

   Platform p_base = { .posX = 100.0, .posY = screenHeight - p_base.height - 35, .width = 100, .height = 20 };
   Ball b_breaker = { .posX = p_base.posX + p_base.width / 2, .posY = p_base.posY - 20, .velocityX = 3, .velocityY = 4, .radius = 10 };

   int base_multiplier = 4;

   bool launch = false;
   bool middle = true;
   bool right = false;
   bool left = false;

   int score = 0;
   int lives = 7;

   const char lives_card[7];
   char gameOverMessage[] = "Game Over. Your score was ";

   int size = snprintf(NULL, 0, "%s%d", gameOverMessage, score);
   const char resultString[size + 1];
   
   Block blocks[NUM_ROWS][BLOCK_ROW_SIZE];

   for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < BLOCK_ROW_SIZE; j++) {
            blocks[i][j].width = BLOCK_WIDTH;
            blocks[i][j].height = BLOCK_HEIGHT;
            blocks[i][j].isHit = false;

            blocks[i][j].posX = j * (BLOCK_WIDTH + BLOCK_GAP_X);
            blocks[i][j].posY = i * (BLOCK_HEIGHT + BLOCK_GAP_Y);
        }
   }

   while(!WindowShouldClose()) {
    sprintf(lives_card, "%d", lives);
    BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText(&lives_card, 10, screenHeight - 20, 20, BLACK);
        DrawCircle(b_breaker.posX, b_breaker.posY, b_breaker.radius, RED);
        DrawRectangle(p_base.posX, p_base.posY, p_base.width, p_base.height, GREEN);
        for (int i = 0; i < NUM_ROWS; i++) {
            for (int j = 0; j < BLOCK_ROW_SIZE; j++) {
                if(!blocks[i][j].isHit) {
                    DrawRectangle(
                        blocks[i][j].posX,
                        blocks[i][j].posY,
                        blocks[i][j].width,
                        blocks[i][j].height,
                        DARKBLUE
                    );
                }
            }
        }
        if (hasCollided(b_breaker, blocks)) {
             b_breaker.velocityY *= -1;
             score += 1;
             if (score >= BLOCK_ROW_SIZE*NUM_ROWS) return 0;
        }
        if (IsKeyDown(KEY_LEFT)) {
            if (p_base.posX < 0) {
                p_base.posX = 0;
            }
            moveStatic(&p_base, &b_breaker, -base_multiplier, launch);
        }
        if (IsKeyDown(KEY_RIGHT)) {
             if (p_base.posX >= screenWidth - p_base.width) {
                p_base.posX = screenWidth - p_base.width;
            }
            moveStatic(&p_base, &b_breaker, base_multiplier, launch);
        }
        if (IsKeyDown(KEY_SPACE) && !launch) {
                 if (lives < 0) {
                    lives = 0;
                    score = 0;
                     for (int i = 0; i < NUM_ROWS; i++) {
                         for (int j = 0; j < BLOCK_ROW_SIZE; j++) {
                            if(blocks[i][j].isHit) blocks[i][j].isHit = false;
                         }
                     }
                 }
                 launch = true;
                 middle = true;
                 right = false;
                 left = false;
        }
        if (launch) {
            if (b_breaker.posY - b_breaker.radius < 0 ||
                b_breaker.posX >= p_base.posX && b_breaker.posX < p_base.posX + p_base.width && b_breaker.posY == p_base.posY) {
                b_breaker.velocityY *= -1;
            }
            if (b_breaker.posX <= p_base.posX + p_base.width && b_breaker.posX > p_base.posX + p_base.width/2 + p_base.width/8 && b_breaker.posY == p_base.posY) {
                b_breaker.velocityX = abs(b_breaker.velocityX);
                middle = false;
                right = true;
                left = false;
            }
            if (b_breaker.posX >= p_base.posX && b_breaker.posX < p_base.posX + p_base.width/2 - p_base.width/8 && b_breaker.posY == p_base.posY) {
                b_breaker.velocityX = abs(b_breaker.velocityX);
                middle = false;
                right = false;
                left = true;
            }
            if (b_breaker.posX + b_breaker.radius > screenWidth || b_breaker.posX - b_breaker.radius < 0) {
                b_breaker.velocityX *= -1;
            }
            moveBall(&b_breaker, p_base, middle, right, left);
        }
        if (b_breaker.posY + b_breaker.radius > screenHeight) {
            launch = false;
            b_breaker.posX = p_base.posX + p_base.width / 2;
            b_breaker.posY = p_base.posY - 20;
            lives--;
        }
         if (lives < 0) {
                sprintf(resultString, "%s%d", gameOverMessage, score);
                DrawText(&resultString, screenWidth/2 - screenWidth/4, screenHeight/2 - 10, 30, BLACK);
         }
    EndDrawing();
   }
   CloseWindow();
   return 0;
};

void moveStatic (Platform *base, Ball *breaker, int speed, bool launch) {
    base->posX += 1 * speed;
    if (launch) return;
    breaker->posX = base->posX + base->width / 2;
    breaker->posY = base->posY - 20;
}

void moveBall(Ball *breaker, Platform base, bool mid, bool right, bool left) {
    breaker->posY += -breaker->velocityY;
    if (mid) breaker->posX += 0;
    if (right) breaker->posX += breaker->velocityX;
    if (left) breaker->posX += -breaker->velocityX;
}

bool hasCollided (Ball breaker, Block blocks[NUM_ROWS][BLOCK_ROW_SIZE]) {
    for (int i = 0; i<NUM_ROWS; i++) {
        for(int j = 0; j<BLOCK_ROW_SIZE; j++) {
           if (!blocks[i][j].isHit) {
                Vector2 ball_position = {.x = breaker.posX, .y = breaker.posY};
                Rectangle block_position = {.x = blocks[i][j].posX, .y = blocks[i][j].posY, .height = blocks[i][j].height, .width = blocks[i][j].width};
                if (CheckCollisionCircleRec(ball_position, breaker.radius, block_position)) {
                    blocks[i][j].isHit = true; 
                    return true;
                }
            }
        }
    }
    return false;
}