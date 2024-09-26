
/*
 * Program: C Programming Assignment 3 Open Ended - Rush Hour
 * Author: Rowen King
 * Date Written: 1/04/23
 *
 * Rush hour is a traffic jam logic game where you have cars stuck in a traffic jam  in a grid and you
 * have to rearrange the cars so that the red car (X) can drive out of the exit (=).
 * The cars can only either move horizontally or vertically and can be car sized (2) or truck sized (3).
 *
 * This program allows the user to start the game and select a level. This loads in the grid for the level.
 * The types of car in the grid are identified by the program and the user is then asked to choose which car
 * they would like to move followed by which direction. The program checks to see if the move is possible and
 * will move the car if it is possible. Then the user is asked whether they would like to continue the program
 * and play again or exit the program.
 *
 */

 /* Header files */
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <windows.h>
 #include <ctype.h>

 /* #define is not a global variable. These variables cannot be changed */
 #define TRUE 1
 #define FALSE 0
 #define CAR_LOOKUP_TABLE_LEN 21
 #define CAR_STRING "XABCDEFGHIJKOPQR.|=_#"
 #define MAX_LEN 128
 #define GRID_DIMENSION_X 8
 #define GRID_DIMENSION_Y 8
 #define EXIT_LOCATION_X 3
 #define EXIT_LOCATION_Y 7

/* This enum is used to assign the levels numbers and would make it easier to add more levels */
typedef enum {
    BEGINNER = 1,
    INTERMEDIATE,
    ADVANCED,
    EXPERT,
    PRACTICE,
    unused = 999
} game_level_t;

/* This enum is used to assign numbers to the different characters included in the grid. */
typedef enum {
    RED_CAR,
    LIME_CAR,
    ORANGE_CAR,
    BLUE_CAR,
    PINK_CAR,
    PURPLE_CAR,
    GREEN_CAR,
    SILVER_CAR,
    VIOLET_CAR,
    YELLOW_CAR,
    GREY_CAR,
    CYAN_CAR,
    AQUA_CAR,
    WHITE_CAR,
    NAVY_CAR,
    MAGENTA_CAR,
    DOT,
    LINE,
    EXIT_CAR,
    UNDERSCORE,
    UNUSED = 666,
} car_colour_t;

typedef struct {
    car_colour_t car_colour;
    int debug_count; /* How many times a car was in this location */
} grid_element_t;

 struct GameGrid {
     /* Array of Structures within a structure */
    grid_element_t matrix[GRID_DIMENSION_X][GRID_DIMENSION_Y];
    unsigned int bitmask; /* where car_colour_t is used */
};

/* This structure is used to store the row and column locations of the car parts */
struct RowsCols {
    int row_1;
    int row_2;
    int row_3;
    int col_1;
    int col_2;
    int col_3;
};

/* Function Prototype definitions */
/* These functions are used in other functions as conversions, checks or inputs */
car_colour_t car_lookup_NumToLet (int);
int car_lookup_LetToNum (car_colour_t);
char *numb_to_colour (car_colour_t);
int ColourNum (int);
int Num_To_ColourNum (struct GameGrid, int, int);
void SetColour (int);
int validate_car_against_level (struct GameGrid, int);
void InvalidInputDisplay (struct GameGrid);
void ChecksFiles (void);
void InitialFileCheck (char *);
void DisplayTextImage (char *);
/* These functions are setting up the menu and level choices and displaying the grid depending on the level */
int Menu_Choices (int, int);
game_level_t levelChoice (void);
void Level_StartUp (struct GameGrid *, char *);
void gridDisplay (struct GameGrid);
/* These functions are where the user chooses the car and the locations of the car are found */
void TypesOfCarsInGrid (struct GameGrid);
int ChooseCarToMove (struct GameGrid);
void PosCar_1 (struct GameGrid, int, struct RowsCols *);
char PosCar_2 (struct GameGrid, int, struct RowsCols *);
char PosCar_3 (struct GameGrid, int, struct RowsCols *);
void Which_Way (int *);
/* These functions identify where the car will be moving to and if it can move */
void Dot_Checker_Horz (grid_element_t [GRID_DIMENSION_X][GRID_DIMENSION_Y], int, int, int *, int *, struct RowsCols *);
void Dot_Checker_Vert (grid_element_t [GRID_DIMENSION_X][GRID_DIMENSION_Y], int, int, int *, int *, struct RowsCols *);
int Check_Dot (car_colour_t, int);
void MoveCarHorz (struct GameGrid *, int, int, int, struct RowsCols *);
void MoveCarVert (struct GameGrid *, int, int, int, struct RowsCols *);

