/*****************************************************************************
 *                               extract.h
 *  
 * Authors: Catherine MacBryde, Alana Sendlakowski
 * Date: 31 January 2022
 * Assignment: Homework 1: filesofpix
 *
 * Summary: To define the functions implemented in extract.c
 *
 ****************************************************************************/

#ifndef EXTRACT_H_INCLUDED
#define EXTRACT_H_INCLUDED

#include <stdio.h>
#include <list.h>
#include <table.h>

List_T fill_table(Table_T table, List_T list_orig, FILE *fp);
int parse_line(char **full_line, char **non_digit, char **digit, int bytes);
List_T free_list(List_T list, int width);
void free_table(const void *key, void **value, void *cl);
int is_digit(char c);

#endif