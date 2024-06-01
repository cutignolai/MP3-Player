// /***************************************************************************//**
//   @file     lcd.c
//   @brief    LCD from SDK in NXP web
//   @author   Micho
//   @cite		https://community.nxp.com/t5/Kinetis-Software-Development-Kit/Driving-16x2-LCD-using-KSDK-drivers/ta-p/1098903
//  ******************************************************************************/

// /*******************************************************************************
//  * INCLUDE HEADER FILES
//  ******************************************************************************/

// #include <stdbool.h>
// #include <stddef.h>
// #include <stdio.h>
// #include <math.h>
// #include <stdlib.h>
// #include <stdint.h>

// #include "MK64F12.h"
// #include "hardware.h"

// #include "board.h"
// #include "gpio.h"
// #include "lcd.h"

// /*******************************************************************************
//  * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
//  ******************************************************************************/
// //CAMBIAR PINES
// #define LCD_ENABLE_EN	DIO_80
// #define LCD_RS_EN		DIO_81
// #define LCD_D7_EN		DIO_82
// #define LCD_D6_EN		DIO_83
// #define LCD_D5_EN		DIO_84
// #define LCD_D4_EN		DIO_85


// #define LCD_D7_ON		gpioWrite(LCD_D7_EN, HIGH)
// #define LCD_D7_OFF		gpioWrite(LCD_D7_EN, LOW)
// #define LCD_D6_ON		gpioWrite(LCD_D6_EN, HIGH)
// #define LCD_D6_OFF		gpioWrite(LCD_D6_EN, LOW)
// #define LCD_D5_ON		gpioWrite(LCD_D5_EN, HIGH)
// #define LCD_D5_OFF		gpioWrite(LCD_D5_EN, LOW)
// #define LCD_D4_ON		gpioWrite(LCD_D4_EN, HIGH)
// #define LCD_D4_OFF		gpioWrite(LCD_D4_EN, LOW)

// #define LCD_ENABLE_ON	gpioWrite(LCD_ENABLE_EN, HIGH)
// #define LCD_ENABLE_OFF	gpioWrite(LCD_ENABLE_EN, LOW)
// #define LCD_RS_ON		gpioWrite(LCD_RS_EN, HIGH)
// #define LCD_RS_OFF		gpioWrite(LCD_RS_EN, LOW)

// #define INIT_INSTRUCTIONS 8

// /*******************************************************************************
//  * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
//  ******************************************************************************/

// const unsigned char  upper_line[] = "TP FINAL:";
// const unsigned char  lower_line[] = "MP3 PLAYER";

// const unsigned char  initLCD[INIT_INSTRUCTIONS]={0x02, 0x28, 0x0C,0x06,0x01,0x00};

// /*******************************************************************************
//  *         FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
//  ******************************************************************************/

// void enable_ten_sec(void);
// void enable_half_sec(void);
// void SetUp(void);
// void instruction(unsigned char x);
// void longInstruction(unsigned char x);
// void lcd_data(unsigned char x);
// void text (unsigned char *b);
// void info(unsigned char x);
// void LCD_Pin_Enable(void);
// void delay(unsigned int time);

// /*******************************************************************************
//  *******************************************************************************
//                         GLOBAL FUNCTION DEFINITIONS
//  *******************************************************************************
//  ******************************************************************************/

// void lcd_init(void){

//     LCD_Pin_Enable(); 	// Enable pins

//     //pit_Init();    		//PIT Module Initialization

// 	delay(2000); 			//Display initialization

// 	SetUp();

// 	instruction(0x80); //First line
// 	text((unsigned char *)&upper_line[0]);

// 	instruction(0xC0); //Second line
// 	text((unsigned char *)&lower_line[0]);

// 	//instruction(0x0F); //Cursor on blinking
// }

// void lcd_clear(void){
//     instructionLong(0x01); 		//Clear display
// }

// void lcd_write_upper_line(unsigned char* words){
//     instruction(0x80); 			//Write in upper line
// 	text(words);
// }