int main () {

    struct GameGrid g1;
    struct RowsCols rows_and_cols;

    char quit = 'y', orientation;
    /* Game Statistics */
    int LevelScore = 0, NumCarsMoved = 0, NumWrongMoves = 0;
    int MenuChoice, ChoosenCarNum, size, direction, Dot_Location_Check, CanEnd;

    game_level_t GameLevel;

    /* Start up check that all the files can be read */
    ChecksFiles ();

    DisplayTextImage("Welcome.txt");
    Sleep(5000);

    do {

        MenuChoice = Menu_Choices(LevelScore, NumCarsMoved);
        NumCarsMoved = 0;
        NumWrongMoves = 0;

        if (MenuChoice == 1) {

            GameLevel = levelChoice ();
            /* Reads in the chosen level */
            switch (GameLevel) {
                case 1:
                    Level_StartUp (&g1, "BeginnerGrid.txt");
                    break;
                case 2:
                    Level_StartUp (&g1, "IntermediateGrid.txt");
                    break;
                case 3:
                    Level_StartUp (&g1, "AdvancedGrid.txt");
                    break;
                case 4:
                    Level_StartUp (&g1, "ExpertGrid.txt");
                    break;
                case 5:
                    Level_StartUp (&g1, "PracticeGrid.txt");
                    break;
                default :
                    (void) fprintf (stderr, "Unknown level value\n");
                    exit(1);
            }

            do {
                do {
                    gridDisplay (g1);
                    printf("\n\tNumber of Moves Made    = %3d\n", NumCarsMoved);
                    printf("\tNumber of Invalid Moves = %3d\n", NumWrongMoves);
                    TypesOfCarsInGrid (g1);
                    ChoosenCarNum = ChooseCarToMove (g1);

                    if ((ChoosenCarNum < RED_CAR) || (ChoosenCarNum > DOT)) {
                        InvalidInputDisplay (g1);
                    }

                } while ((ChoosenCarNum < RED_CAR) || (ChoosenCarNum > DOT));

                PosCar_1 (g1, ChoosenCarNum, &rows_and_cols);

                if ((ChoosenCarNum >= RED_CAR) && (ChoosenCarNum <= CYAN_CAR)) {
                    size = 2;
                    orientation = PosCar_2 (g1, ChoosenCarNum, &rows_and_cols);

                } else if((ChoosenCarNum >= AQUA_CAR) && (ChoosenCarNum <= MAGENTA_CAR)){
                    size = 3;
                    orientation = PosCar_3 (g1, ChoosenCarNum, &rows_and_cols);
                }

                switch (orientation) {

                    case 'h':   /* if the car is horizontal */
                        printf("\nThis car can move horizontally.\n");
                        printf("\nChoose Direction to Move\nLeft  (1)\nRight (2)\n");
                        /* Checking whether the car is able to move */
                        Which_Way (&direction);
                        Dot_Checker_Horz (g1.matrix, direction, size, &Dot_Location_Check, &CanEnd, &rows_and_cols);
                        if (Check_Dot (Dot_Location_Check, CanEnd)) {
                            MoveCarHorz (&g1, ChoosenCarNum, size, direction, &rows_and_cols);   /* Moves the car */
                            NumCarsMoved++;
                        } else {
                            InvalidInputDisplay (g1);
                            NumWrongMoves++;
                        }
                        break;

                    case 'v':    /* If the car is vertical */
                        printf("\nThis car can move vertically.\n");
                        printf("\nChoose Direction to Move\nUp   (1)\nDown (2)\n");
                        /* Checking whether the car is able to move */
                        Which_Way (&direction);
                        Dot_Checker_Vert (g1.matrix, direction, size, &Dot_Location_Check, &CanEnd, &rows_and_cols);
                        if (Check_Dot (Dot_Location_Check, CanEnd)) {
                            MoveCarVert (&g1, ChoosenCarNum, size, direction, &rows_and_cols);   /* Moves the car */
                            NumCarsMoved++;
                        } else {
                            InvalidInputDisplay (g1);
                            NumWrongMoves++;
                        }
                        break;

                }

            } while (g1.matrix[EXIT_LOCATION_X][EXIT_LOCATION_Y].car_colour == EXIT_CAR);
            /* Counts how many levels have been completed (playing the same level twice is also included) */
            LevelScore++;

            do {
                DisplayTextImage ("EndPage.txt");
                printf("\n\tNumber of Moves Made    = %3d\n", NumCarsMoved);
                printf("\tNumber of Invalid Moves = %3d\n", NumWrongMoves);
                printf("\nWould you like to continue playing?\nYes\t(y)\nNo \t(n)\n");
                scanf("%c", &quit);
                fflush(stdin);
            } while ((quit != 'y') && (quit != 'n'));
        }
        else if (MenuChoice == 3) {
                quit = 'n';
        }

        system("cls");
        DisplayTextImage ("ThanksForPlaying.txt");
    } while(quit == 'y');
}



