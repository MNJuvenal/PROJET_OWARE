#include "sow.h"
#include "music.h"

void sow_and_collect_Awale(hole *chosen_hole, int number_player, char *player1_name, char *player2_name, SDL_Point click_coordonates, hole **game_board, SDL_Renderer *renderer, TTF_Font *font, FILE **score_file, int *number_of_turns_without_recolt, int *nb_Awale_collected_by_player1, int *nb_Awale_collected_by_player2)
{
    int i;

    /*
        Si le joueur n'a pas cliqué sur un trou mais plutôt sur une autre parti du plateau on ne fait rien, car l'adresse du trou sera invalide
        Dans le cas on contraire on execute la suite d'instruction suivante
    */

    if (chosen_hole != NULL)
    {
        // On recupère le nombre d'awalé dans le trou choisi
        int number_taken_Awale = chosen_hole->number_of_Awale_in_hole;

        // On récupère le numéro du trou choisi
        int number_chosen_hole = chosen_hole->number_of_hole;

        /*
            On initialise une autre variable au numéro du trou choisi
            Cette variable va prendre les valeurs des trous dans lesquels en sème au cours de la semaille
        */

        int number_current_hole = number_chosen_hole;

        /*
            On crée un pointer qui va prendre l'adresse du dernier dans lequel on va semer
            Cette adresse sera utile pour la récolte
        */

        hole *the_last_hole_sowed = NULL;

        // Dans le trou choisi on prend tous les awalés
        remove_Awale_in_hole(click_coordonates, game_board);

        // On dessine un trou où on a pris les awalé pour bien montrer que le trou a étévidé
        draw_a_hole(renderer, chosen_hole);

        /*
            Pour le nombre d'awalé qui se trouvait dans le trou choisi, on va faire la semaille
            Si le numéro du trou courrant était au 12.Le prochain trou dans lequel on va semer sera le trou numéro 1.
            D'où on réinitialise cette variable à 0.ensuite cette variable sera incrémengté et aura la valeur 1
        */
        for (i = 0; i < number_taken_Awale; i++)
        {

            if (number_current_hole == 12)
            {
                number_current_hole = 0;
            }

            // On incrémente la variable qui prend la valeur du trou courrant,du trou dans lequel on va semer pour semer à chaque fois dans les trous qui suivent
            number_current_hole += 1;

            /*
                Si on revient sur le trou choisi au départ on incrémente la variable qui prend le nombre d'awalé qui se trouvaient dans ce trou pour ne pas compter ce tour.
                Puis on saute cette itération
            */

            if (number_current_hole == number_chosen_hole)
            {
                number_taken_Awale++;
                continue;
            }

            if (number_current_hole >= 1 && number_current_hole <= 6)
            {
                /*
                    Sachant que pour les trous se trouvant dans cette zone,celle du joueur 1, les trous sont numérotés de droite vers gauche de 1 à 6.
                    Donc l'indice de ces trous dans leur tableau équivaut à 6-number_current_hole.
                    Par exemple pour le trou numéro 6 qui se trouve à la case d'indice 0, on a bien 6-6=0
                */

                // On incrémente le nombre d'awale du trou dans lequel on semé
                game_board[0][6 - number_current_hole].number_of_Awale_in_hole += 1;

                // On recupère à chaque fois l'adresse du trou dans equel on a semé.Et à la fin on aura l'adresse du dernier trou dans lequel on a semé
                if (i == (number_taken_Awale - 1))
                {
                    the_last_hole_sowed = &game_board[0][6 - number_current_hole];
                }

                // On dessine l'awalé semé
                draw_an_Awale(renderer, &game_board[0][6 - number_current_hole]);

                // On fait attendre 0.2s avant d'afficher l'awalé semé
                SDL_Delay(200);

                // Si on a semé le dernier awalé et que c'est le joueur 1 qui avait la main, on le pseudo du joueur 1 devient noir et celui du joueur 2 devient rouge pour dire que l'on donne la main
                if (i == (number_taken_Awale - 1) && number_player == 1)
                {
                    affich_text(renderer, font, player1_name, 40, 20, black, 30);
                    affich_text(renderer, font, player2_name, 40, 660, red, 30);
                }

                // Si on a semé le dernier awalé et que c'est le joueur 2 qui avait la main, on le pseudo du joueur 2 devient noir et celui du joueur 1 devient rouge pour dire que l'on donne la main
                if (i == (number_taken_Awale - 1) && number_player == 2)
                {
                    affich_text(renderer, font, player2_name, 40, 660, black, 30);
                    affich_text(renderer, font, player1_name, 40, 20, red, 30);
                }

                // On affiche le nombre d'awalé de chaque trou au dessus ou en dessous de ceux-ci selon que ce soit les trous du joueur 1 ou  2 respectivement
                affich_number_Awale_in_hole(game_board, renderer, font);

                play_music("music/song_sowing_and_collect.mp3");

                if (i == (number_taken_Awale - 1))
                {
                    // On met à jour le rendu pour afficher la dernière semaille
                    SDL_RenderPresent(renderer);

                    // On récolte puis dans collect on met à jour la collection
                    collect(the_last_hole_sowed, number_player, player1_name, player2_name, renderer, font, score_file, game_board, number_of_turns_without_recolt, nb_Awale_collected_by_player1, nb_Awale_collected_by_player2);

                    // On met un petit délai pour les SDL_RenderPresent de collect et affich_number_turn_without ne soit pas en concurrence
                    SDL_Delay(1);

                    affich_number_turn_without(renderer, font, number_of_turns_without_recolt);
                    affich_number_Awale_in_hole(game_board, renderer, font);

                    // On met un délai pour que la mise en jour de affich_number_turn_without ne soit pas en concurrence avec le SDL_RenderPresent de la fin
                    SDL_Delay(1);
                }

                // On met à jour le rendu
                SDL_RenderPresent(renderer);
            }

            if (number_current_hole >= 7 && number_current_hole <= 12)
            {
                /*
                    Sachant que pour les trous se trouvant dans cette zone,celle du joueur 2, les trous sont numérotés de droite vers gauche de 7 à 12.
                    Donc l'indice de ces trous dans leur tableau équivaut à number_current_hole-7.
                    Par exemple pour le trou numéro 7 qui se trouve à la case d'indice 0, on a bien 7-7=0
                */

                // On incrémente le nombre d'awale du trou dans lequel on semé
                game_board[1][number_current_hole - 7].number_of_Awale_in_hole += 1;

                // On recupère à chaque fois l'adresse du trou dans equel on a semé.Et à la fin on aura l'adresse du dernier trou dans lequel on a semé
                if (i == (number_taken_Awale - 1))
                {
                    the_last_hole_sowed = &game_board[1][number_current_hole - 7];
                }

                // On dessine l'awalé semé
                draw_an_Awale(renderer, &game_board[1][number_current_hole - 7]);

                 // On fait attendre 0.2s avant d'afficher l'awalé semé
                SDL_Delay(200);

                // Si on a semé le dernier awalé et que c'est le joueur 1 qui avait la main, on le pseudo du joueur 1 devient noir et celui du joueur 2 devient rouge pour dire que l'on donne la main
                if (i == (number_taken_Awale - 1) && number_player == 1)
                {
                    affich_text(renderer, font, player1_name, 40, 20, black, 30);
                    affich_text(renderer, font, player2_name, 40, 660, red, 30);
                }

                // Si on a semé le dernier awalé et que c'est le joueur 2 qui avait la main, on le pseudo du joueur 2 devient noir et celui du joueur 1 devient rouge pour dire que l'on donne la main
                if (i == (number_taken_Awale - 1) && number_player == 2)
                {
                    affich_text(renderer, font, player2_name, 40, 660, black, 30);
                    affich_text(renderer, font, player1_name, 40, 20, red, 30);
                }

                // On affiche le nombre d'awalé de chaque trou au dessus ou en dessous de ceux-ci selon que ce soit les trous du joueur 1 ou  2 respectivement
                affich_number_Awale_in_hole(game_board, renderer, font);

                play_music("music/song_sowing_and_collect.mp3");

                if (i == (number_taken_Awale - 1))
                {
                    // On met à jour le rendu pour afficher la dernière semaille
                    SDL_RenderPresent(renderer);

                    // On récolte puis dans collect on met à jour la collection
                    collect(the_last_hole_sowed, number_player, player1_name, player2_name, renderer, font, score_file, game_board, number_of_turns_without_recolt, nb_Awale_collected_by_player1, nb_Awale_collected_by_player2);

                    // On met un petit délai pour les SDL_RenderPresent de collect et affich_number_turn_without ne soit pas en concurrence
                    SDL_Delay(1);

                    affich_number_turn_without(renderer, font, number_of_turns_without_recolt);
                    affich_number_Awale_in_hole(game_board, renderer, font);

                    // On met un délai pour que la mise en jour de affich_number_turn_without ne soit pas en concurrence avec  le SDL_RenderPresent de la fin
                    SDL_Delay(1);
                }

                // On met à jour le rendu
                SDL_RenderPresent(renderer);
            }
        }
    }
}

