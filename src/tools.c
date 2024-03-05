#include "tools.h"

hole **init_game_board()
{
    // Notre plateau correspond à une matrice qui a 2 lignes et 6 colonnes

    // On recupère l'adresse d'un tableau qui va contenir les adresses de 2 tableaux d'adresses de structures trou
    hole **game_board = (hole **)malloc(2 * sizeof(hole *));

    // Les 2 tableaux de structure trou vont contenir 6 trous,Ce qui fait 12 trous au total
    game_board[0] = (hole *)malloc(7 * sizeof(hole));
    game_board[1] = (hole *)malloc(7 * sizeof(hole));

    // Enfin numérote les 12 trous et on rempli chaque trou de 3 awalées
    number_holes_and_fill_holes(game_board);

    // On retourne l'adresse de la matrice
    return game_board;
}

int distance(int x1, int y1, int x2, int y2)
{
    /*
        En Mathématiques la distance entre deux points A(x1,y1) et B(x2,y2) dans un plan est égale à dist(A,B)=√((x2−x1)^2+(y2−y1)^2)
    */

    int a, b, distance;
    a = (x2 - x1) * (x2 - x1);
    b = (y2 - y1) * (y2 - y1);

    /*
        Mais ici on va retourner le carré de la distance
    */

    distance = a + b;

    return distance;
}

void choose_playername_and_playmode(char *player1_name, char *player2_name,char *robot_level, char *human_or_robot_player)
{
    int nb_times_bad_choice_number_level=0;

    printf("\nPlayer1, saisissez votre nom de joueur : ");
    scanf(" %s", player1_name);

    printf("\nVoulez vous jouer contre un humain ou un robot ? [h = HUMAIN | Toute autre réponse = ROBOT ] : ");
    scanf(" %s", human_or_robot_player);

    if (strcmp(human_or_robot_player, "h") == 0)
    {
        printf("\nPlayer2, saisissez votre nom de joueur : ");
        scanf(" %s", player2_name);
    }
    else
    {
        sprintf(player2_name, "%s", "robot");
        do
        {
            /*Si on itère encore une fois sela veut dire que l'on a choisi un niveau de difficulté non valide 
              donc nb_times_bad_choice_number_level sera nécessairement différent de 0 à chaque fois que l'on saisira un niveau non valide*/

            if(nb_times_bad_choice_number_level != 0)
            {
                printf("\nChoisissez un niveau de difficulté valide\n");
            }

            printf("\nChoisisez le numéro du niveau de difficulté du robot:\n");
            printf("1- le robot joue de façon aléatoire\n");
            printf("2- niveau facile\n");
            printf("3- niveau moyen\n");
            printf("4- niveau difficile\n");
            scanf("%s",robot_level);

            nb_times_bad_choice_number_level++;

        } while (!strcmp(robot_level,"1")==0 && !strcmp(robot_level,"2")==0 && !strcmp(robot_level,"3")==0 && !strcmp(robot_level,"4")==0);
    }  
}

int choose_randomly_player()
{            
    int number_player = 1 + rand() % 2;
    return number_player;
}

void write_in_scorefile(char *player1_name, char *player2_name, int *nb_Awale_collected_by_player1, int *nb_Awale_collected_by_player2, char *number_Awale_collected_by_player1, char *number_Awale_collected_by_player2, char *score_display_player1, char *score_display_player2, FILE **score_file)
{
    // On va mettre dans des chaînes de caractères les valeurs des nombres d'awalés collectés par chaque joueur
    sprintf(number_Awale_collected_by_player1, "%d", *nb_Awale_collected_by_player1);
    sprintf(number_Awale_collected_by_player2, "%d", *nb_Awale_collected_by_player2);

    char number_Awale_collected_by_player1_copy[2];
    char number_Awale_collected_by_player2_copy[2];

    strcpy(number_Awale_collected_by_player1_copy, number_Awale_collected_by_player1);
    strcpy(number_Awale_collected_by_player2_copy, number_Awale_collected_by_player2);

    char player1_name_copy[25];
    char player2_name_copy[25];

    strcpy(player1_name_copy, player1_name);
    strcpy(player2_name_copy, player2_name);

    // On met dans les chaînes de caractères qui vont contenir les pseudos des joueurs suivis de leur score respectif les valeurs qu'elles doivent contenir
    sprintf(score_display_player1, "%s", strcat(strcat(player1_name_copy, ": "), number_Awale_collected_by_player1));
    sprintf(score_display_player2, "%s", strcat(strcat(player2_name_copy, ": "), number_Awale_collected_by_player2));

    *score_file = fopen("score.txt", "w");
    fputs(score_display_player1, *score_file);
    fputs("\n", *score_file);
    fputs(score_display_player2, *score_file);

    fclose(*score_file);
}

void number_holes_and_fill_holes(hole **game_board)
{
    int j;

    for (j = 0; j < 6; j++)
    {
        // Nous avons une matrice de 2 lignes et de 6colonnes dont on va numéroter les cases  de 1 à 12 en allant d'en haut à droite vers en bas à gauche

        // Pour le premier tableau,on numérote les cases de 1 à 6 de la droite vers la gauche
        game_board[0][5 - j].number_of_hole = j + 1;

        // On met trois awalé dans le trou
        game_board[0][j].number_of_Awale_in_hole = 3;

        // Pour le deuxième tableau,on numérote les cases de 7 à 12 de la gauche vers la droite
        game_board[1][j].number_of_hole = 7 + j;
        game_board[1][j].number_of_Awale_in_hole = 3;
    }

    // On donne au trou qui vont contenir les awalés collectés les nombres 13 et 14 pour les joueurs 1 et 2 respectivement
    game_board[0][6].number_of_hole = 13;
    game_board[1][6].number_of_hole = 14;

    game_board[0][6].number_of_Awale_in_hole = 0;
    game_board[1][6].number_of_Awale_in_hole = 0;
}

void take_Awale(hole *adress_chosen_hole)
{
    // Pour le trou choisit pour la semaille, on met le nombre d'awalé à 0
    adress_chosen_hole->number_of_Awale_in_hole = 0;
}

void free_game_board(hole** game_board)
{
    free(game_board[0]);
    free(game_board[1]);
    free(game_board);
}