/* Functions */

/* Lookup table to convert the car value from a number to a letter */
car_colour_t car_lookup_NumToLet (int n) {
    /* # indicates we have overrun the array */
    const char *car_lookup_tab = CAR_STRING;
    return (car_lookup_tab[n]);
}

/* REFACTOR ALERT : Slow but sure. Look here to speed up */
/* Lookup table to convert the car value from a letter to a number */
int car_lookup_LetToNum (car_colour_t colour) {
    /* # indicates we have overrun the array */
    const char *car_lookup_tab = CAR_STRING;
    for (int i = 0; i < CAR_LOOKUP_TABLE_LEN; i++) {
        if (car_lookup_tab[i] == colour) {
            return (i);
        }
        /* overrun */
        if (car_lookup_tab[i] == '#') {
            fprintf(stderr, "\nError: overrun of array # found\n");
            return (-1);
        }
    }
    fprintf(stderr, "unknown colour %u\n", colour);
    exit (1);
}

/* Lookup table to convert the car value from a number to a string with the colour name */
char *numb_to_colour (car_colour_t t) {

    const char *tab[] = {
        /* Cars of length 2 */
        "red car",
        "lime car",
        "orange car",
        "blue car",
        "pink car",
        "purple car",
        "green car",
        "silver car",
        "violet car",
        "yellow car",
        "grey car",
        "cyan car",
        /* Cars of length 3 */
        "aqua car",
        "white car",
        "navy car",
        "magenta car",
        "dot",
        "line",
        "car exit",
        "underscore",
        "UNUSED"
    };
    /* The above should be global and would avoid the need to strdup() */
    /* Going to ignore memory leaks */
    return (strdup(tab[t]));
}

/* Lookup Table to assign colours for output text */
int ColourNum (int i) {
    const int car_to_colour[] = { 4, 10, 6, 9, 12, 5, 2, 7, 13, 14, 10, 3, 11, 15, 1, 5, 15, 15, 15, 15, 99999};
    return (car_to_colour[i]);
}

/* Lookup Table to assign colours to car letters for output text */
int Num_To_ColourNum (struct GameGrid grid, int i, int j) {
    const int car_to_colour[] = { 4, 10, 6, 9, 12, 5, 2, 7, 13, 14, 10, 3, 11, 15, 1, 5, 15, 15, 15, 15, 99999};
    return (car_to_colour[grid.matrix[i][j].car_colour]);
}

