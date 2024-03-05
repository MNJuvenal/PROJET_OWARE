#include "SDLTools.h"
#include "music.h"

int init_graphic(SDL_Window **window, SDL_Renderer **renderer, TTF_Font **font)
{
        /*Au tout début on charge la SDL,on alloue les ressources à la bibliothèques SDL2/SDL.h,s'il y a une erreur on affiche l'erreur et on return EXIT_FAILURE qui vaut 1 */

        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            fprintf(stderr, "Erreur SDL_INIT_VIDEO : %s", SDL_GetError());
            return EXIT_FAILURE;
        }

        *window = SDL_CreateWindow("Awale", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1440, 740, SDL_WINDOW_SHOWN);

        if (*window == NULL)
        {
            fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
            SDL_Quit();
            return EXIT_FAILURE;
        }

        *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);

        if (*renderer == NULL)
        {
            fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
            SDL_DestroyWindow(*window);
            SDL_Quit();
            return EXIT_FAILURE;
        }

        // On alloue les ressources nécessaires à la bibliothèques SDL2/SDL_ttf.h pour pouvoir utiliser dans notre programme la police suivante
        TTF_Init();

        if (TTF_Init() == -1)
        {
            fprintf(stderr, "Erreur SDL_ttf : %s", TTF_GetError());
            SDL_DestroyWindow(*window);
            SDL_DestroyRenderer(*renderer);
            SDL_Quit();
            return EXIT_FAILURE;
        }

        // On récupère l'adresse la police ouverte
        *font = TTF_OpenFont("./04B_20__.TTF", 28);

        if (*font == NULL)
        {
            fprintf(stderr, "%s", TTF_GetError());
            SDL_DestroyWindow(*window);
            SDL_DestroyRenderer(*renderer);
            SDL_Quit();
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
}



void wait_quit()
{
    /**
     * Ici, on vérifie si le joueur a fermé la fenêtre
     */

    SDL_Event event;
    SDL_bool quit = SDL_FALSE;

    while (!quit)
    {
        SDL_WaitEvent(&event);

        if (event.type == SDL_QUIT)
        {
            quit = SDL_TRUE;
        }
    }
}

void wait_clic()
{
    /**
     * Ici, on va vérifier les clicks de l'utilisateur
     */

    SDL_Event event;
    SDL_bool click = SDL_FALSE;
    while (!click)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
            case SDL_MOUSEBUTTONDOWN:
            {
                click = SDL_TRUE;
                break;
            }

            case SDL_QUIT:
            {
                click = SDL_TRUE;
            }
        }
    }
}

void escape(SDL_Renderer *renderer, SDL_Window *window, TTF_Font *font)
{
    /*
        A la fin d'une partie, on libère la fenêtres et son rendu.
        On libère tout ce qui a été alloué pat la SDL.
        On ferme la police ouverte au tout début.
        Et on libère tout ce qui a été alloué par TTF
    */

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    TTF_CloseFont(font);
    TTF_Quit();
}

void robot_playing_randomly(int* number_player,char* player1_name,char* player2_name,hole** game_board,SDL_Renderer* renderer,TTF_Font* font,FILE** score_file,int* number_of_turns_without_recolt,int* nb_Awale_collected_by_player1,int* nb_Awale_collected_by_player2)
{
   //On choisi aléatoirement l'index du trou dans lequel le robot va prélever des awalés
    int index_of_random_hole=rand()%6;

    //A partir de cet index, on retrouve la coordonné du trou choisi dans le plateau
    SDL_Point click_coordinates_of_robot=transform_index_to_coordonates_of_click(index_of_random_hole);

    //On récupère l'adresse du trou choisi aléatoirement
    hole* random_hole=&game_board[1][index_of_random_hole];

    //Tant que le trou choisi ne contient aucun awalé,on réitère l'opération
    while(random_hole->number_of_Awale_in_hole==0)
    {
        index_of_random_hole=rand()%6;
        click_coordinates_of_robot=transform_index_to_coordonates_of_click(index_of_random_hole);
        random_hole=&game_board[1][index_of_random_hole];
    }

    //Le robot sème
    sow_and_collect_Awale(random_hole,*number_player,player1_name,player2_name,click_coordinates_of_robot,game_board,renderer,font,score_file,number_of_turns_without_recolt,nb_Awale_collected_by_player1,nb_Awale_collected_by_player2);

    //Puis il passe la main au joueur 1
    *number_player=1; 
}

