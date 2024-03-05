#ifndef CHANGE_TURN_H
#define CHANGE_TURN_H

#include "SDLTools.h"

void create_new_game_board(hole ***new_game_board, hole **game_board);
void save_turn(hole **game_board,int number_current_player, int number_of_turns_without_recolt, turn_sequence **all_turn);
void free_turns(turn_sequence **all_turn, turn_sequence **chosen_turn);
void free_all_turn(turn_sequence **all_turn);
void draw_all_awale_collected(hole  **game_board, SDL_Renderer* renderer);
void draw_turn(turn_sequence *chosen_turn, SDL_Renderer* renderer, TTF_Font* font, char *player1_name, char *player2_name);
void validate_turn(turn_sequence **all_turn, turn_sequence **chosen_turn, int *number_player, hole ***game_board, int *number_of_turns_without_recolt, int *nb_Awale_collected_by_player1, int *nb_Awale_collected_by_player2);
void get_click_coordonates(int *number_player, SDL_Point *click_coordinates, char *human_or_robot_player, SDL_Event event);
void change_turn_and_get_click_coordonates(turn_sequence **all_turn, SDL_Renderer* renderer, TTF_Font *font, int *number_player, hole ***game_board, int *number_of_turns_without_recolt, char* human_or_robot_player, SDL_bool* quit, SDL_Point* click_coordinates, char* player1_name, char* player2_name, int *nb_Awale_collected_by_player1, int *nb_Awale_collected_by_player2);

#endif