/* This function is used to colour the output text */
void SetColour (int ForgC) {
    WORD wColour;
    HANDLE hStdOut = GetStdHandle (STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    /* csbi is used for the wAttributes word. */
    if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
        /* Mask out all but the background attribute, and add in the foreground colour */
        wColour = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColour);
    }

    return;
}

/* Checks what cars are in the grid */
int validate_car_against_level (struct GameGrid g1, int car) {
    if (g1.bitmask & (1 << car)) {
            return (TRUE);
    }
    return (FALSE);
}

/* Function that displays the error if an invalid input is used whilst moving the car */
void InvalidInputDisplay (struct GameGrid g1) {
    system("cls");
    char invalid;

    do {
        gridDisplay (g1);
        printf("\nInvalid Input\nPress Enter to continue\n");
        scanf("%c", &invalid);
        fflush(stdin);
    } while (invalid != 10);

    system("cls");
}

void ChecksFiles (void) {

    InitialFileCheck ("BeginnerGrid.txt");
    InitialFileCheck ("IntermediateGrid.txt");
    InitialFileCheck ("AdvancedGrid.txt");
    InitialFileCheck ("ExpertGrid.txt");
    InitialFileCheck ("PracticeGrid.txt");
    InitialFileCheck ("TitlePage.txt");
    InitialFileCheck ("HowToPlay.txt");
    InitialFileCheck ("LevelChoice.txt");
    InitialFileCheck ("RushHour.txt");
    InitialFileCheck ("EndPage.txt");

}

/* At the start of the program checks if all the files required to run the program can be found */
void InitialFileCheck (char *filename) {
    FILE *in = NULL;

    if ((in = fopen (filename, "r")) == NULL) {
        fprintf (stderr, "%s could not be found\n", filename);
        fprintf (stderr, "Closing Program ...\n");
        exit (1);
    }

}

/* Reads in text files for displaying text images */
void DisplayTextImage (char *filename) {
    /* Clears screen by going into the operating system and executing cls */
    int FlashColour = 0;
    do {
        if (filename == ("EndPage.txt")) {
            /* A flashing effect is created by running through the possible colours from SetColour. */
            SetColour(FlashColour);
            FlashColour++;
            /* Sleep (from windows.h) is used to delay for 30ms */
            Sleep(30);
        }
        else if (filename == ("Welcome.txt")){
            SetColour(4);
            FlashColour = 16;
        }
        else {
            FlashColour = 16;
            SetColour(15);
        }
        system("cls");
        FILE *in = NULL;
        char read_string[MAX_LEN];

        /* Opens file and checks if file exists */
        if ((in = fopen (filename, "r")) == NULL) {
            printf ("%s could not be found\n", filename);
            printf ("Closing Program ...\n");
            exit (1);
        }

        while (fgets(read_string, sizeof(read_string), in) != NULL) {
            /* ignore comments start with # in 1st char where references for the text images are located in the files */
            if (read_string[0] != '#') {
                printf("%s",read_string);
            }
        }
        fclose(in);
    } while (FlashColour <= 15);
}

/* Sets up the title page and gives the user menu options */
int Menu_Choices (int LevelScore, int NumCarsMoved) {
    int menu = 0;

    do {
        do {
            DisplayTextImage ("TitlePage.txt");
            printf("\n");
            printf("\n\t|           No. Levels Played  = %3d|\n\t|No. Moves Made in Last Level  = %3d|\n", LevelScore, NumCarsMoved);
            printf("\nChoose what you would like to do using the menu displayed above:\n");
            scanf("%d", &menu);
            fflush(stdin);
        } while ((menu != 1) && (menu != 2) && (menu != 3));

        if (menu == 2) {
            char BackToTitle;
            do {
                DisplayTextImage ("HowToPlay.txt");
                printf("\n\nPress Enter to Return\n");
                scanf("%c", &BackToTitle);
                fflush(stdin);
            } while (BackToTitle != 10);
        }

    } while ((menu != 1) && (menu != 3));

    return (menu);
}

