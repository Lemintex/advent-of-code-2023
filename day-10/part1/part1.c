#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAP_SIZE 139

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

typedef struct
{
    char pipe;
    enum Direction incomingDirection[2];
    enum Direction outgoingDirection[2];
} pipe_type_t;

typedef struct
{
    bool visited[MAP_SIZE][MAP_SIZE];
    char map[MAP_SIZE][MAP_SIZE + 1]; // +1 for null terminator
} pipe_map_t;

pipe_type_t pipes[6] = {
    {'|', {UP, DOWN}, {UP, DOWN}},
    {'-', {LEFT, RIGHT}, {LEFT, RIGHT}},
    {'L', {DOWN, LEFT}, {RIGHT, UP}},
    {'J', {DOWN, RIGHT}, {LEFT, UP}},
    {'7', {UP, RIGHT}, {LEFT, DOWN}},
    {'F', {UP, LEFT}, {RIGHT, DOWN}}};

pipe_map_t pipe_map = {0};
int totalSteps = 0;

void MoveToNextPipe(int x, int y, enum Direction* movingDirection);
bool IsValidPipe(char pipe, enum Direction* direction);
void Start(int startX, int startY)
{
    totalSteps = 0;
    enum Direction movingDirection = DOWN;
    MoveToNextPipe(startX, startY, &movingDirection);
    if (totalSteps == 0)
    {
        movingDirection = UP;
        MoveToNextPipe(startX, startY, &movingDirection);
    }
    if (totalSteps == 0)
    {
        movingDirection = LEFT;
        MoveToNextPipe(startX, startY, &movingDirection);
    }
    if (totalSteps == 0)
    {
        movingDirection = RIGHT;
        MoveToNextPipe(startX, startY, &movingDirection);
    }
    totalSteps++;
}

void MoveToNextPipe(int x, int y, enum Direction* movingDirection)
{

    bool validPipeFound = false;
    bool visited = pipe_map.visited[y][x];
    char pipe;
    if (x < MAP_SIZE - 1)
    {
        pipe = pipe_map.map[y][x + 1];
        if (*movingDirection == RIGHT && IsValidPipe(pipe, movingDirection))
        {
            validPipeFound = true;
            pipe_map.visited[y][x] = true;
            MoveToNextPipe(x + 1, y, movingDirection);
        }
    }
    if (x > 0)
    {
        pipe = pipe_map.map[y][x - 1];
        if (*movingDirection == LEFT && IsValidPipe(pipe, movingDirection)) 
        {
            validPipeFound = true;
            pipe_map.visited[y][x] = true;
            MoveToNextPipe(x - 1, y, movingDirection);
        }
    }
    if (y < MAP_SIZE - 1)
    {
        pipe = pipe_map.map[y + 1][x];
        if (*movingDirection == DOWN && IsValidPipe(pipe, movingDirection))
        {
            validPipeFound = true;
            pipe_map.visited[y][x] = true;
            MoveToNextPipe(x, y + 1, movingDirection);
        }
    }
    if (y > 0)
    {
        pipe = pipe_map.map[y - 1][x];
        if (*movingDirection == UP && IsValidPipe(pipe, movingDirection))
        {
            validPipeFound = true;
            pipe_map.visited[y][x] = true;
            MoveToNextPipe(x, y - 1, movingDirection);
        }
    }
}

bool IsValidPipe(char pipe, enum Direction* direction)
{
    for (int i = 0; i < 6; i++)
    {
        if (pipes[i].pipe == pipe)
        {
            for (int j = 0; j < 2; j++)
            {
                if (pipes[i].incomingDirection[j] == *direction)
                {
                    totalSteps++;
                    *direction = pipes[i].outgoingDirection[j];
                    return true;
                }
            }
        }
    }
    return false;
}

int main()
{
    int x = 0, y = 0;
    FILE* input = fopen("../input.txt", "r");

    // big char array to hold the line
    char line[256];

    bool startFound = false;
    for (int i = 0; fgets(line, sizeof(line), input); i++)
    {
        strcpy(pipe_map.map[i], line);
        if (startFound)
        {
            continue;
        }
        for (int j = 0; j < MAP_SIZE; j++)
        {
            if (pipe_map.map[i][j] == 'S')
            {
                x = j;
                y = i;
                startFound = true;
                break;
            }
        }
    }
    Start(x, y);
    printf("Total steps: %d\n", totalSteps/2);
}