#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <ncurses.h>
#include <time.h>
#include <sys/ioctl.h>
#include "termSymbolNgn.h"

Matrix matrix;
Point snake[28*8];
Point pastSteps[2];
Point apple;
int key, score, direction;
int snakeLenth;

#define head snake[0]
#define up 0
#define right 1
#define down 2
#define left 3
#define snakeTexture '@'
#define appleTexture '+'

void setDirection()
{
        key = getch();
        if(key == KEY_UP && direction != down) direction = up;
        if(key == KEY_DOWN && direction != up) direction = down;
        if(key == KEY_LEFT && direction != right) direction = left;
        if(key == KEY_RIGHT && direction != left) direction = right;
}

void refreshApple()
{
        srand(time(NULL));
        apple.x = rand() % 58 + 1;
        apple.y = rand() % 28 + 1;
}

void refreshSnake()
{
        for(int i = 1; i < snakeLenth; i++)
        {
                copyPosition(snake[i], &pastSteps[1]);
                copyPosition(pastSteps[0], &snake[i]);
                copyPosition(pastSteps[1], &pastSteps[0]);
        }
}

void start()
{
        int speed = 400000;
        snakeLenth = 5;
        head.x = 30;
        head.y = 15;
        for(int i = 1; i < snakeLenth; i++)
        {
                snake[i].y = 0;
                snake[i].x = 0;
        }

        while(1)
        {
                refreshMatrix(&matrix);
                setPoint(&matrix, apple, appleTexture);
                for(int i = 0; i < snakeLenth; i++)
                        setPoint(&matrix, snake[i], snakeTexture);
                printMatrix(matrix);
                usleep(speed);

                if(kbhit()) setDirection();

                copyPosition(head, &pastSteps[0]);
                switch(direction)
                {
                        case up:
                                head.y--;
                                break;
                        case right:
                                head.x++;
                                break;
                        case down:
                                head.y++;
                                break;
                        case left:
                                head.x--;
                                break;
                }

                if(matrix.matrix[head.y][head.x] == '#' || 
                                matrix.matrix[head.y][head.x] == '@')
                {

                        showGameOver(score);
                        score = 0;
                        break;
                }

                if(matrix.matrix[head.y][head.x] == '+')
                {
                        score++;
                        snakeLenth++;
                        speed -= 10000; 
                        refreshApple();
                }
                refreshSnake();
        }
}

int main()
{
        int selected;

        initscr();
        keypad(stdscr, true);
        refreshApple();

        while(selected != 2)
        {
                selected = showMenu();
                if(selected == 1) start();
        }

        endwin();
        return 0;
}
