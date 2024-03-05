#include "change_turn.h"

void create_new_game_board(hole *** new_game_board, hole **game_board)
{
    //On modifie le nouveau plateau pour qu'il ait les mêmes caractéristiques que le précécdent

    (*new_game_board) = (hole **)malloc(2 * sizeof(hole *));

    (*new_game_board)[0] = (hole *)malloc(7 * sizeof(hole));
    (*new_game_board)[1] = (hole *)malloc(7 * sizeof(hole));

    int j;

    for (j = 0; j < 6; j++)
    {
        (*new_game_board)[0][5 - j].number_of_hole = j + 1;

        (*new_game_board)[0][j].number_of_Awale_in_hole = game_board[0][j].number_of_Awale_in_hole;

        (*new_game_board)[1][j].number_of_hole = 7 + j;

        (*new_game_board)[1][j].number_of_Awale_in_hole = game_board[1][j].number_of_Awale_in_hole;
    }

    (*new_game_board)[0][6].number_of_hole = 13;
    (*new_game_board)[1][6].number_of_hole = 14;

    (*new_game_board)[0][6].number_of_Awale_in_hole = game_board[0][6].number_of_Awale_in_hole;
    (*new_game_board)[1][6].number_of_Awale_in_hole = game_board[1][6].number_of_Awale_in_hole;
}



void save_turn(hole** game_board,int number_current_player, int number_of_turns_without_recolt, turn_sequence **all_turn)
{
    //Un alloue un nouveau tour qui aura les caractéristiques du plateau avec ses modifications du au coup du joueur courant

    (*all_turn)->next_turn = (turn_sequence *) malloc (1 * sizeof(turn_sequence));
 
    hole** new_game_board;

    create_new_game_board(&new_game_board, game_board);
    
	(*all_turn)->next_turn->game_board = new_game_board;
    (*all_turn)->next_turn->number_current_player = number_current_player;
    (*all_turn)->next_turn->number_of_turns_without_recolt = number_of_turns_without_recolt;
	(*all_turn)->next_turn->previous_turn = *all_turn;
    (*all_turn)->next_turn->next_turn = NULL;

    //On gardera l'adresse du dernier tour
    (*all_turn) = (*all_turn)->next_turn;
}

void free_turns(turn_sequence **all_turn, turn_sequence **chosen_turn)
{
    //Du tour qui suit le tour choisi s'il y en a au moins un en libère les tours et leurs plateaux alloués
    if((*all_turn)->next_turn != NULL)
    {
        free_turns(&(*all_turn)->next_turn,chosen_turn);
        free_game_board((*all_turn)->next_turn->game_board);
        free((*all_turn)->next_turn);
    }

    //En ce qui concerne le tour choisi, l'adresse du qui est le précède est maintenant à NULL
    if(all_turn == chosen_turn)
    {
        (*all_turn)->next_turn = NULL;
    }   
}

void free_all_turn(turn_sequence **all_turn)
{
    //On libère chaque tour et leur plateau alloué
    if((*all_turn)->game_board != NULL)
    {
        free_all_turn(&(*all_turn)->previous_turn);
        free_game_board((*all_turn)->game_board);
        free(*all_turn);
    } 
}

void draw_all_awale_collected(hole**  game_board, SDL_Renderer* renderer)
{
    int i,j;
    for(i = 0; i < 2; i++)
    {
        for (j = 0; j < game_board[i][6].number_of_Awale_in_hole; j++)
        {
            draw_Awale(renderer, 20 + 10 + 23 + rand() % 97 + (1340 - 100) * i, 220 + 10 + 23 + 70 + rand() % 97, 23);
        }
    }
}

void draw_turn(turn_sequence *chosen_turn, SDL_Renderer* renderer, TTF_Font* font, char *player1_name, char *player2_name)
{
    //On dessine sur la fenêtre tout ce qu'il y a au tour en question
    draw_game_board(renderer);
    draw_all_Awale(renderer, chosen_turn->game_board);
    draw_all_awale_collected(chosen_turn->game_board, renderer);
    affich_number_Awale_in_hole(chosen_turn->game_board, renderer, font);
    affich_number_turn_without(renderer, font, &chosen_turn->number_of_turns_without_recolt);
    affich_score(renderer, font, (chosen_turn->game_board)[0][6].number_of_Awale_in_hole,  (chosen_turn->game_board)[1][6].number_of_Awale_in_hole);

    //On met en évidence le joueur qui a la main                       
    if ( chosen_turn->number_current_player == 1)
    {
        affich_text(renderer, font, player2_name, 40, 660, black, 30);
        affich_text(renderer, font, player1_name, 40, 20, red, 30);
    }

    else if ( chosen_turn->number_current_player == 2)
    {
        affich_text(renderer, font, player1_name, 40, 20, black, 30);
        affich_text(renderer, font, player2_name, 40, 660, red, 30);
    }
}

void validate_turn(turn_sequence **all_turn, turn_sequence **chosen_turn, int *number_player, hole ***game_board, int *number_of_turns_without_recolt, int *nb_Awale_collected_by_player1, int *nb_Awale_collected_by_player2)
{
    //Si on valide un tour,on libère les tours qui suivent et les informations concernant le tour courrant sont mise à jour

    free_turns(chosen_turn, chosen_turn); 
    *number_of_turns_without_recolt = (*chosen_turn)->number_of_turns_without_recolt;
    *number_player = (*chosen_turn)->number_current_player; 
    *nb_Awale_collected_by_player1 = ((*chosen_turn)->game_board)[0][6].number_of_Awale_in_hole;
    *nb_Awale_collected_by_player2 = ((*chosen_turn)->game_board)[1][6].number_of_Awale_in_hole;
    hole **new_game_board;  
    create_new_game_board(&new_game_board, (*chosen_turn)->game_board);                    
    *game_board = new_game_board;  
    *all_turn = (*chosen_turn);
}

