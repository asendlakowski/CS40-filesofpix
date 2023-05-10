/*****************************************************************************
 *                               reconstruct.h
 *  
 * Authors: Catherine MacBryde, Alana Sendlakowski
 * Date: 31 January 2022
 * Assignment: Homework 1: filesofpix
 *
 * Summary: To define the functions implemented in reconstruct.c
 *
 ****************************************************************************/

#ifndef RECONSTRUCT_H_INCLUDED
#define RECONSTRUCT_H_INCLUDED

#include <stdio.h>
#include <list.h>

void get_pixels(char *orig_line, int width);
void print_header(int height, int width);
void print_raw_pgm(int digit);
int get_width(List_T list_orig);
int string_to_int(char *string, int size);

#endif