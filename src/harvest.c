#include "harvest.h"
#include "music.h"

void collect(hole *last_hole, int number_player, char *player1_name, char *player2_name, SDL_Renderer *renderer, TTF_Font *font, FILE **score_file, hole **game_board, int *number_of_turns_without_recolt, int *nb_Awale_collected_by_player1, int *nb_Awale_collected_by_player2)
{
    /*
        Si dans le dernier dans lequel on a semé, il y a 2 ou 3 awale et que ce trou est dans le camp adverse c'est à dire
        si c'est le joueur1 qui a joué, on récolte dans les trous dont les numéros sont compris 1 et 6 avec (number_player%2)*6+1=6+1=7 et (number_player%2)*6+6)=6+6=12.
        Donc chez le joueur 2.
        Si c'est le joueur2 qui a joué, on récolte dans les trous dont les numéros sont compris 1 et 6 avec (number_player%2)*6+1=0+1=1 et (number_player%2)*6+6)=0+6=6.
        Donc chez le joueur 1
    */

    if ((last_hole->number_of_Awale_in_hole == 2 || last_hole->number_of_Awale_in_hole == 3) && (last_hole->number_of_hole >= ((number_player % 2) * 6 + 1) && last_hole->number_of_hole <= ((number_player % 2) * 6 + 6)) && last_hole != NULL)
    {
        //Pour nous laisser un peu plus de temps pour voir la récolte
        SDL_Delay(400);

        // Le joueur peut récolter donc on ajoute le nombre d'awalé collecté par lui dans le trou dans lequel il fait la récolte
        if (number_player == 1)
        {
            *nb_Awale_collected_by_player1 += last_hole->number_of_Awale_in_hole;
            game_board[0][6].number_of_Awale_in_hole=*nb_Awale_collected_by_player1;
            draw_awale_collected(renderer, number_player, last_hole->number_of_Awale_in_hole);
        }
        else
        {
            *nb_Awale_collected_by_player2 += last_hole->number_of_Awale_in_hole;
            game_board[1][6].number_of_Awale_in_hole=*nb_Awale_collected_by_player2;
            draw_awale_collected(renderer, number_player, last_hole->number_of_Awale_in_hole);
        }

        char number_Awale_collected_by_player1[2];
        char number_Awale_collected_by_player2[2];

        char score_display_player1[255];
        char score_display_player2[255];

        // On écrit le tableau des scores dans un fichier que intitulé score.txt
        write_in_scorefile(player1_name, player2_name, nb_Awale_collected_by_player1, nb_Awale_collected_by_player2, number_Awale_collected_by_player1, number_Awale_collected_by_player2, score_display_player1, score_display_player2, score_file);

        // On récolte les awalés
        take_Awale(last_hole);

        // On dessine un trou pour montrer que les awalés ont été récoltés
        draw_a_hole(renderer, last_hole);
        //play_music("music/song_sowing_and_collect.mp3");

        // On va dessiner un rectangle vert qui va effacer l'ancien score
        SDL_SetRenderDrawColor(renderer, 0x9a, 0xcd, 0x32, 0xff);
        SDL_Rect rect;
        rect.h = 100;
        rect.w = 100;
        rect.x = 600;           

        rect.y = (number_player - 1) * 640;
        SDL_RenderFillRect(renderer, &rect);

        // Puis selon que ce soit le joueur 1 ou 2 qui récolte on affiche le nouveau score

        affich_score(renderer, font, *nb_Awale_collected_by_player1, *nb_Awale_collected_by_player2);

        // On remet le nombre de tours sans récolte à 0 car il y eu une récolte
        *number_of_turns_without_recolt = 0;

        play_music("music/song_sowing_and_collect.mp3");

        SDL_RenderPresent(renderer);
    }
    else
    {
        *number_of_turns_without_recolt += 1;
    }
}
