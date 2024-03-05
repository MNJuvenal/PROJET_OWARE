#ifndef SDLTOOLS_H
#define SDLTOOLS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "struct.h"
#include "tools.h"
#include "colors.h"
#include "sow.h"
#include "harvest.h"


int init_graphic(SDL_Window **window, SDL_Renderer **renderer, TTF_Font **font);
void wait_quit();
void wait_clic();
void escape(SDL_Renderer *renderer, SDL_Window *window, TTF_Font *font);

void robot_playing_randomly(int* number_player,char* player1_name,char* player2_name,hole** game_board,SDL_Renderer* renderer,TTF_Font* font,FILE** score_file,int* number_of_turns_without_recolt,int* nb_Awale_collected_by_player1,int* nb_Awale_collected_by_player2);
int robot_easy_level(int* number_player,char* player1_name,char* player2_name,hole** game_board,SDL_Renderer* renderer,TTF_Font* font,FILE** score_file,int* number_of_turns_without_recolt,int* nb_Awale_collected_by_player1,int* nb_Awale_collected_by_player2);
int robot_medium_level(int* number_player,char* player1_name,char* player2_name,hole** game_board,SDL_Renderer* renderer,TTF_Font* font,FILE** score_file,int* number_of_turns_without_recolt,int* nb_Awale_collected_by_player1,int* nb_Awale_collected_by_player2);
int robot_hard_level(int* number_player,char* player1_name,char* player2_name,hole** game_board,SDL_Renderer* renderer,TTF_Font* font,FILE** score_file,int* number_of_turns_without_recolt,int* nb_Awale_collected_by_player1,int* nb_Awale_collected_by_player2);

void affich_image(SDL_Renderer* renderer, char* url_image);
void affich_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, SDL_Color text_color, int size_increase);
void affich_number_Awale_in_hole(hole **game_board, SDL_Renderer *renderer, TTF_Font *font);
void affich_number_turn_without(SDL_Renderer *renderer, TTF_Font *font, int *number_of_turns_without_recolt);
void affich_score(SDL_Renderer *renderer, TTF_Font *font, int nb_Awale_collected_by_player1, int nb_Awale_collected_by_player2);

void draw_game_board(SDL_Renderer *renderer);
void draw_blurry_circles(SDL_Renderer *renderer, int x_center, int y_center, int radius);
void draw_oval(SDL_Renderer *renderer, int x, int y, int radiusX, int radiusY);
void draw_blurry_ovals(SDL_Renderer *renderer, int x_center, int y_center, int radiusX, int radiusY);
void draw_a_hole(SDL_Renderer *renderer, hole *chosen_hole);
void draw_holes(SDL_Renderer *renderer);
void draw_Awale(SDL_Renderer *renderer, int x_center, int y_center, int radius);
void draw_an_Awale(SDL_Renderer *renderer, hole *chosen_hole);
void draw_all_Awale(SDL_Renderer *renderer, hole **game_board);
void draw_awale_collected(SDL_Renderer *renderer, int number_player, int nb_Awale_collected_by_player);
void SDL_RenderFillCircle(SDL_Renderer *renderer, int x_center, int y_center, int radius);
void score_board(SDL_Renderer *renderer, TTF_Font *font, char *player1_name, char *player2_name, int nb_Awale_collected_by_player1, int nb_Awale_collected_by_player2);

SDL_bool other_player_can_not_play(SDL_bool *player_cannot_play, hole **game_board, int number_player);
SDL_bool a_player_cannot_play_so_force_sowing_if_possible(SDL_bool* player_cannot_play,hole** game_board,int* number_player,char* player1_name,char* player2_name,SDL_Renderer* renderer,TTF_Font* font,FILE** score_file,int* number_of_turns_without_recolt,int* nb_Awale_collected_by_player1,int* nb_Awale_collected_by_player2);
SDL_bool game_over(float number_of_turns_without_recolt, int nb_Awale_collected_by_player1, int nb_Awale_collected_by_player2);
SDL_Point transform_index_to_coordonates_of_click(int index);

int remove_Awale_in_hole(SDL_Point click_coordonates, hole **game_board);
hole *return_adress_chosen_hole(SDL_Point *click_coordonates, hole **game_board);

#endif