/* Choosing the level the user wants to play */
game_level_t levelChoice (void) {
    int level = 0;
    system("cls");

    do {
        DisplayTextImage ("LevelChoice.txt");
        scanf("%d", &level);
        fflush(stdin);
        system("cls");
    } while ((level < BEGINNER) || (level > PRACTICE));

    return ((game_level_t)level);
}

/* Reading in the level text file and assigning it to the structure */
void Level_StartUp(struct GameGrid *grid, char *filename) {
    FILE *in;
    /* Reads in the level file */
    if ((in = fopen (filename, "r")) == NULL) {
        fprintf (stderr, "%s could not be found\n", filename);
        fprintf (stderr, "Closing Program ...\n");
        exit (1);
    }

    for (int i = 0; i < GRID_DIMENSION_X; i++) {
        for (int j = 0; j < GRID_DIMENSION_Y; j++) {
            char temp_c;

            if (fscanf (in, "%c\n", &temp_c) < 0) {
    	        fprintf(stderr, "fscanf failed on value [%d][%d]\n", i, j);
    	        exit(1);
            }

            grid->matrix[i][j].car_colour = car_lookup_LetToNum(temp_c);
            /* turn bit on to record which cars are used */
            grid->bitmask |= (1 << grid->matrix[i][j].car_colour);
        }
    }
    fclose (in);
}

/* Displays the Rush Hour Grid to the screen */
void gridDisplay (struct GameGrid grid) {
    system("cls");
    DisplayTextImage ("RushHour.txt");

    for (int i = 0; i < GRID_DIMENSION_X; i++) {
        printf("\n      ");

        for (int j = 0; j < GRID_DIMENSION_Y; j++) {
            int c = Num_To_ColourNum(grid, i, j); /* Setting the colour of each */
            SetColour(c);
            printf ("  %c  ", car_lookup_NumToLet(grid.matrix[i][j].car_colour));
            fflush(stdout);
            SetColour(15); /* White */
        }

        printf("\n");
    }

    SetColour(15);
}

/* Works out what cars are in the grid and prints them out */
void TypesOfCarsInGrid(struct GameGrid g1){
    char CarColour;
    char buf[64];
    int cnt = 0;
    printf("\n");

    for (int i = 0; i < GRID_DIMENSION_X; i++) {

        for (int j = 0; j < GRID_DIMENSION_Y; j++) {

            int s1 = g1.matrix[i][j].car_colour;
            CarColour = car_lookup_NumToLet(s1);
            buf[cnt++] = CarColour;
        }

    }
    /* Loop until the last colour which is DOT */
    for (int i = 0; i < DOT; i++){

        if (strchr(buf, car_lookup_NumToLet(i))) {
            /* Using lookup table to find what the corresponding colour is */
            SetColour(ColourNum(i));
            /* Printing all the cars shown in the grid */
            printf("        %11s = %c\n",numb_to_colour(i), car_lookup_NumToLet(i));
            SetColour(15);  /* White */
        }

    }

}

/* Asks the user to choose what car they would like as long as it is in the grid */
int ChooseCarToMove (struct GameGrid g1) {
    int CarNum;
    char CarLet;
    printf("\nChoose the car which you would like to move using the letters given (use lower case letters):\n");
    /* Only the first character is considered */
    scanf("%c", &CarLet);
    fflush(stdin);
    CarLet = toupper (CarLet); /* Changing lower case inputs to upper case using ctype.h */
    CarNum = car_lookup_LetToNum (CarLet);
    /* Prevents function being called with an invalid index */

    if ((CarNum == -1) || (validate_car_against_level(g1, CarNum) == FALSE)) {
        CarNum = -1;
    }

    return (CarNum);
}

