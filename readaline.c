 /*****************************************************************************
 *                               readaline.c
 *  
 * Authors: Catherine MacBryde, Alana Sendlakowski
 * Date: 31 January 2022
 * Assignment: Homework 1: filesofpix
 *
 * Summary: Contains function to read a line from a given input file
 *          which is used in extract.h.
 *
 ****************************************************************************/

#include <readaline.h>
#include <stdlib.h>
#include <stdio.h>
#include <pnmrdr.h>
#include <assert.h>

/*****************************************************************************
* Name: readaline
* Summary: Reads in a line from a file stream by reading individual characters
*          until a newline or EOF character is identified
* Input:       FILE *inputfd: A pointer to a file stream
*              char **datapp: A char double pointer which is set to the 
*                             address of the first byte of the line
* Returns: the number of bytes in the line that was read
*    Expects: A newline character at the end of each line in the file
* Notes: Returns bytes but also sets datapp to address of first byte 
*        of the line
*        Will CRE if get_line memory allocation fails
*        Will CRE if either inputs are null
*
******************************************************************************/
size_t readaline (FILE *inputfd, char **datapp) 
{
        assert(inputfd != NULL); 
        assert(datapp != NULL);

        int curr = fgetc(inputfd); /*gets first char in line from file*/
        assert(!(ferror(inputfd)));
        unsigned int size = 1000;
        
        /*if readaline is called an there are no more lines to be read*/
        if (curr == EOF) { 
                *datapp = NULL;
                return 0;
        }
        char *get_line = (char*) malloc(sizeof(char) * size);
        assert(get_line != NULL);
        
        get_line[0] = curr; 
        unsigned int bytes = 1;

        while (curr != EOF) { 
                if (bytes == size) {
                        size = size * 2;
                        get_line = realloc(get_line, size);
                        assert(get_line != NULL);
                }
                if (curr == '\n') {
                        *datapp = get_line;
                        return bytes;
                }
                curr = fgetc(inputfd);
                assert(!(ferror(inputfd)));
                get_line[bytes] = curr;
                bytes++;
        }
        return bytes;
}