int robot_easy_level(int* number_player,char* player1_name,char* player2_name,hole** game_board,SDL_Renderer* renderer,TTF_Font* font,FILE** score_file,int* number_of_turns_without_recolt,int* nb_Awale_collected_by_player1,int* nb_Awale_collected_by_player2)
{
    //Le robot vérifie s'il peut récolter dans le camp adverse
    int i,number_player1_hole;
    for(i=5;i>=0;i--)
    {
        number_player1_hole=(game_board[1][i].number_of_hole + game_board[1][i].number_of_Awale_in_hole)%12;
        if((number_player1_hole>=1 && number_player1_hole<=6) && (game_board[0][6-number_player1_hole].number_of_Awale_in_hole==1 || game_board[0][6-number_player1_hole].number_of_Awale_in_hole==2))
        {
            hole* chosen_hole=&game_board[1][i];
            SDL_Point click_coordinates_of_robot=transform_index_to_coordonates_of_click(i);
            sow_and_collect_Awale(chosen_hole,*number_player,player1_name,player2_name,click_coordinates_of_robot,game_board,renderer,font,score_file,number_of_turns_without_recolt,nb_Awale_collected_by_player1,nb_Awale_collected_by_player2);
            *number_player=1;
            return 0;
        }
    }

    //S'il ne peut pas récolter dans le camp adverse il joue de façon aléatoire
   robot_playing_randomly(number_player,player1_name,player2_name,game_board,renderer,font,score_file,number_of_turns_without_recolt,nb_Awale_collected_by_player1,nb_Awale_collected_by_player2);
   return 0;
}

int robot_medium_level(int* number_player,char* player1_name,char* player2_name,hole** game_board,SDL_Renderer* renderer,TTF_Font* font,FILE** score_file,int* number_of_turns_without_recolt,int* nb_Awale_collected_by_player1,int* nb_Awale_collected_by_player2)
{
    //Le robot vérifie si le joueur 1 peut récolter chez lui
    int i,number_player1_hole;
    for(i=0;i<=5;i++)
    {
        number_player1_hole=(game_board[0][i].number_of_hole + game_board[0][i].number_of_Awale_in_hole)%12;
        if((number_player1_hole>=7 && number_player1_hole<=12) && (game_board[1][number_player1_hole-7].number_of_Awale_in_hole==1 || game_board[1][number_player1_hole-7].number_of_Awale_in_hole==2))
        {
            hole* chosen_hole=&game_board[1][number_player1_hole-7];
            SDL_Point click_coordinates_of_robot=transform_index_to_coordonates_of_click(number_player1_hole-7);
            sow_and_collect_Awale(chosen_hole,*number_player,player1_name,player2_name,click_coordinates_of_robot,game_board,renderer,font,score_file,number_of_turns_without_recolt,nb_Awale_collected_by_player1,nb_Awale_collected_by_player2);
            *number_player=1;
            return 0;
        }
    }

    //Il vérifie si le joueur 1 peut récolter dans son camp
    for(i=5;i>=0;i--)
    {
        number_player1_hole=(game_board[1][i].number_of_hole + game_board[1][i].number_of_Awale_in_hole)%12;
        if((number_player1_hole>=1 && number_player1_hole<=6) && (game_board[0][6-number_player1_hole].number_of_Awale_in_hole==1 || game_board[0][6-number_player1_hole].number_of_Awale_in_hole==2))
        {
            hole* chosen_hole=&game_board[1][i];
            SDL_Point click_coordinates_of_robot=transform_index_to_coordonates_of_click(i);
            sow_and_collect_Awale(chosen_hole,*number_player,player1_name,player2_name,click_coordinates_of_robot,game_board,renderer,font,score_file,number_of_turns_without_recolt,nb_Awale_collected_by_player1,nb_Awale_collected_by_player2);
            *number_player=1;
            return 0;
        }
    }

    //Si personne ne peut récolter il joue de façon aléatoire
   robot_playing_randomly(number_player,player1_name,player2_name,game_board,renderer,font,score_file,number_of_turns_without_recolt,nb_Awale_collected_by_player1,nb_Awale_collected_by_player2);
   return 0;
}

