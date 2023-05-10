/*****************************************************************************
 *                               reconstruct.c
 *  
 * Authors: Catherine MacBryde, Alana Sendlakowski
 * Date: 31 January 2022
 * Assignment: Homework 1: filesofpix
 *
 * Summary: Utilizes a list from Hanson's data structures to manipulate
 *          a list of the lines from the original image to print
 *          a raw PGM containing those lines to stdout. The functions
 *          use the list produced by extract.h and are called in 
 *          restoration.c.
 *
 ****************************************************************************/

#include <reconstruct.h>
#include <readaline.h>
#include <extract.h>
#include <stdlib.h>
#include <stdio.h>
#include <pnmrdr.h>
#include <assert.h>
#include <list.h>

/*****************************************************************************
* Name: get_pixels
* Summary: To convert a string composed of digits and spaces into integer
*          digits that represent pixel values
* Input:         char *orig_line: pointer to array of chars containing the 
*                                 digits of an original image line
*                int width: integer to represent number of digits in an
*                           image line
* Returns: none
*    Expects: pointer to char array orig_line to contain at least two numbers
*             with a space between
* Notes: Calls print_raw_pgm function to print once converted to int
*        Will CRE if orig_line string is null
******************************************************************************/
void get_pixels(char *orig_line, int width)
{
        assert(orig_line != NULL);
        char digit[4];
        /*sets all elements of digit array to null character*/
        memset(digit, 0, 4);
        int digit_index = 0, orig_index = 0, curr_width = 0;
        
        while(curr_width < width) {
                if (is_digit(orig_line[orig_index])) {
                        digit[digit_index] = orig_line[orig_index];
                        orig_index++;
                        digit_index++;
                } else {
                        int digit_as_int = string_to_int(digit, digit_index);
                        print_raw_pgm(digit_as_int);
                        memset(digit, 0, 4);
                        
                        digit_index = 0;
                        orig_index++;
                        curr_width++;
                }
        }
}

/*****************************************************************************
* Name: print_header
* Summary: To output the header contents of the raw PGM to stdout
* Input: 
*           int height: the number of lines in the original image
*           int width: the number of digits in a given line
* Returns: none
*    Expects: none
* Notes: 255 is always the max val, P5 is always the magic number
*
******************************************************************************/
void print_header(int height, int width) 
{
        printf("P5\n%u %u\n%d\n", width, height, 255);
}

/*****************************************************************************
* Name: print_raw_pgm
* Summary: to print the binary version of a digit to stdout
* Input:            int digit: the integer value of a given pixel
* Returns: none
*    Expects: an integer in the range [0, 255]
* Notes: none
*
******************************************************************************/
void print_raw_pgm(int digit) 
{
        printf("%c", digit);
}

/*****************************************************************************
* Name: get_width
* Summary: To calculate the number of digits in an original image line
* Input:        List_T list_orig: a list containing lines from original image
* Returns: An integer representing the width of the image
*    Expects: non-empty list
* Notes: Uses the first line in the list, but does not discard any elements
*        Will CRE if list is null
******************************************************************************/
int get_width(List_T list_orig)
{
        assert(list_orig != NULL);
        /*accesses member variable in list struct to get the first element*/
        void *first_line = list_orig->first;
        char *line = (char *)first_line;
        int index = 0;
        int width = 0;

        while (is_digit((line)[index]) || (line)[index] == ' ') {
                if ((line)[index] == ' ') {
                        width++;
                }
                index++;
        }
        return width;
}

/*****************************************************************************
* Name: string_to_int
* Summary: converts a pointer to an array of integer characters into a
*          integer representation of those characters
* Input:        char *string: An array of chars containing digits as a string
*                   int size: the length of the string variable
* Returns: An integer representing a given pixel value
*       Expects: the array of characters to be integer characters only
* Notes: Will CRE if string is null
*
******************************************************************************/
int string_to_int(char *string, int size) 
{
        assert(string != NULL);
	int i = 0;

	for (int index = 0; index < size; index++) {
		assert(is_digit(string[index]));
                i = (i * 10) + string[index] - '0';
	}
        return i;
}