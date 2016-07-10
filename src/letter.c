#include <stdlib.h>

#include "logger.h"
#include "letter.h"

static char letters[] = {
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.'
};

static int texture_coords[] = {
  2, 114, 25, 152,    // 0
  155, 114, 175, 152, // 1
  29, 114, 54, 152,   // 2
  84, 114, 107, 152,  // 3
  393, 156, 419, 195, // 4
  57, 114, 80, 152,   // 5
  450, 156, 480, 195, // 6
  110, 114, 134, 152, // 7
  422, 156, 450, 195, // 8
  480, 156, 506, 195, // 9
  407, 56, 426, 78    // .
};


Letter* Letter_new(int width, int height)
{
  Letter *letter = malloc(sizeof(Letter));
  letter->sprite = Sprite_new("data/font.png", width, height);
  Sprite_create_frames(letter->sprite, 11, texture_coords);
  return letter;
}

void Letter_display(Letter *letter, char character)
{
  int letter_index;
  for (letter_index = 0; letter_index < sizeof(letters); letter_index++) {
    if (letters[letter_index] == character) {
      break;
    }
  }
  if (letter_index >= letter->sprite->frame_count) {
    Log("Letter error, unable to display %c", character);
    return;
  }
  Sprite_set_frame(letter->sprite, letter_index);
}

void Letter_set_position(Letter *letter, float x, float y)
{
  Sprite_set_position(letter->sprite, x, y);
}