int robot_hard_level(int* number_player,char* player1_name,char* player2_name,hole** game_board,SDL_Renderer* renderer,TTF_Font* font,FILE** score_file,int* number_of_turns_without_recolt,int* nb_Awale_collected_by_player1,int* nb_Awale_collected_by_player2)
{
    //Le robot vérifie si le joueur 1 peut récolter chez lui
    int i,number_player1_hole;
    for(i=0;i<=5;i++)
    {
        number_player1_hole=(game_board[0][i].number_of_hole + game_board[0][i].number_of_Awale_in_hole)%12;
        if((number_player1_hole>=7 && number_player1_hole<=12) && (game_board[1][number_player1_hole-7].number_of_Awale_in_hole==1 || game_board[1][number_player1_hole-7].number_of_Awale_in_hole==2))
        {
            hole* chosen_hole=&game_board[1][number_player1_hole-7];
            SDL_Point click_coordinates_of_robot=transform_index_to_coordonates_of_click(number_player1_hole-7);
            sow_and_collect_Awale(chosen_hole,*number_player,player1_name,player2_name,click_coordinates_of_robot,game_board,renderer,font,score_file,number_of_turns_without_recolt,nb_Awale_collected_by_player1,nb_Awale_collected_by_player2);
            *number_player=1;
            return 0;
        }
    }

    //Il vérifie si le joueur 1 peut récolter dans son camp
   for(i=5;i>=0;i--)
    {
        number_player1_hole=(game_board[1][i].number_of_hole + game_board[1][i].number_of_Awale_in_hole)%12;
        if((number_player1_hole>=1 && number_player1_hole<=6) && (game_board[0][6-number_player1_hole].number_of_Awale_in_hole==1 || game_board[0][6-number_player1_hole].number_of_Awale_in_hole==2))
        {
            hole* chosen_hole=&game_board[1][i];
            SDL_Point click_coordinates_of_robot=transform_index_to_coordonates_of_click(i);
            sow_and_collect_Awale(chosen_hole,*number_player,player1_name,player2_name,click_coordinates_of_robot,game_board,renderer,font,score_file,number_of_turns_without_recolt,nb_Awale_collected_by_player1,nb_Awale_collected_by_player2);
            *number_player=1;
            return 0;
        }
    }

    SDL_Point coordinates_of_hole;

    //Sinon le robot va semer en prenat les awalés les plus à droite
    for(i=5;i>=0;i--)
	{
        //On va parcourir les coordonnées des centres de chaque trou du joueur 2 en allant de la droite vers la gauche
        coordinates_of_hole.x=150+120 + i*180;
        coordinates_of_hole.y=400+70;

        /*Sachant que la semaille se fait dans le sens inverse des aiguilles d'une montre,en allant de la droite vers la gauche on va regarder si les trous du joueur 2 selon leur position,ont assez d'awalé pour en donner au joueur 1.
        Par exemple pour le trou numéro 12,il faut au moins 1 awalé pour en donner au joueur 2,on a 12+1=13.
        Ou encore pour le trou numéro 11,il faut au moins 2 awalé pour en donner au joueur 2,on a 11+2=13*/
        if(game_board[1][i].number_of_Awale_in_hole>0)
        {

            //Dès que l'on trouve un trou qui satisfait cette condition, on sème
	        sow_and_collect_Awale(&game_board[1][i],*number_player,player1_name,player2_name,coordinates_of_hole,game_board,renderer,font,score_file,number_of_turns_without_recolt,nb_Awale_collected_by_player1,nb_Awale_collected_by_player2);

            //On donne la main au joueur 2
            *number_player=1;

            return 0;
        }
	} 

    return 0;
}

void draw_game_board(SDL_Renderer *renderer)
{
    SDL_Rect rect;

    // On va dessiner tout en haut un rectangle qui prend 1/6 de la hauteur de la fenêtre et toute sa largeur
    rect.h = 100;
    rect.w = 1440;
    rect.x = 0;
    rect.y = 0;
    SDL_SetRenderDrawColor(renderer, 0x9a, 0xcd, 0x32, 0xff);
    SDL_RenderFillRect(renderer, &rect);

    // On modifie la coordonnée pour y dessiner le même cette fois ci tout en bas
    rect.y = 740 - 100;
    SDL_RenderFillRect(renderer, &rect);

    // Entre les deux rectangles dessinés, on va dessiner un rectangle qui prend les 4/6 de hauteur restante et qui commence biensûr à partir de la fin du rectangle qui est tout en haut

    rect.y = 100;
    rect.h = 740 - 100 * 2;
    SDL_SetRenderDrawColor(renderer, 0x4a, 0x2b, 0x00, 0xff);
    SDL_RenderFillRect(renderer, &rect);

    // On dessine les 12 trous du plateau
    draw_holes(renderer);
}

void draw_blurry_circles(SDL_Renderer *renderer, int x_center, int y_center, int radius)
{
    /*
        Ici, on souhaite dessiner des cercles concentriques (ayant le même centre) de plus en plus petit et de plus en plus clair pour donner un effet d'ombre et lumière
    */

    int k;
    for (k = radius; k >= 0; k--)
    {

        SDL_SetRenderDrawColor(renderer, radius - k, radius - k, radius - k, 0.2);
        SDL_RenderFillCircle(renderer, x_center, y_center, k);
    }
}

