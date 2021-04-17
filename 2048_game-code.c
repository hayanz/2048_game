#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

#define LEFT 'a'
#define RIGHT 'd'
#define UP 'w'
#define DOWN 's' // define the command of the direction

#define LIMIT 600 // set the time limit
#define SUCCESS 2048

typedef struct rank {
    char name[30];
    char success;
    int count;
    int time;
    int combo;
} RANK;

int board[5][5] = {0};
char success;
int count = 0;
int TimeRequired = 0;
int combo = 0; // initialize the records
int combo_1 = -1; // to count the combo when merging is occured in succession

void main_screen();
void how_to_play();
void draw_board(int lifetime);
void create_num();
int move_game(char key);
int create_check();
int check_result();
RANK get_record();
void add_record(FILE *fp);
void print_all(FILE *fp);
void print_record(RANK data);
int time_check(clock_t start, clock_t end);

int main()
{
    char command, command_1, command_2;
    char key;
    char new_key;
    int i = 0;
    int lifetime = LIMIT;
    int act = 0;
    int check = 0;
    int check_1 = 0;
    int before_check = 0;
    int check_success = 0;
    RANK user;
    FILE *fp;
    
    clock_t start, curr;

    if ((fp = fopen("rankinglist.txt", "a+")) == NULL)
    {
        fprintf(stderr, "Unable to open the file\n");
        exit(1);
    }

    while (1)
    {
        main_screen();
        command = getch();
        
        if (command == 'A' || command == 'a')
        {
            system("cls");
            create_num();
            draw_board(lifetime);

            start = clock();

            while (1)
            {
                do
                {
                    key = getch();
                } while (!(key == LEFT || key == RIGHT || key == UP || key == DOWN));    
                
                act = move_game(key); 

                curr = clock();
                if (time_check(start, curr) >= 1)
                {
                    lifetime-= time_check(start, curr); // count the time remaining
                    start = clock(); // initialize the starting time
                }

                check = check_result();
                before_check = create_check();

                if (before_check == 0) // check the case that the board is full but the tiles can be merged
                {
                    if (check == 0)
                    {
                        if (key == LEFT || key == RIGHT)
                        {
                            do
                            {
                                new_key = getch();
                            } while (!(new_key == UP || new_key == DOWN));    
                        }
                        else if (key == UP || key == DOWN)
                        {
                            do
                            {
                                new_key = getch();
                            } while (!(new_key == LEFT || new_key == RIGHT));
                        } // get the new input
                        
                        act = move_game(new_key); // move the tiles again

                        if (combo_1 >= 0)
                        {
                            combo = combo_1; // count the combo
                        }

                        if ((create_check()) == 1)
                        {
                            create_num();
                        }
                        system("cls");                    
                        draw_board(lifetime); // redraw the board

                        continue;
                    }

                }

                if (act > 0)
                {
                    count++;
                    TimeRequired = LIMIT - lifetime; // calculate the time required

                    if (lifetime <= 0)
                    {
                        success = 'X';

                        system("cls");                        
                        printf("===============\n");
                        printf("+++GAME OVER+++\n");
                        printf("===============\n");
                        printf("\n\n\n");    
                        printf("Press the 'A/a' button to continue ...");
                        
                        do
                        {
                            command_2 = getch();
                        } while (!(command_2 == 'A' || command_2 == 'a'));

                        system("cls");
                        add_record(fp);
                        system("cls");

                        break;                        
                    }
                    else
                    {
                        check = check_result(); // check whether the game is over or not

                        if (check == 1)
                        {
                            success = 'X';

                            system("cls");                        
                            printf("===============\n");
                            printf("+++GAME OVER+++\n");
                            printf("===============\n");
                            printf("\n\n\n");    
                            printf("Press the 'A/a' button to continue ...");
                            
                            do
                            {
                                command_2 = getch();
                            } while (!(command_2 == 'A' || command_2 == 'a'));

                            system("cls");
                            add_record(fp);
                            system("cls");

                            break;
                        }

                        for (i = 0; i < 25; i++)
                        {
                            if (board[i / 5][i % 5] == SUCCESS)
                            {
                                success = 'O';
                                check_success = 1;

                                system("cls");
                                printf("================\n");
                                printf("Congratulations!\n");
                                printf("+++GAME CLAER+++\n");
                                printf("================\n");
                                printf("\n\n\n");
                                printf("Press the 'A/a' button to continue ...");
                                
                                do
                                {
                                    command_2 = getch();
                                } while (!(command_2 == 'A' || command_2 == 'a'));

                                system("cls");
                                add_record(fp);
                                system("cls");

                                break;
                            }
                        }

                        if (check_success == 1) // check if the game is clear
                        {
                            break; // go to the main screen
                        }
                    }          
                }
                else if (act == 0)
                {
                    TimeRequired = LIMIT - lifetime; // calculate the time required
                    check = check_result(); // check whether the game is over or not
                    
                    if (check == 1)
                    {
                        success = 'X';

                        system("cls");                        
                        printf("===============\n");
                        printf("+++GAME OVER+++\n");
                        printf("===============\n");
                        printf("\n\n\n");    
                        printf("Press the 'A/a' button to continue ...");
                        
                        do
                        {
                            command_2 = getch();
                        } while (!(command_2 == 'A' || command_2 == 'a'));

                        system("cls");
                        add_record(fp);
                        system("cls");

                        break;                        
                    }          
                }

                if (combo_1 >= 0)
                {
                    combo = combo_1; // count the combo
                }

                if ((create_check()) == 1)
                {
                    create_num();
                }
                system("cls");                    
                draw_board(lifetime);
            }

            for (i = 0; i < 25; i++)
            {
                board[i / 5][i % 5] = 0;
            } // reset the board

            count = 0;
            TimeRequired = 0;
            combo = 0;
            combo_1 = -1;
            lifetime = LIMIT; // reset the values
        }
        else if (command == 'B' || command == 'b')
        {
            system("cls");
            how_to_play();

            do
            {
                command_1 = getch();
            } while (!(command_1 == 'B' || command_1 == 'b'));
            
            system("cls");
        }
        else if (command == 'C' || command == 'c')
        {
            system("cls");
            print_all(fp);
            printf("\n\n");
            printf("Press the 'B/b' button to go to the main screen...");
            
            do
            {
                command_1 = getch();
            } while (!(command_1 == 'B' || command_1 == 'b'));

            system("cls");          
        }
        else if (command == 'Q' || command == 'q')
        {
            break;
        }
        else
        {
            system("cls");
        }
    }
    
    return 0;
}