void get_click_coordonates(int *number_player, SDL_Point *click_coordinates, char *human_or_robot_player, SDL_Event event)
{
    //Si c'est le joueur1 ou le joueur2 humain qui a la main, on récupère les coordonnées de son click
    if (*number_player == 1 || ((strcmp(human_or_robot_player, "h") == 0) && *number_player == 2))
    {
        click_coordinates->x = event.button.x;
        click_coordinates->y = event.button.y;   
    }
}

void change_turn_and_get_click_coordonates(turn_sequence **all_turn, SDL_Renderer* renderer, TTF_Font *font, int *number_player, hole ***game_board, int *number_of_turns_without_recolt, char* human_or_robot_player, SDL_bool* quit, SDL_Point* click_coordinates, char* player1_name, char* player2_name, int *nb_Awale_collected_by_player1, int *nb_Awale_collected_by_player2)
{
    turn_sequence *chosen_turn = *all_turn;
    SDL_bool click_directional_button = SDL_FALSE, click_return = SDL_FALSE;

    // Boucle principale
    SDL_Event event;
    SDL_bool continuer = SDL_TRUE;
   
    while (continuer)
    {  
        //Si c'est le robot qui joue, il n'y a pas de coordonnées de click à prendre et on changera de tour après son coup on en a envie
        if(strcmp(human_or_robot_player,"h") != 0 && *number_player == 2)
        {
            continuer = SDL_FALSE;
            break;
        } 
        
        SDL_WaitEvent(&event);   

        // Gestion des événements
        switch (event.type)
        {
            case SDL_MOUSEBUTTONDOWN:
            {
                //Si il a changé de tour et cliqué sur retour ou s'il n'a pas changé de tour, on peut prendre les coordonnées du click du ou des joueurs humains
                if((click_directional_button && click_return) || (!click_directional_button && !click_return))
                {
                    get_click_coordonates(number_player, click_coordinates, human_or_robot_player, event);
                                     
                    continuer = SDL_FALSE;
                }
                
                break;
            }

            case SDL_QUIT:
            {
                *quit = SDL_TRUE;
                continuer = SDL_FALSE;
                break;
            }

            case SDL_KEYDOWN:
            {
                // Gestion des touches enfoncées*
                switch (event.key.keysym.sym)
                {
                    case SDLK_RETURN:
                    { 
                        validate_turn(all_turn, &chosen_turn, number_player, game_board, number_of_turns_without_recolt, nb_Awale_collected_by_player1, nb_Awale_collected_by_player2);

                        /*Sachant que les coordonnées que les coordonnées du click du joueur qui avait la main sont conservés.
                          Si on ne change pas les coordonnées en coordonnées de point ne se retrouvant pas dans les trous du joueur qui avait la main comme par exemple si on prend x = y = 0.
                          Si on retourne à un tour ou le ce joueur avait le main le coup qu'il avait sera rejoué automatiquement.
                         */ 
                        if((strcmp(human_or_robot_player,"h") == 0 && *number_player == 2) || *number_player == 1)
                        {
                            click_coordinates->x = 0;
                            click_coordinates->y = 0;   
                        }

			    	    continuer = SDL_FALSE;

                        click_return = SDL_TRUE;

                        break;
                    }
			            
                    //Si on clique à gauche, on part au tour précédent s'il y en a un    
                    case SDLK_LEFT:
                    {
                        if( chosen_turn->previous_turn->previous_turn != NULL )
                        {
                            chosen_turn = chosen_turn->previous_turn;

                            draw_turn(chosen_turn, renderer, font, player1_name, player2_name);

                            click_directional_button = SDL_TRUE;
                            
                            SDL_RenderPresent(renderer);
                        }
                        break;
                    }

                    //Si on clique à droite, on part au tour suivant s'il y en a un    
                    case SDLK_RIGHT:
                    {  
                        if(chosen_turn->next_turn != NULL)
                        {
                            chosen_turn = chosen_turn->next_turn;
                            
                            draw_turn(chosen_turn, renderer, font, player1_name, player2_name);

                            click_directional_button = SDL_TRUE;
                                                       
                            SDL_RenderPresent(renderer);
                        }
                        break;
                    }

                    //Si on clique en bas, on part au début du jeu    
                    case SDLK_DOWN:
                    {
                        if(chosen_turn->previous_turn->previous_turn == NULL)
                        {
                            //On ne fait rien si on est déja au tout début du jeu
                        }
                        else
                        {
                            while( chosen_turn->previous_turn->previous_turn != NULL)
                            {
                                chosen_turn = chosen_turn->previous_turn; 
                            }

                            draw_turn(chosen_turn, renderer, font, player1_name, player2_name);

                            click_directional_button = SDL_TRUE;
                            
                            SDL_RenderPresent(renderer);

                            break;
                        }
                        
                        break;
                    }

                    //Si on clique en haut, on part au dernier tour   
                    case SDLK_UP:
                    {
                        if(chosen_turn->next_turn == NULL)
                        {
                            //On ne fait rien si on est déja au dernier tour de la partie
                        }
                        else
                        {
                            while( chosen_turn->next_turn != NULL)
                            {
                                chosen_turn = chosen_turn->next_turn; 
                            }

                            draw_turn(chosen_turn, renderer, font, player1_name, player2_name);

                            click_directional_button = SDL_TRUE;
                            
                            SDL_RenderPresent(renderer);

                            break;
                        }

                        break;
                    }    
                }

                break;
            }   
        }
    }
}