void draw_oval(SDL_Renderer *renderer, int x, int y, int radiusX, int radiusY)
{

    // On utilise la méthode de Bresenham pour faire cela.
    // Le mérite de l'application de cette méthode revient à ChatGPT ( version utilisant le moteur GPT 3.5 )

    for (int i = -radiusX; i <= radiusX; ++i)
    {
        int xCoord = x + i;
        int yTop = y - (int)(radiusY * sqrtf(1 - ((float)i * i) / (radiusX * radiusX)));
        int yBottom = y + (int)(radiusY * sqrtf(1 - ((float)i * i) / (radiusX * radiusX)));

        SDL_RenderDrawPoint(renderer, xCoord, yTop);
        SDL_RenderDrawPoint(renderer, xCoord, yBottom);
    }

    for (int i = -radiusY; i <= radiusY; ++i)
    {
        int yCoord = y + i;
        int xLeft = x - (int)(radiusX * sqrtf(1 - ((float)i * i) / (radiusY * radiusY)));
        int xRight = x + (int)(radiusX * sqrtf(1 - ((float)i * i) / (radiusY * radiusY)));

        SDL_RenderDrawPoint(renderer, xLeft, yCoord);
        SDL_RenderDrawPoint(renderer, xRight, yCoord);
    }
}

void draw_blurry_ovals(SDL_Renderer *renderer, int x_center, int y_center, int radiusX, int radiusY)
{
    /*
        Ici, on souhaite dessiner des formes ovales concentriques (ayant le même centre) de plus en plus petit et de plus en plus clair, pour donner un effet d'ombre et lumière
    */

    int k;
    for (k = radiusX; k >= 0; k--)
    {

        SDL_SetRenderDrawColor(renderer, radiusX - k, radiusX - k, radiusX - k, 0.2);
        draw_oval(renderer, x_center, y_center, k, radiusY + k - radiusX);
    }
}

void draw_holes(SDL_Renderer *renderer)
{
    int i = 0, j = 0, k = 0;

    while (k < 2)
    {
        // Nous allons dessiner des trous avec des effets d'ombre dans la zone de jeu du joueur 1
        draw_blurry_circles(renderer, 150 + 120 + i * 180, 200 + 70 + j, 85);

        i++;

        /*
            Quand les 6 trous du joueur 1 sont dessinés, on réinitialise i à 0 pour recommencer la même opération dans la zone de jeu du joueur 2
            On donne à j la valeur 200 pour dessiner dans la zone du joueur 2 qui est plus en bas.
            On incrémente k pour faire cette opération encore une fois.
        */

        if (i == 6)
        {
            i = 0;
            j = 200;
            k++;
        }
    }

    // On dessine les formes ovales qui vont contenir les awalés récoltés
    draw_blurry_ovals(renderer, 100, 300 + 70, 80, 150);
    draw_blurry_ovals(renderer, 1340, 300 + 70, 80, 150);
}

void draw_a_hole(SDL_Renderer *renderer, hole *chosen_hole)
{
    int number_chosen_hole = chosen_hole->number_of_hole;

    // On va dessiner un trou selon le numéro du trou choisi

    /*
        Si le numéro du trou est compris entre 1 et 6, on va dessiner un trou dans la zone de jeu du joueur 1.
        La place où on va dessiner va dépendre du numéro du trou choisi
        Si le numéro du trou est compris entre 7 et 12, on va dessiner un trou dans la zone de jeu du joueur 2.
        La place où on va dessiner va dépendre du numéro du trou choisi
    */

    if (number_chosen_hole >= 1 && number_chosen_hole <= 6)
    {
        draw_blurry_circles(renderer, 150 + 120 + (6 - number_chosen_hole) * 180, 200 + 70, 85);
    }

    if (number_chosen_hole >= 7 && number_chosen_hole <= 12)
    {
        draw_blurry_circles(renderer, 150 + 120 + (number_chosen_hole - 7) * 180, 200 + 200 + 70, 85);
    }
}

void draw_Awale(SDL_Renderer *renderer, int x_center, int y_center, int radius)
{
    // Cette fonction s'occupe principalement du design d'un Awale

    /*
        Comme dans la fonction draw_blurry_circles(), on va dessiner des cercles ayant le même centre de plus en plus petit allant d'un rouge sombre à un rouge de plus en plus clair pour donner un effet d'ombre et lumière.
    */

    int k = 1, l = radius;
    while (k <= 69)
    {

        SDL_SetRenderDrawColor(renderer, 70 + k * 3.64, 0, 0, 0.2);
        SDL_RenderFillCircle(renderer, x_center, y_center, l);
        k++;
        l -= radius / 70.0;
    }

    // Au centre, on dessine un awale d'un rouge plus sombre de notre choix
    SDL_SetRenderDrawColor(renderer, 0x7b, 0x00, 0x00, 0.2);
    SDL_RenderFillCircle(renderer, x_center, y_center, l);
}

