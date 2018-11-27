#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gameshopHead.h"

// Initialize global shop database
ShopInventory shop = { 0, NULL };

/**
 * Prints the specs of a single game
 */
void print_game(Game *newgame) {
    printf(
        "%-20s\t%.2f\t%.2f\n",
        newgame->name,
        newgame->price,
        newgame->revenue
    );
}

/**
 * The comparison function for qsort.
 */

int revenue_comparator(const void *v1, const void *v2)
{
    const Game *p1 = *(Game **)v1;
    const Game *p2 = *(Game **)v2;
       if (p1->revenue > p2->revenue)
          return -1;
       else if (p1->revenue < p2->revenue)
          return 1;
       else
          return 0;
}

/**
 * Prints header and all the games in the database. Uses qsort to sort the shop array by revenue.
 */
void list_games() {
    printf("%-20s\t%-2s\t%-2s\n", "NAME", "PRICE", "REVENUE");
    qsort(shop.games,shop.size,sizeof(Game*),revenue_comparator);
    for (int i = 0; i < shop.size; i++) {
        print_game(shop.games[i]);
    }
}

/**
 * Clears the database and frees memory
 */
void clear_games() {
    for (int i = 0; i < shop.size; i++) {
        free(shop.games[i]);
    }
    free(shop.games);
    shop.size = 0;
    shop.games = NULL;
}


/**
 * Buys the game, adds n times price to the game's profit. Yeah!
 */
int buy_game(char* name, int n){
            int i;
            int flag= -1;
            int size = shop.size;
            for (i=0;i<size;i++){
                if(strcmp(shop.games[i]->name,name)==0){
                    flag=i;
                }
               }
            if (flag>=0){
                shop.games[flag]->revenue+= ((shop.games[flag])->price)*n;
            }
            if (flag==-1){
                printf("Game not found!\n");
            }
           return flag;
           }


/**
 * Adds a game to the database.
 */
void add_game(char *name, float revenue, float price) {
    Game *newGame;
    newGame = malloc(sizeof(Game));
    strcpy(newGame->name, name);
    newGame->price = price;
    newGame->revenue = revenue;

    shop.size += 1;
    shop.games = realloc(shop.games, sizeof(Game) * shop.size);
    shop.games[shop.size - 1] = newGame;
}

/**
 * Writes games in inventory into a file.
 */
int save_games(char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Could not open file\n");
        return EXIT_FAILURE;
    }
    for (int i = 0; i < shop.size; i++) {
        fprintf(
            f,
            "%s %f %f\n",
            shop.games[i]->name,
            shop.games[i]->revenue,
            shop.games[i]->price
            );
    }
    fclose(f);
    return 0;
}


/**
 * Loads games from file and overwrites old ones
 */
int load_games(char *filename) {
    clear_games();
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Could not open file\n");
        return EXIT_FAILURE;
    }
    char name[20];
    float revenue,price;
    int r;
    while ((r = fscanf(f, "%s %f %f", name, &revenue, &price)) == 3) {
        add_game(name,revenue,price);
    }
    fclose(f);
    return 0;
}

/**
 * Reads the next command from user input and executes.
 * Each command checks its own correct format.
 * cmd 'X' is the identifier that marks the previous command as invalid.
 */
int _get_command() {

    char cmd;
    char name[20];
    float price;
    float n; /* This is the amount to buy in the buy function. We let it be a float and make sure it is an integer later on.*/
    int ret;
    char* getstest;
    char inputstring[31];
    char wastestring[31];
    getstest=fgets(inputstring,30,stdin);
    if (getstest==NULL){
        clear_games();
        printf("Games database cleared. \n");
        exit(0);
    }else{
        cmd=inputstring[0];}


    switch(cmd) {
        case 'A':
            ret=sscanf(inputstring,"%*c %s %f", name, &price);
            if (ret==2){
                add_game(name, 0, price);
                printf("Added game %s with price %f \n",name,price);
            }else{
                cmd='X';}
            break;

        case 'L':
            ret=sscanf(inputstring,"L %s",wastestring);
            if (ret==-1){
                list_games();
                cmd='L';}
            else{
                    cmd='X';}
            break;


        case 'Q':
            ret=sscanf(inputstring,"Q %s",wastestring);
            if (ret==-1){
                clear_games();
                printf("Games database cleared. \n");
                cmd='Q';}
            else{
                    cmd='X';}
            break;

        case 'B':
            cmd='B';
            ret=sscanf(inputstring,"%*c %s %f", name,&n);
            if (ret==2 ){
                    if((int)n==n || (int)n==n+1){
                        int a=(int)n;
                        int flag=buy_game(name, a);
                        if (flag!=-1){
                        printf("Added %d copies to %s.\n",a,name);
                        cmd='B';
                        }
                    }else{
                        printf("Input was not integer! \n");}
            }else{
                cmd='X';}
            break;


        case 'W':
            cmd='W';
            ret=sscanf(inputstring, "%*c %s", name);
            if (ret==1){
                save_games(name);
                printf("Games saved to file %s. \n",name);
                }else{

                cmd='X';}
            break;


        case 'O':
            cmd='O';
            ret=sscanf(inputstring, "%*c %s", name);
            if (ret==1){
                load_games(name);
                printf("Games loaded from file %s. \n",name);
                }else{
                cmd='X';}
            break;

        default:
            sscanf(inputstring,"%*[^\n]s");
            cmd='X';
            break;
    }
    return cmd;
}

/**
 * Runs the loop until we get a Q command.
 * The actual command evaluation happens in the _get_command-function.
 */
int main(void) {

    char input = 'N';
    printf("Give commands. Name has to be under 20 characters, otherwise the system crashes.\n");
    while (input != 'Q') {

        input = _get_command();
            if (input=='X'){
            printf("Invalid command! \n");
        }
    }
    printf("Goodbye!\n");

    return 0;
}

