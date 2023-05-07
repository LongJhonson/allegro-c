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
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include "./functions.h"

const float FPS = 60;
const int CIRCLE_SIZE = 50;
const int POINT_SIZE = 30;
const int offset = 60;
const int SCREEN_HEIGHT = 720;
const int SCREEN_WIDTH = 1280;
const int TURBO_SPEEd = 5;


// function for player movement

int main(int argc, char *argv[])
{
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_KEYBOARD_STATE wait_key_State;

	int pulsed_key = 0;
	

	bool running = true;
	bool redraw = true;
	bool pause = false;

	int score = 0;
	int posx = 50, posy = 50;

	float initial_enemy_speed = 1.0;
	float enemy_speed = 1.0;
	float enemy_speed_increase = 0.1;
	int enemy_count = 0;
	bool enemy_alive = false;
	int enemy_x, enemy_y;

	int best_score = 0;
	int turbo = 3;
	bool turbo_active = false;

	bool point_alive = false;
	int point_x, point_y;

	bool show_overlay = false;

	// Initialize allegro
	if (!al_init())
	{
		fprintf(stderr, "Failed to initialize allegro.\n");
		return 1;
	}
	ALLEGRO_COLOR OVERLAY_COLOR = al_map_rgba(255, 100, 100, 0.5); //tiene que ir despues de al_init

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
	display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
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
			controls(&running, &show_overlay, &pause, &pulsed_key, &wait_key_State);
			if (!pause)
			{
				movePlayer(&posx, &posy, &wait_key_State);

				if (!point_alive)
				{
					point_alive = true;
					point_x = generateRandom(0, SCREEN_WIDTH);
					point_y = generateRandom(0, SCREEN_HEIGHT);
				}
				if (!enemy_alive)
				{
					enemy_x = generateRandom(0, 800);
					enemy_y = generateRandom(0, 600);
					enemy_alive = true;
				}
				else
				{
					moveEnemyToPlayer(posx, posy, &enemy_x, &enemy_y, enemy_speed);
					if (checkCollision(enemy_x, enemy_y, posx, posy, CIRCLE_SIZE))
					{
						printf("GAME OVER\n");
						enemy_alive = false;
						enemy_speed = initial_enemy_speed;
						if (score > best_score)
						{
							best_score = score;
						}
						score = 0;
					}
				}
				if (checkCollision(point_x, point_y, posx, posy, POINT_SIZE))
				{
					score++;
					enemy_speed += enemy_speed_increase;
					point_alive = false;
				}
			}
			al_draw_filled_circle(posx, posy, CIRCLE_SIZE, al_map_rgb(255, 255, 100));		 // personaje
			al_draw_filled_circle(enemy_x, enemy_y, CIRCLE_SIZE, al_map_rgb(255, 255, 255)); // enemigo
			al_draw_filled_circle(point_x, point_y, POINT_SIZE, al_map_rgb(255, 100, 255));	 // enemigo

			if (show_overlay)
			{
				al_draw_textf(al_create_builtin_font(), OVERLAY_COLOR, 10, 10, 0, "Score: %d", score);
				al_draw_textf(al_create_builtin_font(), OVERLAY_COLOR, 10, 20, 0, "Enemy speed: %.1f", enemy_speed);
				if (best_score > 0)
				{
					al_draw_textf(al_create_builtin_font(), OVERLAY_COLOR, 10, 30, 0, "Best score: %d", best_score);
				}
			}
			if(pause){
				al_draw_textf(al_create_builtin_font(), OVERLAY_COLOR, 10, SCREEN_HEIGHT - 10, 0, "GAME PAUSED");
			}
			al_flip_display();
			redraw = false;
		}
	}

	// Clean up
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}