void draw_an_Awale(SDL_Renderer *renderer, hole *chosen_hole)
{
    // Avec cette fonction, on va dessiner un Awale dans le bon trou selon le numéro du trou choisi
    int number_chosen_hole = chosen_hole->number_of_hole;

    /*
        Si le numéro du trou choisi se trouve entre 1 et 6, c'est donc un trou du joueur 1.
        On va dessiner dans une place de ce trou de façon aléatoire.
        Si le numéro du trou choisi se trouve entre 7 et 12, c'est donc un trou du joueur 2.
        On va dessiner dans une place de ce trou de façon aléatoire.
    */

    if (number_chosen_hole >= 1 && number_chosen_hole <= 6)
    {
        draw_Awale(renderer, 150 + 100 + (6 - number_chosen_hole) * 180 + rand() % 50, 180 + 70 + rand() % 50, 23);
    }

    if (number_chosen_hole >= 7 && number_chosen_hole <= 12)
    {
        draw_Awale(renderer, 150 + 100 + (number_chosen_hole - 7) * 180 + rand() % 50, 180 + 200 + 70 + rand() % 50, 23);
    }
}

void draw_all_Awale(SDL_Renderer *renderer, hole **game_board)
{
    int i = 0, j = 0, k = 0;
    int l, m;

    /*
        On va dessiner les 3 awale se trouvant dans les 6 trous du joueur 1 au début du jeu
        On le fait d'abord pour le joueur 1
        Quand les 6 trous du joueur 1 ont été remplis, on réinitialise i à 0 pour recommencer la même opération dans la zone de jeu du joueur 2.
        On donne à j la valeur 200 pour dessiner dans la zone joueur 2 qui est plus en bas.
        On incrémente k pour faire cette opération encore une fois
    */

    while (k < 2)
    {
        for (l = 0; l < 6; l++)
        {
            for (m = 0; m < game_board[k][l].number_of_Awale_in_hole; m++)
            {
                draw_Awale(renderer, 150 + 95 + i * 180 + rand() % 55, 175 + 70 + j + rand() % 55, 23);
            }

            i++;
        }

        if (i == 6)
        {
            j = 200;
            i = 0;
            k++;
        }
    }
}

int remove_Awale_in_hole(SDL_Point click_coordonates, hole **game_board)
{
    /*
        Si le click se fait dans l'un des 6 trous de 90 pixels de rayon du joueur 1,on prend les Awale dans le trou en question et on retourne 0. Les coordonnés prises qui ne sont pas celles du click sont celles des centres des trous que l'on a dessiné avec draw_holes()
        Sinon, si le click se fait dans l'un des 6 trous de 90 pixels de rayon du joueur 2,on prend les Awale dans le trou choisi et on retourne 0
    */

    int a = 0;

    while (a < 6)
    {
        if (distance(click_coordonates.x, click_coordonates.y, 150 + 120 + a * 180, 200 + 70) <= 85 * 85)
        {
            take_Awale(&game_board[0][a]);
            return 0;
        }

        if (distance(click_coordonates.x, click_coordonates.y, 150 + 120 + a * 180, 200 + 200 + 70) <= 85 * 85)
        {
            take_Awale(&game_board[1][a]);
            return 0;
        }
        a++;
    }
    return 0;
}

void draw_awale_collected(SDL_Renderer *renderer, int number_player, int nb_Awale_collected_by_player)
{
    /*
        On dessine de façon aléatoire dans une zone circulaire de l'ovale qui va contenir les awalés colléctés du joueur qui a la main
        Cette zone circulaire a le même centre que l'ovale en question
    */

    int i;

    for (i = 0; i < nb_Awale_collected_by_player; i++)
    {
        draw_Awale(renderer, 20 + 10 + 23 + rand() % 97 + (1340 - 100) * (number_player - 1), 220 + 10 + 23 + 70 + rand() % 97, 23);
    }
}