// void lcd_write_lower_line(unsigned char* words){
//     instruction(0xC0); 			//Write in lower line
// 	text(words);
// }

// void lcd_Shift_Right(void){
//     instruction(0x1C); 			//Lcd corrimiento hacia derecha
// }

// void lcd_Shift_Left(void){
//     instruction(0x18); 			//Lcd corrimiento hacia izquierda
// }

// /*******************************************************************************
//  *******************************************************************************
//                         LOCAL FUNCTION DEFINITIONS
//  *******************************************************************************
//  ******************************************************************************/

// void enable_ten_sec(void){
// 	LCD_ENABLE_ON;
// 	delay(10000);
// 	LCD_ENABLE_OFF;
// }

// void enable_half_sec(void){
// 	LCD_ENABLE_ON;
// 	delay(500);
// 	LCD_ENABLE_OFF;
// }

// //-----------------------------------------------------------------------

// void SetUp(void){
// 	unsigned char a = 0;

// 	while(initLCD[a])
// 		{
// 			longInstruction(initLCD[a]);
// 			a++;
// 		}
// }

// //-----------------------------------------------------------------------

// void instruction(unsigned char x){
// 		LCD_RS_OFF;

// 		lcd_data(x&0xF0);
// 		enable_ten_sec();

// 		lcd_data((x<<4)&0xF0);
// 		enable_ten_sec();
// }

// //-----------------------------------------------------------------------

// void longInstruction(unsigned char x){
// 		LCD_RS_OFF;

// 		lcd_data(x&0xF0);
// 		enable_half_sec();

// 		lcd_data((x<<4)&0xF0);
// 		enable_ten_sec();
// }

// //-----------------------------------------------------------------------

// void lcd_data(unsigned char x)
// {
// 	//Bit 7
// 	if (x&0x80)
// 	{
// 	  LCD_D7_ON;
// 	}
// 	else
// 	{
// 	  LCD_D7_OFF;
// 	}

// 	//Bit 6
// 	if (x&0x40)
// 	{
// 	  LCD_D6_ON;
// 	}
// 	else
// 	{
// 	  LCD_D6_OFF;
// 	}

// 	//Bit 5
// 	if (x&0x20)
// 	{
// 	  LCD_D5_ON;
// 	}
// 	else
// 	{
// 	  LCD_D5_OFF;
// 	}

// 	//Bit 4
// 	if (x&0x10)
// 	{
// 	  LCD_D4_ON;
// 	}
// 	else
// 	{
// 	  LCD_D4_OFF;
// 	}
// }

// //-----------------------------------------------------------------------

// void text (unsigned char *b){
// 	while(*b)
// 	{
// 		info(*b);
// 		b++;
// 	}
// }

// //-----------------------------------------------------------------------

// void info(unsigned char x)
// {
// 	//if (x == '�') x = 238;

// 	LCD_RS_ON;

// 	lcd_data( x&0xF0 );
// 	enable_half_sec();

// 	lcd_data( (x<<4)&0xF0 );
// 	enable_half_sec();
// }


// //-----------------------------------------------------------------------

// void LCD_Pin_Enable(void){

// 	gpioMode(LCD_ENABLE_EN, OUTPUT);
// 	gpioMode(LCD_RS_EN, OUTPUT);
// 	gpioMode(LCD_D7_EN, OUTPUT);
// 	gpioMode(LCD_D6_EN, OUTPUT);
// 	gpioMode(LCD_D5_EN, OUTPUT);
// 	gpioMode(LCD_D4_EN, OUTPUT);
	
// 	LCD_ENABLE_OFF;
// 	LCD_RS_OFF;
// 	LCD_D7_OFF;
// 	LCD_D6_OFF;
// 	LCD_D5_ON;
// 	LCD_D4_ON;
// }

// //-----------------------------------------------------------------------

// void delay(unsigned int time){
// 	volatile int i;
// 	for(i = 0; i < time; i++);
// }

// //-----------------------------------------------------------------------