void main_screen() // print the main screen of the game
{
    printf(",--.   ,--.        ,--.            ,--.   ,--.                        \n");
    printf("|   `.'   | ,--,--.`--',--,--,     |   `.'   | ,---. ,--,--, ,--.,--. \n");
    printf("|  |'.'|  |' ,-.  |,--.|      |    |  |'.'|  || .-. :|      ||  ||  | \n");
    printf("|  |   |  |` '-'  ||  ||  ||  |    |  |   |  |`   --.|  ||  |'  ''  ' \n");
    printf("`--'   `--' `--`--'`--'`--''--'    `--'   `--' `----'`--''--' `----'  \n");
    printf("======================================================================\n");
    printf("A/a. GAME START\n");
    printf("B.b. HOW TO PLAY\n");
    printf("C/c. RANKING\n");
    printf("Q/q. EXIT\n");
    printf("======================================================================\n");
}

void how_to_play() // print the manual of the game
{
    printf("==========================HOW TO PLAY==========================\n");
    printf("Use the following keys to move the tiles : \n\n");
    printf("         |  w : UP  |\n");
    printf("a : LEFT | s : DOWN | d : RIGHT\n\n");
    printf("When two tiles with the SAME number touch, they MERGE INTO ONE!\n");
    printf("This game has a time limit of ONLY '600' seconds(10 minuites).\n");
    printf("If the time is OVER, you will LOSE the game.\n");
    printf("And when the tiles CANNOT be merged ANYMORE, the game is OVER.\n");
    printf("If the number '2048' is created, you will WIN the game!\n");
    printf("==============================================================\n");
    printf("\n\n\n");
    printf("Press the 'B/b' button to go to the main screen...");
}

