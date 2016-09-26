/**
 * fifteen.c
 *
 * Made by - Osaid Rehman Nasir
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 */
 
#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(int d);
void draw(int d);
bool move(int tile , int d);
bool won(int d);

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init(d);

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw(d);

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won(d))
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile ;
        scanf("%i",&tile);
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile,d))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(int d)
{
    int k = (d*d)-1 , n = 0;
    if(d%2!=0) {
    for(int i=0;i<d;i++)
    {
        for(int j=0;j<d;j++)
        {
            if(i==d-1&&j==d-1) { board[i][j]=0;   break;}
            board[i][j] = k-n;
            n++;
        }
    }
  }
    else
    {
        for(int i=0;i<d;i++)
        {
            for(int j=0;j<d;j++)
            {
                if(i==d-1)
                {
                    if(j==d-1)
                     {board[i][j]=0; break;}
                    else if(j==d-3)
                    {
                    board[i][j]=1;
                    board[i][j+1]=2;
                    board[i][d-1]=0;
                    break;
                    }
                }    
               board[i][j] = k-n;
                n++;
                
            }
        }
    }
}

/**
 * Prints the board in its current state.
 */
void draw(int d)
{
   for(int i=0;i<d;i++)
   {
       for(int j=0;j<d;j++)
       {
           if(board[i][j]==0)
           {
               printf("_  ");
           }
           else
           {
               printf("%i",board[i][j]);
               if(board[i][j]>=10)
                 printf(" ");
               else
                 printf("  ");
           }
       }
       printf("\n");
   }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile , int d)
{
    int temp;
    for(int i=0; i < d ; i++)
    {
        for(int j=0; j<d ;j++)
        {
            if(board[i][j]==tile)
            {
                
                if(i!=0&&board[i-1][j]==0)
                {
                    temp=board[i][j];
                    board[i][j] = board[i-1][j];
                    board[i-1][j] = temp;
                    return true;
                }
                else if(j!=0&&board[i][j-1]==0)
                {
                    temp=board[i][j];
                    board[i][j] = board[i][j-1];
                    board[i][j-1] = temp;
                    return true;
                }
                else if(i!=d-1&&board[i+1][j]==0)
                {
                    temp=board[i][j];
                    board[i][j] = board[i+1][j];
                    board[i+1][j] = temp;
                    return true;
                }
                    
                else if(j!=d-1&&board[i][j+1]==0)
                {
                    temp=board[i][j];
                    board[i][j] = board[i][j+1];
                    board[i][j+1] = temp;
                    return true;
                }
            }
        }
    }
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(int d)
{
    int count=0 , chkr=1;
    for(int i=0;i<d;i++)
    {
        for(int j=0;j<d;j++)
        {
            if(board[i][j]==chkr)
            {
                count++;
            }
            chkr++;
        }
    }
    if(count==(d*d)-1)  return true;
    return false;
}
