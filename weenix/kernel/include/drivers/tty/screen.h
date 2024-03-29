/******************************************************************************/
/* Important Spring 2017 CSCI 402 usage information:                          */
/*                                                                            */
/* This fils is part of CSCI 402 kernel programming assignments at USC.       */
/*         53616c7465645f5f2e8d450c0c5851acd538befe33744efca0f1c4f9fb5f       */
/*         3c8feabc561a99e53d4d21951738da923cd1c7bbd11b30a1afb11172f80b       */
/*         984b1acfbbf8fae6ea57e0583d2610a618379293cb1de8e1e9d07e6287e8       */
/*         de7e82f3d48866aa2009b599e92c852f7dbf7a6e573f1c7228ca34b9f368       */
/*         faaef0c0fcf294cb                                                   */
/* Please understand that you are NOT permitted to distribute or publically   */
/*         display a copy of this file (or ANY PART of it) for any reason.    */
/* If anyone (including your prospective employer) asks you to post the code, */
/*         you must inform them that you do NOT have permissions to do so.    */
/* You are also NOT permitted to remove or alter this comment block.          */
/* If this comment block is removed or altered in a submitted file, 20 points */
/*         will be deducted.                                                  */
/******************************************************************************/

#pragma once
#include "types.h"

/**
 * Initialize the screen subsystem.
 */
void screen_init(void);

/**
 * Move the cursor to the given (x,y) coords (measured from top left).
 *
 * @param x the x coordinate
 * @param y the y coordinate
 */
void screen_move_cursor(uint8_t x, uint8_t y);

/**
 * Writes a character to the screen at the given position.
 *
 * @param c the character to display on the screen
 * @param x the x coordinate
 * @param y the y coordinate
 */
void screen_putchar(char c, uint8_t x, uint8_t y);

/**
 * Writes a character to the screen at a given position with the given
 * attributes.
 *
 * @param c the character to display on the screen
 * @param x the x coordinate
 * @param y the y coordinate
 * @param the attributes for the character
 */
void screen_putchar_attrib(char c, uint8_t x, uint8_t y, uint8_t attrib);

/**
 * Write a buffer of characters which is _EXACTLY_ DISPLAY_WIDTH x
 * DISPLAY_HEIGHT characters.
 *
 * @param buf the buffer to write to the screen
 */
void screen_putbuf(const char *buf);

/**
 * Write a buffer of characters which is _EXACTLY_ DISPLAY_WIDTH x
 * DISPLAY_HEIGHT characters and attributes.
 *
 * @param buf the buffer to write to the screen
 */
void screen_putbuf_attrib(const uint16_t *buf);

/**
 * Clear the screen.
 */
void screen_clear(void);