void affich_image(SDL_Renderer *renderer, char *url_image)
{
     //On initialise SDL_image tout en vérifiant s'il y a des erreurs
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        fprintf(stderr, "Erreur lors de l'initialisation de SDL_image : %s\n", IMG_GetError());
    }

    //On charge l'image et on vérifie s'il y a des erreurs l'hors du chargement
    SDL_Surface *surface_image = IMG_Load(url_image);
    if (surface_image == NULL)
    {
        fprintf(stderr, "Erreur lors du chargement de l'image : %s\n", IMG_GetError());
        IMG_Quit();
    }

    //On crée une texture à partir de la surface de l'image 
    SDL_Texture *texture_image = SDL_CreateTextureFromSurface(renderer, surface_image);
    if (texture_image == NULL)
    {
        fprintf(stderr, "Erreur lors de la création de la texture : %s\n", SDL_GetError());
        SDL_FreeSurface(surface_image);
        IMG_Quit();
    }

    //On copie entièrement la texture sur le rendu et on met l'image en plein écran
    SDL_RenderCopy(renderer, texture_image, NULL, NULL);

    //On met à jour le rendu
    SDL_RenderPresent(renderer);

    //On libère la surface de l'image
    SDL_FreeSurface(surface_image);

    //On libère la texture
    SDL_DestroyTexture(texture_image);
    
    //On libère tout ce qui a été alloué pour les images
    IMG_Quit();
}

void affich_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, SDL_Color text_color, int size_increase)
{
    /**
     * Ici, on crée une surface qui contient le pointer de la police du texte que l'on va afficher, le texte à afficher et sa couleur.
     * On transforme la surface en texture,que l'on va afficher dans le rendu.
     * On crée un rectangle dont on va copié les dimensions et la position pour la texture
     * On copie entièrement la texture sur le rendu et on lui donne les dimensions et la position du rectangle
     * On libère la surface et la texture après afficher la chaîne de caractères
     */

    SDL_Surface *surface_text = TTF_RenderText_Solid(font, text, text_color);
    SDL_Texture *texture_text = SDL_CreateTextureFromSurface(renderer, surface_text);
    SDL_Rect rect = {x, y, surface_text->w + size_increase, surface_text->h + size_increase};

    SDL_RenderCopy(renderer, texture_text, NULL, &rect);

    //On libère la surface et la texture
    SDL_FreeSurface(surface_text);
    SDL_DestroyTexture(texture_text);
}

void affich_number_Awale_in_hole(hole **game_board, SDL_Renderer *renderer, TTF_Font *font)
{
    int i = 0, j = 0;

    // Ici, on va dessiner un rectangle marron qui va effacer l'ancien nombre d'awalé dans chaque trou
    SDL_Rect rect;
    rect.h = 30;
    rect.w = 70;

    while (i < 2)
    {
        rect.x = 150 + 120 - 20 + j * 180;

        rect.y = 100 + 20 + 10 + i * 450;

        SDL_SetRenderDrawColor(renderer, 0x4a, 0x2b, 0, 0);
        SDL_RenderFillRect(renderer, &rect);

        // On affiche le nombre d'awalé se trouvant dans chaque trou
        char number_Awale_in_hole[20];
        sprintf(number_Awale_in_hole, "%d", game_board[i][j].number_of_Awale_in_hole);
        affich_text(renderer, font, number_Awale_in_hole, 150 + 120 - 20 + j * 180, 100 + 20 + 10 + i * 450, blue, 0);

        j++;

        /*
            Quand le nombre d'awalé dans les 6 trous du joueur 1 a été affiché, on réinitialise j à 0 pour recommencer la même opération dans la zone de jeu du joueur 2
            On incrémente i pour faire cette opération encore une fois
        */
        if (j == 6)
        {
            j = 0;
            i++;
        }
    }
}

void affich_number_turn_without(SDL_Renderer *renderer, TTF_Font *font, int *number_of_turns_without_recolt)
{
    // On va dessiner un rectangle vert qui va effacer l'ancien nombre de tours sans récolte.
    SDL_Rect rect;
    rect.h = 70;
    rect.w = 140;

    SDL_SetRenderDrawColor(renderer, 0x9a, 0xcd, 0x32, 0xff);

    rect.x = 1000;
    rect.y = 20;

    SDL_RenderFillRect(renderer, &rect);

    rect.y = 660;
    SDL_RenderFillRect(renderer, &rect);

    char nb_turns_without_recolt[2];

    sprintf(nb_turns_without_recolt, "%d", *number_of_turns_without_recolt);

    affich_text(renderer, font, nb_turns_without_recolt, 1000, 20, black, 30);
    affich_text(renderer, font, nb_turns_without_recolt, 1000, 660, black, 30);
}

void affich_score(SDL_Renderer *renderer, TTF_Font *font, int nb_Awale_collected_by_player1, int nb_Awale_collected_by_player2)
{
    char number_Awale_collected_by_player1[2];
    char number_Awale_collected_by_player2[2];

    sprintf(number_Awale_collected_by_player1, "%d", nb_Awale_collected_by_player1);
    sprintf(number_Awale_collected_by_player2, "%d", nb_Awale_collected_by_player2);

    affich_text(renderer, font, number_Awale_collected_by_player1, 600, 20, black, 30);  
    affich_text(renderer, font, number_Awale_collected_by_player2, 600, 660, black, 30);       
}

