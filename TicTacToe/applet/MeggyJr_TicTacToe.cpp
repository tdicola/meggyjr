/*

 This library is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this library.  If not, see <http://www.gnu.org/licenses/>.
 	  
 */

// TODO:
// - Add board size constant and refactor methods to bounds check

#include <MeggyJrSimple.h>

// Variable declarations
#include "WProgram.h"
void setup();
void loop();
void ResetGameState();
byte CheckGameWinner();
void DrawTile(int x, int y, byte color);
void DrawBoard();
byte boardState[3][3];

byte cursorX;
byte cursorY;
byte cursorColor;
boolean drawCursor;
int cursorBlinkFrequency;
unsigned long cursorTime;

byte currentPlayer;

unsigned long time;


void setup()
{
  MeggyJrSimpleSetup();

  // Prepare for any debugging  
  Serial.begin(9600);
  
  ResetGameState();
  
  CheckButtonsPress();
  
  time = millis();
}

void loop()
{
  unsigned long newTime = millis();
  unsigned long frameTime = newTime - time;
  time = newTime;
  
  if (CheckGameWinner() > 0)
  {
    ResetGameState(); 
  }
  
  // Blink cursor
  cursorTime += frameTime;
  if (cursorTime >= cursorBlinkFrequency)
  {
    cursorTime = 0;
    drawCursor = !drawCursor;
  }
  
  // Handle movements
  CheckButtonsPress();
  if (Button_Up)
  {
    cursorY = constrain(cursorY + 1, 0, 2);
    drawCursor = true;
  }
  if (Button_Down)
  {
    cursorY = constrain(cursorY - 1, 0, 2);
    drawCursor = true;
  }
  if (Button_Left)
  {
    cursorX = constrain(cursorX - 1, 0, 2);
    drawCursor = true;
  }
  if (Button_Right)
  {
    cursorX = constrain(cursorX + 1, 0, 2);
    drawCursor = true;
  }
  if (Button_A || Button_B)
  {
    if (boardState[cursorX][cursorY] == 0)
    {
      boardState[cursorX][cursorY] = currentPlayer;
      
      if (currentPlayer == 1)
      {
        currentPlayer = 2;
        cursorColor = DimBlue;
        drawCursor = false;
      }
      else if (currentPlayer == 2)
      { 
        currentPlayer = 1;
        cursorColor = DimRed;
        drawCursor = false;
      }
    }
  }
  
  ClearSlate();
  
  DrawBoard();
  
  // Draw cursor
  if (drawCursor)
  {
    DrawTile(cursorX, cursorY, cursorColor);
  }
  
  DisplaySlate();
}

void ResetGameState()
{
  for (int i = 0; i < 3; ++i)
  {
     for (int j = 0; j < 3; ++j)
     {
       boardState[i][j] = 0; 
     }
  }
  
  currentPlayer = 1;
  
  cursorX = 1;
  cursorY = 1;
  cursorColor = DimRed;
  cursorBlinkFrequency = 400;
  cursorTime = 0;  
}

byte CheckGameWinner()
{
   bool hasFreeSpace = false;

   // Check for free spaces
   for (int i = 0; !hasFreeSpace && i < 3; ++i)
   {
     for (int j = 0; !hasFreeSpace && j < 3; ++j)
     {
       if (boardState[i][j] == 0)
       {
         hasFreeSpace = true;
       }
     }
   }
   
   for (int i = 0; i < 3; ++i)
   {
      // Check rows
      if ((boardState[0][i] == boardState[1][i]) && (boardState[1][i] == boardState[2][i]) && boardState[0][i] > 0)
      {
        return boardState[0][i];
      } 
     
      // Check columns
      if ((boardState[i][0] == boardState[i][1]) && (boardState[i][1] == boardState[i][2]) && boardState[i][0] > 0)
      {
        return boardState[i][0];
      }     
   }
   
   // Check diagonals
   if ((boardState[0][0] == boardState[1][1]) && (boardState[1][1] == boardState[2][2]) && boardState[0][0] > 0)
   {
     return boardState[0][0];
   } 
   if ((boardState[0][2] == boardState[1][1]) && (boardState[1][1] == boardState[2][0]) && boardState[0][2] > 0)
   {
     return boardState[0][2];
   } 

   // Check for draw   
   if (!hasFreeSpace)
   {
     return 3;
   }
   
   return 0;
}

void DrawTile(int x, int y, byte color)
{
   if (x < 3 && x >= 0 && y < 3 && y >= 0)
   {
     DrawPx(2 * x + x, 2 * y + y, color);
     DrawPx(2 * x + x + 1, 2 * y + y, color);
     DrawPx(2 * x + x + 1, 2 * y + y + 1, color);
     DrawPx(2 * x + x, 2 * y + y + 1, color);
   }
}

void DrawBoard()
{
  // Draw white lines
  for (int i = 0; i < 8; ++i)
  {
    DrawPx(2, i, White);
    DrawPx(5, i, White);
    DrawPx(i, 2, White);
    DrawPx(i, 5, White);
  } 
  
  // Draw marked spots
  for (int i = 0; i < 3; ++i)
  {
     for (int j = 0; j < 3; ++j)
     {
       switch (boardState[i][j])
       {
         case 1: DrawTile(i, j, Red);
                 break;
         case 2: DrawTile(i, j, Blue);
                 break;
       }
     }
  }
}



int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

