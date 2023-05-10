/*****************************************************************************
 *                               extract.c
 *  
 * Authors: Catherine MacBryde, Alana Sendlakowski
 * Date: 31 January 2022
 * Assignment: Homework 1: filesofpix
 *
 * Summary: Utilizes a Hanson table and Hanson list to determine the original
 *          lines of a corrupted image file by extracting data which is then
 *          used in reconstruct.h and restoration.h 
 *
 ****************************************************************************/

#include <readaline.h>
#include <extract.h>
#include <reconstruct.h>
#include <stdlib.h>
#include <stdio.h>
#include <pnmrdr.h>
#include <assert.h>
#include <atom.h>
#include <table.h>
#include <list.h>

/*****************************************************************************
* Name: fill_table
* Summary: 
* Input:       Table_T table: An empty table passed in, used to hash key-value
*                              pairs to identify the original image lines
*           List_T list_orig: An empty list passed in, used to store the 
*                                 digit lines from the original image file
*                   FILE *fp: A pointer to the file stream where each line is
*                             read
*
* Returns: A list containing only the lines from the original image
*    Expects: readaline to retrieve at least two lines and two original lines
*             to be present; table and list should both contain no elements
*             when passed in to function
* Notes: Will CRE if fp is null
*        Will CRE if table is null
*        Will CRE if list_orig is not null at the start of function
*        Will CRE if the first character is an EOF character
*        Will CRE if list_orig is null at the end of function
*
******************************************************************************/
List_T fill_table(Table_T table, List_T list_orig, FILE *fp)
{
        assert(fp != NULL);
        assert(table != NULL);
        assert(list_orig == NULL);
        char *line = "", *n_d = "", *d = "";
        
        /*first line of the file read in and stored in line*/
        int first_char = fgetc(fp);
        assert(first_char != EOF);
        ungetc(first_char, fp);

        int bytes = readaline(fp, &line);
        const char *address_orig = "";

    	while (bytes != 0) {
                int nd = parse_line(&line, &n_d, &d, bytes);
                const char *const_non_digit = n_d;
                const char *line_address = Atom_new(const_non_digit, nd);
                void *prev_val = Table_put(table, line_address, d);
        
                /*checks if there is already a value at the key in the table*/
                if (prev_val != NULL) {
                        list_orig = List_push(list_orig, prev_val);
                        address_orig = line_address;
                } 
                free(line);
                free(n_d);
                bytes = readaline(fp, &line);
    	}
        /*retrieves the final line from the table*/
        list_orig = List_push(list_orig, Table_remove(table, address_orig));
        assert(list_orig != NULL);
        return list_orig;
}

/*****************************************************************************
* Name: parse_line
* Summary: To parse through a line and separate the characters that are 
*          represented as digits from those that are non-digit
* Input:        char **full_line: pointer to an array of characters that
*                                 contains the full line to parse through
*               char **non_digit: pointer to an array of characters that is
*                                 used to hold the non_digit characters from
*                                 full_line
*               char **digit:     pointer to an array of characters that is
*                                 used to hold the digit characters from
*                                 full_line
*                int bytes: the length/number of bytes of the full line
* Returns: integer which is the number of bytes in the non-digit char array
*    Expects: non_digit and digit pointer to a char array assumed to be 
*             empty (any data will be lost if not)
* Notes: Will CRE if d or nd memory allocation fails
*        Will CRE if full_line, non_digit, or digit is null
******************************************************************************/
int parse_line(char **full_line, char **non_digit, char **digit, int bytes) 
{
        assert(full_line != NULL);
        assert(non_digit != NULL);
        assert(digit != NULL);
        /*no assert needed for non_digit and digit because ok to be null*/
        char *non_d = (char *) calloc(sizeof(char), bytes + 1);
        char *d = (char *) calloc(sizeof(char), bytes + 1);
        assert(non_d != NULL);
        assert(d != NULL);
    	int non_d_index = 0, d_index = 0, full_index = 0;

        /*indexes full_line and seperates the non-digit and digit bytes*/
    	while (full_index < bytes && bytes != 0) {
                if (is_digit((*full_line)[full_index])) {
                        d[d_index] = (*full_line)[full_index];
                        d_index++;
                        full_index++;
                        /*Checks if char at full_index is a digit*/
                        while (full_index < bytes &&
                               is_digit((*full_line)[full_index])) {
                                
                                d[d_index] = (*full_line)[full_index];
                                d_index++;
                                full_index++;
                        }
                        /*adds space between each pixel value*/
                        d[d_index] = ' ';
                        d_index++;
                } else {
                        non_d[non_d_index] = (*full_line)[full_index];
                        non_d_index++;
                        full_index++;
                }
        }
	*digit = d;
	*non_digit = non_d;
        return non_d_index;
}

/*****************************************************************************
* Name: free_list
* Summary: To iterate through the list of original image lines and deallocate
*          its elements
* Input:        List_T list: the list of original image lines
*               int width: number of pixels in each line
* Returns: the updated, newly empty list
*    Expects: The list to contain at least 2 elements
* Notes: Will CRE if list is null when enters function
*        Will CRE if list is not null before return
******************************************************************************/
List_T free_list(List_T list, int width) 
{
        assert(list != NULL);
        int length = List_length(list);

        while (length != 0) {
                char *temp = "";
                /*top element in list removed from list and placed in temp*/
                list = List_pop(list, (void *)&temp);
                get_pixels(temp, width);
                free(temp);
                length--;
        }
        assert(list == NULL);
        return list;
}

/*****************************************************************************
* Name: free_table
* Summary: Deallocate one element of the table
* Input:        const void *key: pointer to key in the table
*               void **value: pointer to the value in the table
*               void *cl: void pointer used to specify closure
* Returns: none
*    Expects: none
* Notes: used in Table_map to deallocate every element in the table
*        Will CRE if key or value is null
******************************************************************************/
void free_table(const void *key, void **value, void *cl) 
{
        assert(key != NULL);
        assert(value != NULL);
        free(*value);
        (void) cl;
}

/*****************************************************************************
* Name: is_digit
* Summary: To determine whether a character represents a digit or non-digit
* Input:          char c: any character 
* Returns: An integer; 0 if false and 1 if true
*    Expects: valid character
* Notes: none
*
******************************************************************************/
int is_digit(char c)
{
        return (c >= '0' && c <= '9');
}