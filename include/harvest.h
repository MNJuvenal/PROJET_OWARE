#ifndef HARVEST_H
#define HARVEST_H

#include "struct.h"
#include "colors.h"
#include "SDLTools.h"

void collect(hole *last_hole_sowed, int number_player, char *player1_name, char *player2_name, SDL_Renderer *renderer, TTF_Font *font, FILE **score_file, hole **game_board, int *number_of_turns_without_recolt, int *nb_Awale_collected_by_player1, int *nb_Awale_collected_by_player2);

#endif