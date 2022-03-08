/*
 * Progress Bar module for C
 * Allows the creation and usage of simple progress bars for generic C codes
 * The updation of progress is requred to be done manually, thereby allowing
 * usage across all sorts of looping constructs
 * Usage:
 *      Before the start of the loop, create a ProgressBar object and set it up 
 *      with the total value as the number of times the loop is iterated.
 *      Update the ProgressBar at the start of the loop.
 *      Clear the ProgressBar once done with the loop.
 *      Now, the ProgressBar object is also reset.
 * 
 * Example:
 *      int total = 100;
 *      ProgressBar pb;
 *      PB_Init(total, &pb);
 *      for (int i = 0; i < total; i++) {
 *          PB_Update(i + 1, &pb);
 *          sleep(1);
 *          printf("Iteration %d finished\n", i + 1);
 *      }
 *      PB_Clear(&pb);
 * 
 * Written By: Shivansh Bakshi
 */

#include<sys/ioctl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#define COLUMN_MARGIN 5
#define PREFIX_STRING_SIZE 16
#define PREFIX_STRING "Progress: [%3d%%]%.*s%*c"

typedef struct ProgressBar {
    float total;
    float curr;
    // Terminal Dimensions
    unsigned short rows;
    unsigned short columns;
    // ProgressBar stirng parameters
    //          The string itself: Length = 16 + len (15 from "Progress: [xxx%]")
    //          How many block characters it can have
    //          The block string to fill the progress bar
    int blockLen;
    char* blockString;
    char* progressString;
}ProgressBar;

void PB_Init(int total, ProgressBar* pb)
{
    ProgressBar* pbar = (ProgressBar*)malloc(sizeof(ProgressBar));
    pbar->total = total;
    pbar->curr = 0;
    // Get the terminal size
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    pbar->rows = w.ws_row;
    pbar->columns = w.ws_col;
    // Set blockLength as columns - ( PREFIX_STRING_SIZE + COLUMN_MARGIN )
    int blockLen = pbar->columns - (PREFIX_STRING_SIZE + COLUMN_MARGIN);
    pbar->blockLen = blockLen;
    pbar->blockString = (char*)calloc(blockLen, sizeof(char));
    char bl = '#';
    // Fill blockString with # character
    for (int i = 0; i < blockLen; i++) {
        strncat(pbar->blockString, &bl, 1);
    }
    pbar->progressString = (char*)calloc(blockLen + PREFIX_STRING_SIZE + 1, sizeof(char));
    memcpy(pb, pbar, sizeof(ProgressBar));

    // Ensure that the last line is available
    printf("\n\0337\033[0;%dr\0338\033[1A", pbar->rows - 1);
    fflush(stdout);
    // Print Breakdown
    // printf("\n"):            Go to next line
    // printf("\0337"):         Save Cursor Position
    // printf("\033[0;%dr"):    Set top and bottom margins
    // printf("\0338"):         Restore Cursor Position
    // printf("\033[1A"):       Cursor Up
}

void PB_Update(float val, ProgressBar* pb)
{
    pb->curr = val;
    float percent = (pb->curr)/(pb->total);
    int width = (int) (percent * pb->blockLen);
    sprintf(pb->progressString, PREFIX_STRING, (int) (100*percent), width, pb->blockString, pb->blockLen - width, '|');
    printf("\0337\033[%d;0f%s\0338", pb->rows, pb->progressString);  
    fflush(stdout);
    // Print Breakdown
    // printf("\0337"):         Save the cursor position
    // printf("\033[%d;0f"):    Move cursor to last row
    // printf("%s"):            Draw the progressbar
    // printf("\0338"):         Restore the cursor
}

void PB_Clear(ProgressBar* pb)
{
    printf("\0337\033[0;%dr\033[%d;0f\033[0K\0338", pb->rows, pb->rows);
    fflush(stdout);
    // Print Breakdown
    // printf("\0337"):         Save cursor position
    // printf("\033[0;%dr"):    Drop Margin reservations
    // printf("\033[%d;0f"):    Go to last line
    // printf("\033[0K"):       Clean the line
    // printf("\0338"):         Restore the cursor
    pb->total = 0;
    pb->curr = 0;
    pb->rows = 0;
    pb->columns = 0;
    pb->blockLen = 0;
    strcpy(pb->blockString, "");
    strcpy(pb->progressString, "");
}
