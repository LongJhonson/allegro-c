#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printRandoms(int lower, int upper,
                  int count)
{
    int i;
    for (i = 0; i < count; i++)
    {
        int num = (rand() %
                   (upper - lower + 1)) +
                  lower;
        printf("%d ", num);
    }
}

int generateRandom(int lower, int upper)
{
    int num = (rand() %
               (upper - lower + 1)) +
              lower;
    return num;
}

bool checkCollision(int x1, int y1, int x2, int y2, int offset)
{
    if (x1 + offset > x2 - offset && x1 - offset < x2 + offset)
    {
        if (y1 + offset > y2 - offset && y1 - offset < y2 + offset)
        {
            return true;
        }
    }
    return false;
};

void moveEnemyToPlayer(int player_x, int player_y, int *enemy_x, int *enemy_y, float enemy_speed)
{
    if (*enemy_x < player_x)
    {
        *enemy_x += enemy_speed;
    }
    if (*enemy_x > player_x)
    {
        *enemy_x -= enemy_speed;
    }
    if (*enemy_y < player_y)
    {
        *enemy_y += enemy_speed;
    }
    if (*enemy_y > player_y)
    {
        *enemy_y -= enemy_speed;
    }
};

void movePlayer(int *posx, int *posy, ALLEGRO_KEYBOARD_STATE *keyState)
{
    if (al_key_down(keyState, ALLEGRO_KEY_UP))
    {
        *posy -= 10;
    }

    if (al_key_down(keyState, ALLEGRO_KEY_DOWN))
    {
        *posy += 10;
    }

    if (al_key_down(keyState, ALLEGRO_KEY_LEFT))
    {
        *posx -= 10;
    }

    if (al_key_down(keyState, ALLEGRO_KEY_RIGHT))
    {
        *posx += 10;
    }
}

void controls(bool *running, bool *show_overlay, bool *pause, int *pulsed_key, ALLEGRO_KEYBOARD_STATE *keyState)
{
    if (al_key_down(keyState, ALLEGRO_KEY_ESCAPE))
    {
        *running = false;
    }
    if (al_key_down(keyState, ALLEGRO_KEY_SPACE))
    {
        printf("Space pressed\n");
        *show_overlay = true;
    }
    if (al_key_down(keyState, ALLEGRO_KEY_Q))
    {
        printf("Q pressed\n");
        *show_overlay = false;
    }
    if (al_key_down(keyState, ALLEGRO_KEY_W))
    {
        *pause = !*pause;
        // *pulsed_key = ALLEGRO_KEY_W;
    }
}

// function to check collision with point
void checkPointCollision(int *score, int *point_x, int *point_y, int *posx, int *posy, bool *point_alive, int offset)
{
    if (checkCollision(*posx, *posy, *point_x, *point_y, offset))
    {
        *score += 1;
        *point_alive = false;
    }
}