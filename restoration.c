/*****************************************************************************
 *                               restoration.c
 *  
 * Authors: Catherine MacBryde, Alana Sendlakowski
 * Date: 31 January 2022
 * Assignment: Homework 1: filesofpix
 *
 * Summary: To organize and utilize functions defined in extract.h and 
 *          reconstruct.h to restore a corrupted image inputed from 
 *          a file or stdin
 *
 ****************************************************************************/

#include <readaline.h>
#include <stdlib.h>
#include <stdio.h>
#include <pnmrdr.h>
#include <assert.h>
#include <atom.h>
#include <table.h>
#include <list.h>
#include <extract.h>
#include <pnmrdr.h>
#include <reconstruct.h>

/*defines an estimated number for the entries in our created table */
#define HINT 100

/*****************************************************************************
* Name: main
* Summary: To call functions in extract.h and reconstruct.h in order to
*          restore a corrupted image file 
* Input:        int argc: the number of arguments provided in the command line
*           char *argv[]: an array of chars containing the arguments from the
*                         command line
* Returns: an integer 0 after completing successfully 
*    Expects: none
* Notes: 
*       Will CRE if more than 2 arguments are supplied in command line
*       Will CRE if file cannot be opened
*
******************************************************************************/
int main(int argc, char *argv[]) 
{  
        assert(argc <= 2);
        FILE *fp;
        if (argc == 2) {
                char *filename = argv[1];
                fp = fopen(filename, "rb");
                assert(fp != NULL);
        }
        
        Table_T table = Table_new(HINT, NULL, NULL);
        List_T list_orig = List_list(NULL);

        if (argc == 2) {
                list_orig = fill_table(table, list_orig, fp);
        } else {
                list_orig = fill_table(table, list_orig, stdin);
        }
        
        list_orig = List_reverse(list_orig);
        
        int height = List_length(list_orig);
        int width = get_width(list_orig);
        print_header(height, width);
        
        list_orig = free_list(list_orig, width);
        List_free(&list_orig);

        Table_map(table, free_table, NULL);
        Table_free(&table);

        if (argc == 2) {
                fclose(fp);
        }
        exit(EXIT_SUCCESS);
}