/* Reads through the grid to find the first of all the letters corresponding to a car and what row and column there are in */
void PosCar_1 (struct GameGrid g1, int ChoosenCarNum, struct RowsCols *rc) {

    for (int i = 0; i < GRID_DIMENSION_X; i++) {

        for (int j = 0; j < GRID_DIMENSION_Y; j++) {

            if (g1.matrix[i][j].car_colour == ChoosenCarNum) {
                /* If car number is found, the location is recorded */
                rc->row_1 = i;
                rc->col_1 = j;
            }
        }
    }
}

/*
 * For a car with size of 2 squares, it uses the first row and column location to find where the second location is and
 * if it is below, above, left or right and therefore assigns whether it is horizontal or vertical
 */
char PosCar_2 (struct GameGrid g1, int ChoosenCarNum, struct RowsCols *rc) {
    char VertHorz;
    /* If the second location of the car is above position 1 */
    if ((g1.matrix[rc->row_1 - 1][rc->col_1].car_colour) == ChoosenCarNum) {
        rc->row_2 = rc->row_1 - 1;
        rc->col_2 = rc->col_1;
        VertHorz = 'v';
    }
    /* If the second part of the car is left of position 1 */
    else if ((g1.matrix[rc->row_1][rc->col_1-1].car_colour) == ChoosenCarNum) {
        rc->row_2 = rc->row_1;
        rc->col_2 = rc->col_1 - 1;
        VertHorz = 'h';
    }
    return (VertHorz);
}

/*
 * For a car with size of 3 squares, it uses the first row and column location to
 * find where the second and third location is and
 * if it is below, above, left or right and therefore assigns whether it is horizontal or vertical
 */
char PosCar_3 (struct GameGrid g1, int ChoosenCarNum, struct RowsCols *rc) {
    char VertHorz;
    /* If the second part of the car is above position 1 and the third part of the car is above position 2 */
    if (((g1.matrix[rc->row_1 - 1][rc->col_1].car_colour) == ChoosenCarNum) &&
        ((g1.matrix[rc->row_1 - 2][rc->col_1].car_colour) == ChoosenCarNum)) {
        rc->row_2 = rc->row_1 - 1;
        rc->col_2 = rc->col_1;
        rc->row_3 = rc->row_1 - 2;
        rc->col_3 = rc->col_1;
        VertHorz = 'v';
    }
    /* If the second part of the car is left of position 1 and the third part of the car is left of position 2 */
    else if (((g1.matrix[rc->row_1][rc->col_1 - 1].car_colour) == ChoosenCarNum) &&
             ((g1.matrix[rc->row_1][rc->col_1 - 2].car_colour) == ChoosenCarNum)) {
        rc->row_2 = rc->row_1;
        rc->col_2 = rc->col_1 - 1;
        rc->row_3 = rc->row_1;
        rc->col_3 = rc->col_1 - 2;
        VertHorz = 'h';
    }
    return (VertHorz);
}

/* The user enters in which direction they want the chosen car to move (up, down, left or right) */
void Which_Way (int *direction) {
    scanf("%d", &*direction);
    fflush(stdin);
}

/* Checks horizontally if the car can move in the direction the user wants */
/* demonstrates passing array of structs rather than struct of array of structs */
void Dot_Checker_Horz (grid_element_t matrix[GRID_DIMENSION_X][GRID_DIMENSION_Y], int direction, int size,
                       int *Dot_Location_Check, int *CanEnd, struct RowsCols *rc) {
    /* Check if can move Left */
    if (direction == 1) {

        if (size == 2) {
            *Dot_Location_Check = matrix[rc->row_2][rc->col_2 - 1].car_colour;
        }
        else if (size == 3) {
            *Dot_Location_Check = matrix[rc->row_3][rc->col_3 - 1].car_colour;
        }
        *CanEnd = FALSE;

    }
    /* Check if can move Right */
    else if (direction == 2) {
        *Dot_Location_Check = matrix[rc->row_1][rc->col_1 + 1].car_colour;

        if (matrix[rc->row_1][rc->col_1].car_colour == 0) {
            *CanEnd = TRUE;
        }
        else {
            *CanEnd = FALSE;
        }

    }

}

