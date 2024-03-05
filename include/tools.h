#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struct.h"

/**
* Outils utilisés et qui ne sont pas liés à sdl
*/

hole **init_game_board();
void take_Awale(hole *adress_chosen_hole);
int distance(int x1, int y1, int x2, int y2);
void number_holes_and_fill_holes(hole **game_board);
void choose_playername_and_playmode(char *player1_name, char *player2_name,char* robot_level, char *human_or_robot_player);
int choose_randomly_player();
void write_in_scorefile(char *player1_name, char *player2_name, int *nb_Awale_collected_by_player1, int *nb_Awale_collected_by_player2, char *number_Awale_collected_by_player1, char *number_Awale_collected_by_player2, char *score_display_player1, char *score_display_player2, FILE **score_file);
void free_game_board(hole** game_board);

#endif