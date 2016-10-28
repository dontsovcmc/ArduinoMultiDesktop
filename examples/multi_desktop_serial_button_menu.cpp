#include "Arduino.h"
#include "stdlib.h"

#include <menu.h>
#include <menuPrint.h>
#include <menuFields.h>
#include <keyStream.h>
#include <chainStream.h>

#include <multidesktop.h>

#define LED_WORK 3
bool ledLight=false;

#define BUTTON_1 4   
#define BUTTON_2 5  

// set true - if you want to show menu
// set false - when exit menu
bool poll_menu = false;

void print(const char *buffer, int ms)
{
	Serial.println(buffer); delay(ms);
}

// desktop functions
void up() { print("up pressed", 1000);  }

void down() { print("down pressed", 1000); }

void led_button() 
{ 
	ledLight = !digitalRead(LED_WORK);
	digitalWrite(LED_WORK, ledLight); 
}

void menu_button() 
{
	print("MENU", 1000); 
	poll_menu = true;
}

void show_1_screen() 
{
	print("MAIN DESKTOP", 0);
	print("[ + ]  [ - ]   [ * D2]", 0);
}

void show_2_screen()
{
	digitalRead(LED_WORK) ? print("LED on", 0) : print("LED off", 0);
	print("[ + LED]   [ * D3]", 0);
}

void show_3_screen()
{
	print("LAST DESKTOP", 0);
	print("[ + MENU]   [ * D1]", 0);
}

//Multi Desktop configuration
#define BUTTONS 2
int8_t codes[] = { menu::upCode , menu::downCode };

func desktop1cb[] = { &up, &down };
func desktop2cb[] = { &led_button, 0 };
func desktop3cb[] = { &menu_button, 0 };

MultiDesktop<BUTTONS> multi_desktop( menu::enterCode , codes);
Desktop<BUTTONS> mainDesktop(&show_1_screen, desktop1cb); 
Desktop<BUTTONS> secondDesktop(&show_2_screen, desktop2cb);
Desktop<BUTTONS> thirdDesktop(&show_3_screen, desktop3cb);

void setup()
{
	pinMode(LED_WORK, OUTPUT);
	pinMode(BUTTON_1,INPUT);
	pinMode(BUTTON_2,INPUT); 
	
	digitalWrite(LED_WORK, LOW);
	
	Serial.begin(9600);
	while(!Serial);
	
	print("HELLO", 500);
	
	multi_desktop.add_desktop(&mainDesktop);
	multi_desktop.add_desktop(&secondDesktop);
	multi_desktop.add_desktop(&thirdDesktop);
	
	multi_desktop.show();
}

bool close_menu()
{
	poll_menu = false;
	multi_desktop.show(); 
	return true;
}

//Setup input
keyMap encBtn_map[]={{BUTTON_1,menu::upCode}, {BUTTON_2,menu::enterCode}};//negative pin numbers means we have a pull-up, this is on when low
keyLook<2> encButton(encBtn_map);
Stream* in2[]={&encButton,&Serial};
chainStream<2> allIn(in2);

//Setup menu
TOGGLE(ledLight,toggle_led,"LED: ",
  VALUE("ON",true),
  VALUE("OFF",false)
);

MENU(mainMenu,"Main"
  , SUBMENU(toggle_led)
  , OP("Exit", close_menu)
);

menuPrint menu_out(Serial); //describe output device

void loop() 
{	
	if (poll_menu)
	{
		mainMenu.poll(menu_out, allIn);
	}
	else if (allIn.available()) 
	{
        char ch = allIn.read();
		
		multi_desktop.button_pressed(ch);
		
		if (ch != -1)
			multi_desktop.show(); 
    }
	
	digitalWrite(LED_WORK, ledLight);
	
	delay(1); // delay in between reads for stability
}
