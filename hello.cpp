/*
 * This program uses the Allegro game library to display a blank window.
 *
 * It initializes the display and starts up the main game loop. The
 * game loop only checks for two events: timer (determined by the FPS)
 * and display close (when the user tries to close the window).
 *
 * http://www.damienradtke.org/building-a-mario-clone-with-allegro
 */

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "./functions.h"

const float FPS = 60;
const int CIRCLE_SIZE = 50;
const int offset = 10;

int main(int argc, char *argv[])
{
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_KEYBOARD_STATE wait_key_State;

	bool running = true;
	bool redraw = true;
	float enemy_speed = 1.0;
	int score = 0;
	int posx = 50, posy = 50;
	int enemy_count = 0;
	bool enemy_alive = false;
	int enemy_x, enemy_y;

	// Initialize allegro
	if (!al_init())
	{
		fprintf(stderr, "Failed to initialize allegro.\n");
		return 1;
	}

	if (!al_install_keyboard())
	{
		fprintf(stderr, "Failed to initialize keyboard.\n");
		return 1;
	}

	// Initialize allegro primitives
	if (!al_init_primitives_addon())
	{
		fprintf(stderr, "Failed to initialize allegro primitives.\n");
		return 1;
	}

	// Initialize the timer
	timer = al_create_timer(1.0 / FPS);
	if (!timer)
	{
		fprintf(stderr, "Failed to create timer.\n");
		return 1;
	}

	// Create the display
	display = al_create_display(800, 600);
	if (!display)
	{
		fprintf(stderr, "Failed to create display.\n");
		return 1;
	}

	// Create the event queue
	event_queue = al_create_event_queue();
	if (!event_queue)
	{
		fprintf(stderr, "Failed to create event queue.");
		return 1;
	}

	// Register event sources
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	// Display a black screen
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();

	// Start the timer
	al_start_timer(timer);

	// Game loop
	while (running)
	{
		ALLEGRO_EVENT event;
		ALLEGRO_TIMEOUT timeout;

		// Initialize timeout
		al_init_timeout(&timeout, 0.06);
		al_get_keyboard_state(&wait_key_State);

		if (al_key_down(&wait_key_State, ALLEGRO_KEY_ESCAPE))
		{
			running = false;
		}

		if (al_key_down(&wait_key_State, ALLEGRO_KEY_UP))
		{
			posy -= 10;
		}

		if (al_key_down(&wait_key_State, ALLEGRO_KEY_DOWN))
		{
			posy += 10;
		}

		if (al_key_down(&wait_key_State, ALLEGRO_KEY_LEFT))
		{
			posx -= 10;
		}

		if (al_key_down(&wait_key_State, ALLEGRO_KEY_RIGHT))
		{
			posx += 10;
		}

		if (al_key_down(&wait_key_State, ALLEGRO_KEY_SPACE))
		{
			printf("Space pressed\n");
			score++;
			enemy_alive = false;
		}

		if (al_key_down(&wait_key_State, ALLEGRO_KEY_Q))
		{
			enemy_speed += 0.1;
		}

		// Fetch the event (if one exists)
		bool get_event = al_wait_for_event_until(event_queue, &event, &timeout);

		// Handle the event
		if (get_event)
		{
			switch (event.type)
			{
			case ALLEGRO_EVENT_TIMER:
				redraw = true;
				break;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				running = false;
				break;
			default:
				fprintf(stderr, "Unsupported event received: %d\n", event.type);
				break;
			}
		}

		// Check if we need to redraw
		if (redraw && al_is_event_queue_empty(event_queue))
		{
			// Redraw
			al_clear_to_color(al_map_rgb(0, 0, 0));
			// al_draw_line(10, 10, 100, 10, al_map_rgb(0, 255, 255), 1);
			al_draw_filled_circle(posx, posy, CIRCLE_SIZE, al_map_rgb(255, 255, 255)); // personaje
			if (!enemy_alive)
			{
				enemy_x = generateRandom(0, 800);
				enemy_y = generateRandom(0, 600);
				enemy_alive = true;
			}
			else
			{
				if (enemy_x + CIRCLE_SIZE < posx - CIRCLE_SIZE)
				{
					enemy_x += enemy_speed;
				}
				if (enemy_x - CIRCLE_SIZE > posx + CIRCLE_SIZE)
				{
					enemy_x -= enemy_speed;
				}
				if (enemy_y + CIRCLE_SIZE < posy - CIRCLE_SIZE)
				{
					enemy_y += enemy_speed;
				}

				if (enemy_y - CIRCLE_SIZE > posy + CIRCLE_SIZE)
				{
					enemy_y -= enemy_speed;
				}
				if (enemy_x + CIRCLE_SIZE < posx - CIRCLE_SIZE && enemy_y + CIRCLE_SIZE < posy - CIRCLE_SIZE)//MAL
				{
					printf("GAME OVER\n");
					enemy_alive = false;
				}
				// else
				// {
				// 	printf("GAME OVER\n");
				// 	enemy_alive = false;
				// }
			}

			// function to check collision btween 2 circles

			al_draw_filled_circle(enemy_x, enemy_y, CIRCLE_SIZE, al_map_rgb(255, 255, 255)); // personaje

			al_flip_display();
			redraw = false;
		}
		// al_draw_line(100, 100, 400, 400, al_map_rgb(255, 0, 0), 3);
		// al_draw_filled_circle(400, 300, 50, al_map_rgb(0, 255, 255));
	}

	// Clean up
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}