void draw_board(int lifetime) // show the game screen
{
    int i = 0;
    int j = 0;

    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 5; j++)
        {
            if (board[i][j] == 0)
            {
                printf("\t|");
            }
            else
            {
                printf("%d\t|", board[i][j]);
            }
        }
        printf("\n");
        printf("----------------------------------------");
        printf("\n");
    }
    printf("\n\n\n");
    printf("MOVE : %d\n", count);
    printf("COMBO : %d\n", combo);
    printf("\n\n\n");
    printf("\rTIME REMAINING : %d sec", lifetime); // show the remaining time on the screen
}

void create_num()
{
    int i = 0;
    int j = 0;
    int count = 0;
    int num = 0;

    srand((unsigned int) time(NULL));

    for (count = 0; count < 2; count++) // add the new number twice
    {
        if (rand()%10 < 9)
        {
            num = 2; // create 2 with a 90% chance
        }
        else
        {
            num = 4; // create 4 with a 10% chance
        }

        do
        {
            i = rand()%5;
            j = rand()%5;
        } while (board[i][j] != 0);
        
        board[i][j] = num;        
    }
}

int move_game(char key)
{
    int act = 0; // check the change of the board
    int i = 0;
    int j = 0;
    int k = 0;
    
    if (key == LEFT)
    {
        for (i = 0, act = 0; i < 5; i++)
        {
            for (j = 1; j < 5; j++)
            {
                for (k = j; k > 0; k--)
                {
                    if (board[i][k] == 0 || board[i][k] > 5000)
                    {
                        break;
                    }
                    else if (board[i][k - 1] != 0 && board[i][k - 1] != board[i][k])
                    {
                        break;
                    }
                    else if (board[i][k - 1] == 0)
                    {
                        board[i][k - 1] = board[i][k];
                    }
                    else if (board[i][k] == board[i][k - 1])
                    {
                        board[i][k - 1] *= 2;
                        board[i][k - 1] += 5000; // add 5000 not to merge again

                        combo_1++;
                    }
                    board[i][k] = 0;
                    act++;
                }
            }
        }
    }
    else if (key == RIGHT)
    {
        for (i = 0, act = 0; i < 5; i++)
        {
            for (j = 3; j >= 0; j--)
            {
                for (k = j; k < 4; k++)
                {
                    if (board[i][k] == 0 || board[i][k] > 5000)
                    {
                        break;
                    }
                    else if (board[i][k + 1] != 0 && board[i][k + 1] != board[i][k])
                    {
                        break;
                    }
                    else if (board[i][k + 1] == 0)
                    {
                         board[i][k + 1] = board[i][k];
                    }
                    else if (board[i][k] == board[i][k + 1])
                    {
                        board[i][k + 1] *= 2;
                        board[i][k + 1] += 5000;

                        combo_1++;
                    }
                    board[i][k] = 0;
                    act++;
                }
            }
        }
    }
    else if (key == UP)
    {
        for (j = 0, act = 0; j < 5; j++)
        {
            for (i = 1; i < 5; i++)
            {
                for (k = i; k > 0; k--)
                {
                    if (board[k][j] == 0 || board[k][j] > 5000)
                    {
                        break;
                    }
                    else if (board[k - 1][j] != 0 && board[k - 1][j] != board[k][j])
                    {
                        break;
                    }
                    else if (board[k - 1][j] == 0)
                    {
                        board[k - 1][j] = board[k][j];
                    }
                    else if (board[k][j] == board[k - 1][j])
                    {
                        board[k - 1][j] *= 2;
                        board[k - 1][j] += 5000;

                        combo_1++;
                    }
                    board[k][j] = 0;
                    act++;
                }
            }
        }
    }
    else if (key == DOWN)
    {
        for (j = 0, act = 0; j < 5; j++)
        {
            for (i = 3; i >= 0; i--)
            {
                for (k = i; k < 4; k++)
                {
                    if (board[k][j] == 0 || board[k][j] > 5000)
                    {
                        break;
                    }
                    else if (board[k + 1][j] != 0 && board[k + 1][j] != board[k][j])
                    {
                        break;
                    }
                    else if (board[k + 1][j] == 0)
                    {
                        board[k + 1][j] = board[k][j];
                    }
                    else if (board[k][j] == board[k + 1][j])
                    {
                        board[k + 1][j] *= 2;
                        board[k + 1][j] += 5000;

                        combo_1++;
                    }
                    board[k][j] = 0;
                    act++;
                }
            }
        }
    }

    for (i = 0; i < 25; i++)
    {
        if (board[i / 5][i % 5] > 5000)
        {
            board[i / 5][i % 5] -= 5000;
        }
    } // revert to the previous number

    return act;
}