/* Checks vertically if the car can move in the direction the user wants */
/* demonstrates passing array of structs rather than struct of array of structs */
void Dot_Checker_Vert (grid_element_t matrix[GRID_DIMENSION_X][GRID_DIMENSION_Y], int direction, int size,
                       int *Dot_Location_Check, int *CanEnd, struct RowsCols *rc) {
    /* Check if can move Up */
    if (direction == 1) {

        if (size == 2) {
            *Dot_Location_Check = matrix[rc->row_2 - 1][rc->col_2].car_colour;
        }
        else if (size == 3) {
            *Dot_Location_Check = matrix[rc->row_3 - 1][rc->col_3].car_colour;
        }

    }
    /* Check if can move Down */
    else if (direction == 2) {
            *Dot_Location_Check = matrix[rc->row_1 + 1][rc->col_1].car_colour;
    }

    *CanEnd = FALSE;
}

/* Checks if there is a dot where the car is to move or if the red car is exiting then the game can end */
int Check_Dot (car_colour_t Dot_Location_Check, int CanEnd) {

    if ((Dot_Location_Check == DOT) || ((Dot_Location_Check == EXIT_CAR) && (CanEnd == TRUE))) {
        return (TRUE);
    }

    return (FALSE);
}

/* Moves the car horizontally (left or right) */
void MoveCarHorz (struct GameGrid *g1, int ChoosenCarNum, int size, int direction, struct RowsCols *rc) {
    /* Move Left */
    if (direction == 1) {
        g1->matrix[rc->row_1][rc->col_1 - 1].car_colour = (ChoosenCarNum);
        g1->matrix[rc->row_2][rc->col_2 - 1].car_colour = (ChoosenCarNum);

        if (size == 2) {
            g1->matrix[rc->row_1][rc->col_1].car_colour = DOT;
        }
        else if (size == 3) {
            g1->matrix[rc->row_3][rc->col_3-1].car_colour = (ChoosenCarNum);
            g1->matrix[rc->row_1][rc->col_1].car_colour = DOT;
        }
      }
    /* Move Right */
    else if (direction == 2) {
        g1->matrix[rc->row_1][rc->col_1 + 1].car_colour = (ChoosenCarNum);
        g1->matrix[rc->row_2][rc->col_2 + 1].car_colour = (ChoosenCarNum);

        if (size == 2) {
            g1->matrix[rc->row_2][rc->col_2].car_colour = DOT;
        }
        else if (size == 3) {
            g1->matrix[rc->row_3][rc->col_3 + 1].car_colour = (ChoosenCarNum);
            g1->matrix[rc->row_3][rc->col_3].car_colour = DOT;
        }
    }

}

/* Moves the car vertically (up or down) */
void MoveCarVert (struct GameGrid *g1, int ChoosenCarNum, int size, int direction, struct RowsCols *rc) {
    /* Move Up */
    if (direction == 1) {
        g1->matrix[rc->row_1 - 1][rc->col_1].car_colour = (ChoosenCarNum);
        g1->matrix[rc->row_2 - 1][rc->col_2].car_colour = (ChoosenCarNum);

        if (size == 2) {
            g1->matrix[rc->row_1][rc->col_1].car_colour = DOT;
        }
        else if (size == 3) {
            g1->matrix[rc->row_3 - 1][rc->col_3].car_colour = (ChoosenCarNum);
            g1->matrix[rc->row_1][rc->col_1].car_colour = DOT;
        }
    }
    /* Move Down */
    else if (direction == 2) {
        g1->matrix[rc->row_1 + 1][rc->col_1].car_colour = (ChoosenCarNum);
        g1->matrix[rc->row_2 + 1][rc->col_2].car_colour = (ChoosenCarNum);

        if (size == 2) {
            g1->matrix[rc->row_2][rc->col_2].car_colour = DOT;
        }
        else if (size == 3) {
            g1->matrix[rc->row_3 + 1][rc->col_3].car_colour = (ChoosenCarNum);
            g1->matrix[rc->row_3][rc->col_3].car_colour = DOT;
        }
    }
}
