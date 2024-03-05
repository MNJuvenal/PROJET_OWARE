#ifndef SOW_H
#define SOW_H

#include <string.h>

#include "colors.h"
#include "harvest.h"
#include "SDLTools.h"
#include "struct.h"

void sow_and_collect_Awale(hole *chosen_hole, int number_player, char *player1_name, char *player2_name, SDL_Point click_coordonates, hole **game_board, SDL_Renderer *renderer, TTF_Font *font, FILE **score_file, int *number_of_turns_without_recolt, int *nb_Awale_collected_by_player1, int *nb_Awale_collected_by_player2);
void force_sowing(SDL_bool *player_cannot_play, hole **game_board, int *number_player, char *player1_name, char *player2_name, SDL_Renderer *renderer, TTF_Font *font, FILE **score_file, int *number_of_turns_without_recolt, int *nb_Awale_collected_by_player1, int *nb_Awale_collected_by_player2);

#endif