int create_check() // check if the new numbers can be added
{
    int i = 0;
    int j = 0;
    int cnt = 0;

    for (i = 0; i < 25; i++)
    {
        for (j = 0; j < 25; j++)
        {
            if (board[i][j] != 0)
            {
                cnt++;
            }
        }
    }

    if (cnt == 24 || cnt == 25)
    {
        return 0; // unable to create the number
    }
    else
    {
        return 1; // able to create the number
    }
}

int check_result() // check the result of the game if the time is not over
{
    int i = 0;
    int j = 0;
    int result = 1; // assume that the game is over

    for (i = 0; i < 25; i++) // check the presence of the blank
    {
        if (board[i / 5][i % 5] == 0)
        {
            result = 0; // continue the game
            return result;
        }
    }

    for (i = 0; i < 4; i++) // check the last row of the board
    {
        if (board[i][4] == board[i + 1][4])
        {
            result = 0; // continue the game
            return result;
        }
    }

    for (i = 0; i < 4; i++) // check the values in the middle of the board
    {
        for (j = 0; j < 4; j++)
        {
            if (board[i][j] == board[i + 1][j] || board[i][j] == board[i][j + 1])
            {
                result = 0; // continue the game
                return result;
            }
        }
    }

    for (j = 0; j < 4; j++) // check the last column of the board
    {
        if (board[4][j] == board[4][j + 1])
        {
            result = 0; // continue the game
            return result;
        }
    }                
    
    return result;
}

RANK get_record() // get the new record
{
    RANK data;
    
    fflush(stdin);

    printf("Enter the name of the user : ");
    gets(data.name);
    data.success = success;
    data.count = count;
    data.time = TimeRequired;
    data.combo = combo;

    return data;
}

void add_record(FILE *fp) // save the record to the text file
{
    RANK data;

    data = get_record();
    fseek(fp, 0, SEEK_END);
    fwrite(&data, sizeof(data), 1, fp);
}

void print_record(RANK data) // print each record
{
    printf("%s\t%c\t%d\t%d\t%d", data.name, data.success, data.count, data.time, data.combo);
    printf("\n");
}

void print_all(FILE *fp) // print all records saved before
{
    RANK data;

    fseek(fp, 0, SEEK_SET);
    fflush(stdin);

    printf("NAME\tSUCCESS\tMOVE\tTIME(s)\tCOMBO");
    printf("\n");

    while (1)
    {
        fread(&data, sizeof(data), 1, fp);
        if (feof(fp))
        {
            break;
        }
        print_record(data);
    }
}

int time_check(clock_t start, clock_t end) // calculate the time passed
{
    int check = 0;
    int required = 0;

    required = (int) (end - start) / CLOCKS_PER_SEC;

    return required;
}