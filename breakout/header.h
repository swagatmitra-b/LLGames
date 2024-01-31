
#define BLOCK_ROW_SIZE 25
#define NUM_ROWS 6
#define BLOCK_WIDTH 30
#define BLOCK_HEIGHT 15
#define BLOCK_GAP_X 5
#define BLOCK_GAP_Y 5

typedef struct {
    float posX;
    float posY;
    int width;
    int height;
} Platform;

typedef struct {
    float posX;
    float posY;
    int width;
    int height;
    bool isHit;
} Block;

typedef struct {
    float posX;
    float posY;
    int velocityX;
    int velocityY;
    int radius;
} Ball;

void moveStatic (Platform *base, Ball *breaker, int speed, bool launch);
void moveBall(Ball *breaker, Platform base, bool mid, bool right, bool left);
bool hasCollided (Ball breaker, Block blocks[NUM_ROWS][BLOCK_ROW_SIZE]);
