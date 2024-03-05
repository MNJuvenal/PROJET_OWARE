#include <time.h>

#include "struct.h"
#include "tools.h"
#include "sow.h"
#include "SDLTools.h"
#include "colors.h"
#include "harvest.h"
#include "change_turn.h"

int main()
{
    srand(time(NULL));

    int number_player = choose_randomly_player();
    int number_of_turns_without_recolt = 0;
    int nb_Awale_collected_by_player1 = 0;
    int nb_Awale_collected_by_player2 = 0;
    char player1_name[25], player2_name[25];
    char human_or_robot_player[2];
    char robot_level[1];
    char number_Awale_collected_by_player1[2];
    char number_Awale_collected_by_player2[2];
    char score_display_player1[50];
    char score_display_player2[50];

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *font = NULL;
    SDL_Point click_coordinates;
    SDL_bool player_cannot_play = SDL_FALSE;

    FILE *score_file = NULL;
    hole **game_board = init_game_board();
    turn_sequence *all_turn = (turn_sequence *) malloc (1*sizeof(turn_sequence));
    all_turn->game_board = NULL;
    all_turn->previous_turn = NULL;
    all_turn->next_turn = NULL;
    hole **new_game_board;
    create_new_game_board(&new_game_board, game_board);
    save_turn(new_game_board, number_player, number_of_turns_without_recolt, &all_turn);

    /*
        Le joueur 1 saisi son pseudo. Puis il choisit s'il veut jouer contre humain ou contre le robot.
        S'il veut jouer contre un humain, le joueur 2 saisira à son tour son pseudo
    */

    choose_playername_and_playmode(player1_name, player2_name, robot_level, human_or_robot_player);

    // On alloue la fenêtre, son rendu et la police
    init_graphic(&window, &renderer, &font);

    //Après avoir choisi les pseudos et le mode de jeu, on affiche une image en plein écran avec le message qui va suivre
    affich_image(renderer, "image/image_awale.webp");
    affich_text(renderer, font, "Awale", 600, 200, white, 50);
    affich_text(renderer, font, "Click anywhere to start", 300, 500, white, 30);
    SDL_RenderPresent(renderer);
   

    wait_clic();

    // On dessine le plateau, tous les awalés et le nombre d'awalé dans chaque trou
    draw_game_board(renderer);
    draw_all_Awale(renderer, game_board);
    affich_number_Awale_in_hole(game_board, renderer, font);


    // On écrit le tableau des score dans le fichier score.txt
    write_in_scorefile(player1_name, player2_name, &nb_Awale_collected_by_player1, &nb_Awale_collected_by_player2, number_Awale_collected_by_player1, number_Awale_collected_by_player2, score_display_player1, score_display_player2, &score_file);

    // On affiche les noms des joueurs et leurs scores, qui est bien sûr au début 0 pour chacun
    affich_text(renderer, font, player1_name, 40, 20, black, 30);
    affich_text(renderer, font, player2_name, 40, 660, black, 30);

    affich_score(renderer, font, nb_Awale_collected_by_player1, nb_Awale_collected_by_player2);

    SDL_bool quit = SDL_FALSE;

    // On affiche en rouge le numéro du joueur qui commence la partie
    if (number_player == 1)
    {
        affich_text(renderer, font, player1_name, 40, 20, red, 30);
    }
    else
    {
        affich_text(renderer, font, player2_name, 40, 660, red, 30);
    }

    SDL_RenderPresent(renderer);

    /*
        On continue de jouer tant que l'un des joueurs n'a pas récolté plus de la moitié des awalés, que les joueurs n'ont pas fait 20 coups sont récolte,que l'un des joueurs n'a plus d'awalé pour jouer ou que l'un des joueurs veut fermer la fenêtre
    */

    do
    {       
        if(a_player_cannot_play_so_force_sowing_if_possible(&player_cannot_play,game_board,&number_player,player1_name,player2_name,renderer,font,&score_file,&number_of_turns_without_recolt,&nb_Awale_collected_by_player1,&nb_Awale_collected_by_player2))
        {
            save_turn(game_board, number_player, number_of_turns_without_recolt, &all_turn);

            //L'un d'un joueurs ne peut pas jouer
            /*Soit on a pu forcer la semaille, alors on doit sauter l'itération,
              soit on n'a pas pu le faire, donc on saute l'itération et la partie s'achève*/
            continue;
        }

        /*
            Si on ne force pas la semaille, on récupère les coordonnées du clic du joueur en cours et on regarde aussi si l'un des joueurs veut fermer la fenêtre.
            On le fait pour le joueur 1 et le joueur 2 s'il est humain

            Et on change de tour si l'un des joueurs le décide
        */

        change_turn_and_get_click_coordonates(&all_turn, renderer, font, &number_player, &game_board, &number_of_turns_without_recolt, human_or_robot_player, &quit, &click_coordinates, player1_name, player2_name, &nb_Awale_collected_by_player1, &nb_Awale_collected_by_player2);
        
        // On recupère l'adresse du trou choisi.Si le joueur n'a pas cliqué sur un trou, chosen_hole garde la valeur NULL et on refait une itération

        hole *chosen_hole = return_adress_chosen_hole(&click_coordinates, game_board);

        if ((chosen_hole != NULL && chosen_hole->number_of_Awale_in_hole != 0) || (strcmp(human_or_robot_player, "h") != 0 && (number_player == 2)))
        {
            // Si c'est le joueur 1 qui a cliqué, on ne permettra qu'il choisisse que l'un de ses trous, ceux dont les numéros sont compris entre 1 et 6
            if (number_player == 1)
            {
                if (chosen_hole->number_of_hole >= 1 && chosen_hole->number_of_hole <= 6)
                {
                    sow_and_collect_Awale(chosen_hole, number_player, player1_name, player2_name, click_coordinates, game_board, renderer, font, &score_file, &number_of_turns_without_recolt, &nb_Awale_collected_by_player1, &nb_Awale_collected_by_player2);

                    // Puis il donne la main au joueur 2
                    number_player = 2;
                }
            }

            else if (strcmp(human_or_robot_player, "h") == 0 && number_player == 2)
            {
                // Sinon si c'est le joueur 2 est humain et a cliqué, on ne permettra qu'il choisisse que l'un de ses trous, ceux dont les numéros sont compris entre 7 et 12

                if (chosen_hole->number_of_hole >= 7 && chosen_hole->number_of_hole <= 12)
                {
                    sow_and_collect_Awale(chosen_hole, number_player, player1_name, player2_name, click_coordinates, game_board, renderer, font, &score_file, &number_of_turns_without_recolt, &nb_Awale_collected_by_player1, &nb_Awale_collected_by_player2);

                    // Puis il donne la main au joueur 1
                    number_player = 1;
                }
            }

            // Sinon si c'est le robot qui joue
            else if(strcmp(human_or_robot_player, "h") != 0 && number_player == 2)
            {
                //On attend 0.4s avant que le robot joue
                SDL_Delay(400);

                //On vérifie le niveau de difficulté du robot
                if(strcmp(robot_level,"1")==0)
                {
                    robot_playing_randomly(&number_player,player1_name,player2_name,game_board,renderer,font,&score_file,&number_of_turns_without_recolt,&nb_Awale_collected_by_player1,&nb_Awale_collected_by_player2);
                }
                else if(strcmp(robot_level,"2")==0)
                {
                    robot_easy_level(&number_player,player1_name,player2_name,game_board,renderer,font,&score_file,&number_of_turns_without_recolt,&nb_Awale_collected_by_player1,&nb_Awale_collected_by_player2);          
                }
                else if(strcmp(robot_level,"3")==0)
                {
                    robot_medium_level(&number_player,player1_name,player2_name,game_board,renderer,font,&score_file,&number_of_turns_without_recolt,&nb_Awale_collected_by_player1,&nb_Awale_collected_by_player2);  
                }
                else if(strcmp(robot_level,"4")==0)
                {
                    robot_hard_level(&number_player,player1_name,player2_name,game_board,renderer,font,&score_file,&number_of_turns_without_recolt,&nb_Awale_collected_by_player1,&nb_Awale_collected_by_player2);  
                }
            }  

            /*Si un joueur humain n'a pas cliqué sur un trou ou que le robot n'a pas joué,
              c'est à dire quand le game_board n'a pas été modifié on ne sauvegarde pas le tour*/
            if(chosen_hole != NULL ||(strcmp(human_or_robot_player, "h") != 0 && number_player == 2))
            {
              save_turn(game_board, number_player, number_of_turns_without_recolt, &all_turn);
            }
        }
    }
    while (!game_over(number_of_turns_without_recolt, nb_Awale_collected_by_player1, nb_Awale_collected_by_player2) && !quit && !player_cannot_play);


    // On affiche les scores et le résultat de la parti
    score_board(renderer, font, player1_name, player2_name, nb_Awale_collected_by_player1, nb_Awale_collected_by_player2);

    // On attend que les joueurs voient le tableau des scores
    wait_quit();

    //On libère tous les tours
    free_all_turn(&all_turn);

    //On libère le plateau alloué
    free_game_board(game_board);

    // On libère tout ce qui a été alloué pour le jeu
    escape(renderer, window, font);

    return 0;
}
