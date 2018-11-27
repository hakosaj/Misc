

typedef struct {
    char name[20];
    float revenue;
    float price;
} Game;

typedef struct {
    int size;
    Game **games;
} ShopInventory;

void print_game(Game *newgame);
void list_games();
void clear_games();
int buy_game(char *name,int n);
void add_game(char *name, float revenue, float price);
int save_games(char *filename);
int load_games(char *filename);
int revenue_comparator(const void *v1, const void *v2);