void force_sowing(SDL_bool *player_cannot_play, hole **game_board, int *number_player, char *player1_name, char *player2_name, SDL_Renderer *renderer, TTF_Font *font, FILE **score_file, int *number_of_turns_without_recolt, int *nb_Awale_collected_by_player1, int *nb_Awale_collected_by_player2)
{
    /*
        Cette fonction sera utilisé quand un joueur aura toutes ses cases vides pour forcer la semaille du joueur adverse si possible.
        On prendra les awalé dans le trou qui est le plus proche du joueur n'ayant pas d'awalé c'est à dire le trou nécessitant le moins d'awalé pour pouvoir en donner à celui qui n'en a plus.
    */

    SDL_Point coordinates_of_hole;

    // Si on force la semaille chez le joueur 1
    if (*number_player == 1)
    {
        int i;

        // On va regarder dans les 6 trous
        for (i = 0; i < 6; i++)
        {
            // On va parcourir les coordonnées des centres de chaque trou du joueur 2 en allant de la droite vers la gauche
            coordinates_of_hole.x = 150 + 120 + i * 180;
            coordinates_of_hole.y = 200 + 70;

            /*
                Sachant que la semaille se fait dans le sens inverse des aiguilles d'une montre,en allant de la gauche vers la droite on va regarder si les trous du joueur 1 selon leur position,ont assez d'awalé pour en donner au joueur 2.
                Par exemple pour le trou numéro 6,il faut au moins 1 awalé pour en donner au joueur 2,on a 6+1=7.
                Ou encore pour le trou numéro 5,il faut au moins 2 awalé pour en donner au joueur 2,on a 5+2=7
            */

            if ((game_board[0][i].number_of_Awale_in_hole + game_board[0][i].number_of_hole) >= 7)
            {
                SDL_Delay(500);

                // Dès que l'on trouve un trou qui satisfait cette condition, on sème
                sow_and_collect_Awale(&game_board[0][i], *number_player, player1_name, player2_name, coordinates_of_hole, game_board, renderer, font, score_file, number_of_turns_without_recolt, nb_Awale_collected_by_player1, nb_Awale_collected_by_player2);

                // Si on a pu forcer la semaille donc le joueur 2 peut continuer à jouer.D'où player_cannot_play prend la valeur SDL_FALSE
                *player_cannot_play = SDL_FALSE;

                // On donne la main au joueur 2
                *number_player = 2;

                // On a trouvé un trou qui satisfait la condition condition, donc on arrête de chercher
                break;
            }
        }
    }
    else
    {
        int i;

        for (i = 0; i < 6; i++)
        {
            // On va parcourir les coordonnées des centres de chaque trou du joueur 2 en allant de la droite vers la gauche
            coordinates_of_hole.x = 150 + 120 + (5 - i) * 180;
            coordinates_of_hole.y = 400 + 70;

            /*
                Sachant que la semaille se fait dans le sens inverse des aiguilles d'une montre,en allant de la droite vers la gauche on va regarder si les trous du joueur 2 selon leur position,ont assez d'awalé pour en donner au joueur 1.
                Par exemple pour le trou numéro 12,il faut au moins 1 awalé pour en donner au joueur 2,on a 12+1=13.
                Ou encore pour le trou numéro 11,il faut au moins 2 awalé pour en donner au joueur 2,on a 11+2=13
            */

            if ((game_board[1][5 - i].number_of_Awale_in_hole + game_board[1][5 - i].number_of_hole) >= 13)
            {
                SDL_Delay(500);

                // Dès que l'on trouve un trou qui satisfait cette condition, on sème
                sow_and_collect_Awale(&game_board[1][5 - i], *number_player, player1_name, player2_name, coordinates_of_hole, game_board, renderer, font, score_file, number_of_turns_without_recolt, nb_Awale_collected_by_player1, nb_Awale_collected_by_player2);

                // Si on a pu forcer la semaille donc le joueur 2 peut continuer à jouer.D'où player_cannot_play prend la valeur SDL_FALSE
                *player_cannot_play = SDL_FALSE;

                // On donne la main au joueur 2
                *number_player = 1;

                break;
            }
        }
    }
}