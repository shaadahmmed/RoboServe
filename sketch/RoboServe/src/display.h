#ifndef DISPLAY_H
#define DISPLAY_H

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
byte currentRow = 0;

void displayClearRow(int row);

void beginDisplay()
{
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    currentRow = 0;
}

void displaySetCursor(int col, int row)
{
    lcd.setCursor(col, row);
    currentRow = row;
}

void displayPrint(String str)
{
    lcd.print(str);
}

void displayPrint(String str, int row)
{
    int size = str.length();
    currentRow = row;

    if (size > 20)
    {
        for (int i = 0; i <= size - 20; i++)
        {
            displaySetCursor(0, row);
            displayClearRow(row);
            displayPrint(str.substring(i, i + 20));
            delay(300);
        }
        return;
    }
    displaySetCursor(0, row);
    displayPrint(str);
}

void displayPrintCenter(String str, int row)
{
    int strLength = str.length();
    int spaces = (20 - strLength) / 2;
    displaySetCursor(spaces, row);
    displayPrint(str);
    currentRow = row;
}

void displayClearRow(int row)
{
    lcd.setCursor(0, row);
    lcd.print("                    ");
}

void displayOverrideRow(String str, int row)
{
    currentRow = row;
    displayClearRow(row);
    displayPrint(str, row);
}

void displayClear()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    currentRow = 0;
}

#endif