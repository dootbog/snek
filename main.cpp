#include "raylib.h"
#include <stdio.h>

#define SWIDE 800
#define STALL 450
#define STITLE "SNEK!"
#define START_CELLS 10
#define MAX_CELLS 99

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))
#define lerp(a,b,t) ((1-t)*a+t*b)
#define clamp(n,lo,hi) (((n)>(hi))?(hi):(((n)<(lo))?(lo):(n)))

enum screen_state {menu, gameplay, paused, gameover};
struct foody { int x, y, w, h; Color color; };
struct snakey { int x, dx, y, dy, w, h, speed, cells; };
struct v2 { int x, y; };
char score[99];
const int go_up    = KEY_UP;
const int go_left  = KEY_LEFT;
const int go_down  = KEY_DOWN; 
const int go_right = KEY_RIGHT;
screen_state screen = gameplay;
foody eat = {.w=8, .h=10, .x=0, .y=0, .color=RED};
snakey me = {.x=0, .dx=0, .y=0, .dy=0, .w=8, .h=10, .speed=1, .cells=START_CELLS};
v2 us[MAX_CELLS];

int snap(int n, int g)
{
	return ((int)n/g)*g;
}

void move_food()
{
	eat.x = snap(GetRandomValue(40, SWIDE-40), me.w);
	eat.y = 0;
}

void update()
{
	bool hit_wall = false;
	if (gameplay != screen) return;
	float t = 0.3;
	eat.y = lerp(eat.y, eat.y + me.speed, t);
	me.x = clamp(lerp(me.x, me.x + me.dx, t), 0, SWIDE-me.w);
	me.y = clamp(lerp(me.y, me.y + me.dy, t), me.h, STALL-me.h);
	hit_wall = me.x == 0 || me.x >= SWIDE-me.w || me.y == me.h || me.y >= STALL-me.h;
	us[0] = {.x=me.x, .y=me.y};
	for (int i = me.cells-1; i > 0; i--) us[i] = us[i-1];
	Rectangle a = {(float)me.x, (float)me.y, (float)me.w, (float)me.h};
	Rectangle b = {(float)eat.x, (float)eat.y, (float)eat.w, (float)eat.h};
	if (CheckCollisionRecs(a, b)) {
		move_food();
		me.cells = clamp(1 + me.cells, 1, MAX_CELLS-1);
	}
	if (eat.y > STALL) {
		move_food();
		me.cells = clamp((int)me.cells/3, 1, me.cells);
	}
	if (1 == me.cells || hit_wall)
	{
		screen = gameover;
		me.cells = START_CELLS;
		me.x=(int)SWIDE/2; me.y=(int)STALL/2;
	}
}

void draw_game_over()
{
	ClearBackground(RED);
	DrawText("YOU DIED", SWIDE-(SWIDE/3), STALL-STALL/3, 32, WHITE);
	DrawText("PRESS P TO PLAY AGAIN", SWIDE-(SWIDE/3), STALL-STALL/4, 12, WHITE);
}

void draw()
{
	if (gameover == screen)
	{
		draw_game_over();
		return;
	}
	ClearBackground(BLACK);
	snprintf(score, 3, "%d", me.cells);
	DrawText(score, me.w, me.h, 16, GREEN);
	DrawRectangle(eat.x, eat.y, eat.w, eat.h, eat.color);
	for (int i=0; i<me.cells; i++)
	{
		v2 u = us[i];
		DrawRectangle(u.x, u.y, me.w, me.h, DARKGRAY);
	}
	if (paused == screen)
	{
		DrawText("PAUSED", SWIDE-(SWIDE/3), STALL-STALL/3, 32, WHITE);
	}
}

void inputs()
{
	if (IsKeyPressed(go_left))  { me.dx = me.dx - me.speed; me.dy = 0; }
	if (IsKeyPressed(go_up))    { me.dy = me.dy - me.speed; me.dx = 0; }
	if (IsKeyPressed(go_right)) { me.dx = me.dx + me.speed; me.dy = 0; }
	if (IsKeyPressed(go_down))  { me.dy = me.dy + me.speed; me.dx = 0; }
	if (IsKeyPressed(KEY_F)) ToggleFullscreen();
	if (IsKeyPressed(KEY_R)) move_food();
	if (IsKeyPressed(KEY_P)) screen = (gameplay == screen) ? paused : gameplay;
}

int main(void)
{
	InitWindow(SWIDE, STALL, STITLE);
	SetTargetFPS(60);

	me.x=(int)SWIDE/2; me.y=(int)STALL/2;
	move_food();

	while ( ! WindowShouldClose())
	{
		update();
		BeginDrawing();
		draw();
		EndDrawing();
		inputs();
	}

	CloseWindow();
	return 0;
}