void SDL_RenderFillCircle(SDL_Renderer *renderer, int x_center, int y_center, int radius)
{
    // On va dessiner tous les points se trouvant se trouvant à équidistance du centre ou moins, c'est à dire les points se trouvant à la circonférence et à l'intérieur du cercle

    int i = -radius, j = -radius;
    for (i = -radius; i <= radius; i++)
    {
        for (j = -radius; j <= radius; j++)
        {
            if (distance(x_center + i, y_center + j, x_center, y_center) <= radius * radius)
            {
                SDL_RenderDrawPoint(renderer, x_center + i, y_center + j);
            }
        }
    }
}

hole *return_adress_chosen_hole(SDL_Point *click_coordonates, hole **game_board)
{
    int a = 0, b = 0, c = 0;
    hole *adress_chosen_hole = NULL;

    /*
        On veut obtenir l'adresse du trou choixi.
        Si le click se fait dans l'un des 6 trous de 90 pixels de rayon du joueur 1,on prend l'adresse du trou en question.
        Les coordonnés prises qui ne sont pas celles du click sont celles des centres des trous que l'on a dessiné avec draw_holes
        Si le click a été fait dans le trou en question on recupère l'adresse du trou
        Quand les 6 trous du joueur 1 ont été vérifié, on réinitialise a à 0 pour recommencer la même opération dans la zone de jeu du joueur 2
        On donne à b la valeur 200 pour faire les même vérifications dans la zone joueur 2 qui est plus en bas.
        On incrémente c pour faire cette opération encore une fois.
    */

    while (c < 2)
    {
        if (distance(click_coordonates->x, click_coordonates->y, 150 + 120 + a * 180, 200 + 70 + b) <= 85 * 85)
        {
            adress_chosen_hole = &game_board[c][a];
        }

        a++;
        if (a == 6)
        {
            b = 200;
            a = 0;
            c++;
        }
    }

    return adress_chosen_hole;
}

SDL_bool other_player_can_not_play(SDL_bool *player_cannot_play, hole **game_board, int number_player)
{
    int i;
    int number_of_empty_holes_of_other_player = 0;

    /*
        On va compter le nombre de trous vides chez l'adversaire
        Si c'est le joueur 1 qui joue number_player%2 équivaut à 1 ce qui correspond à l'indice du tableau du joueur 2
        Si c'est le joueur 2 qui joue number_player%2 équivaut à 0 ce qui correspond à l'indice du tableau de joueur 1

        Si un trou est vide en incrémente le nombre de trous vides de ce joueur
    */

    for (i = 0; i < 6; i++)
    {

        if (game_board[number_player % 2][i].number_of_Awale_in_hole == 0)
        {
            number_of_empty_holes_of_other_player++;
        }
    }

    /*
        Si le nombre de trous vides est 6 pour le joueur courant ou pour son adversaire ,l'un des joueur adverse n'a plus d'awale pour jouer.
        On recupère cette information via un pointer.
        On va aussi retourner cette information,comme c'est vrai il faut vérifier si le joueur qui a encore des awalé peut en donner à celui qui n'en a pas
        Dans tous les autres cas, les joueurs peuvent encore joué, on récupère cette information
    */

    if (number_of_empty_holes_of_other_player == 6)
    {
        *player_cannot_play = SDL_TRUE;
        return SDL_TRUE;
    }

    else
    {
        *player_cannot_play = SDL_FALSE;
        return SDL_FALSE;
    }
}

