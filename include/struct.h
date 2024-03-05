#ifndef STRUCT_H
#define STRUCT_H

typedef struct hole
{
    int number_of_hole;
    int number_of_Awale_in_hole;
} hole;

typedef struct turn_sequence
{
	hole** game_board;
    int number_of_turns_without_recolt;
    int number_current_player;
	struct turn_sequence* previous_turn;
	struct turn_sequence* next_turn;
}turn_sequence;

#endif