SDL_bool a_player_cannot_play_so_force_sowing_if_possible(SDL_bool* player_cannot_play,hole** game_board,int* number_player,char* player1_name,char* player2_name,SDL_Renderer* renderer,TTF_Font* font,FILE** score_file,int* number_of_turns_without_recolt,int* nb_Awale_collected_by_player1,int* nb_Awale_collected_by_player2)
{
        //Si à un moment de la partie, le joueur adverse n'a plus d'awalé,la variable player_cannot_play prend la valeur SDL_TRUE
        if(other_player_can_not_play(player_cannot_play,game_board,*number_player))
        {
            /*On force si possible la semaille du joueur qui en possède encore.Dans ce cas la fonction.Dans ce cas la variable player_cannot_play reprend la valeur SDL_FALSE
              Sinon elle reste à SDL_TRUE,ce qui mettra fin à la partie*/
            force_sowing(player_cannot_play,game_board,number_player,player1_name,player2_name,renderer,font,score_file,number_of_turns_without_recolt,nb_Awale_collected_by_player1,nb_Awale_collected_by_player2);
            
            //On retourne SDL_TRUE car l'un des joueurs ne peut pas jouer
            return SDL_TRUE;
        }

        //Si à un moment de la partie, le joueur courant n'a plus d'awalé, à cause du joueur adverse qui a récolté les derniers qu'il lui restait,la variable player_cannot_play prend aussi la valeur SDL_TRUE
        if(other_player_can_not_play(player_cannot_play,game_board,*number_player%2 + 1))
        {
            //On force alors la semaille chez le joueur adverse si possible
            //Si number_player était égale à 1, il prend la valeur 2,s'il était égale à 2 il prend la valeur 1
            *number_player=*number_player%2 + 1;
            force_sowing(player_cannot_play,game_board,number_player,player1_name,player2_name,renderer,font,score_file,number_of_turns_without_recolt,nb_Awale_collected_by_player1,nb_Awale_collected_by_player2);

            //On retourne SDL_TRUE car l'un des joueurs ne peut pas jouer
            return SDL_TRUE;
        }

        //On retourne SDL_FALSE si les deux joueurs peuvent jouer
        return SDL_FALSE;
}

SDL_bool game_over(float number_of_turns_without_recolt, int nb_Awale_collected_by_player1, int nb_Awale_collected_by_player2)
{
    // Si l'un des joueurs a collecté au moins 19 awalé donc au moins plus de la moitié de tous les awalé, le jeu est fini
    SDL_bool game_is_over = SDL_FALSE;
    if (nb_Awale_collected_by_player1 >= 19 || nb_Awale_collected_by_player2 >= 19)
    {
        game_is_over = SDL_TRUE;
    }

    // Si il y a eu 20 tours sans récolte alors le jeu est aussi fini
    if (number_of_turns_without_recolt == 20)
    {
        game_is_over = SDL_TRUE;
    }

    return game_is_over;
}

SDL_Point transform_index_to_coordonates_of_click(int index)
{
    // Connaissant l'indice du trou dans son tableau et le fait que ce trou est dans le tableau qui contient les trous du joueur 2,on va déterminer les coordonnées du centre de ce trou
    SDL_Point coordonates;
    coordonates.x = 150 + 120 + 180 * index;
    coordonates.y = 400 + 70;
    return coordonates;
}

void score_board(SDL_Renderer *renderer, TTF_Font *font, char *player1_name, char *player2_name, int nb_Awale_collected_by_player1, int nb_Awale_collected_by_player2)
{
    //Pour qu'il n' y ai pas deux SDL_RenderPresent(renderer) en concurrence
    SDL_Delay(50);

    // On affiche une belle image en arrière plan
    affich_image(renderer,"image/image_awale_2.webp");

    //Pour qu'il n' y ai pas deux SDL_RenderPresent(renderer) en concurrence
    SDL_Delay(50);

    // On crée des chaînes de caractères dans lesquelles on met les valeurs des nombres d'awalés collectés par chaque joueur
    char number_Awale_collected_by_player1[3];
    sprintf(number_Awale_collected_by_player1, "%d", nb_Awale_collected_by_player1);
    char number_Awale_collected_by_player2[3];
    sprintf(number_Awale_collected_by_player2, "%d", nb_Awale_collected_by_player2);

    // On affiche les pseudos des joueurs et leur score
    affich_text(renderer, font, player1_name, 50, 20, yellow, 30);
    affich_text(renderer, font, number_Awale_collected_by_player1, 50, 120, yellow, 30);

    affich_text(renderer, font, player2_name, 980, 20, yellow, 30);
    affich_text(renderer, font, number_Awale_collected_by_player2, 980, 120, yellow, 30);

    /*
        Si c'est le joueur qui a le plus grand score, on affiche que c'est lui le gagnat.
        Sinon si c'est le joueur 2 qui le plus grand score c'est lui le gagnant.
        Sinon il y a égalité.
    */

    if (nb_Awale_collected_by_player1 < nb_Awale_collected_by_player2)
    {
        affich_text(renderer, font, "The winner is", 440, 350, yellow, 30);
        affich_text(renderer, font, player2_name, 440, 420, red, 30);
    }
    else if (nb_Awale_collected_by_player1 > nb_Awale_collected_by_player2)
    {
        affich_text(renderer, font, "The winner is", 440, 350, yellow, 30);
        affich_text(renderer, font, player1_name, 440, 420, red, 30);
    }
    else
    {
        affich_text(renderer, font, "DRAW", 650, 380, red, 30);
    }

    SDL_RenderPresent(renderer);

    play_music("music/Applaudissements.mp3");
}
