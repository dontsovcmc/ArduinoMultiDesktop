#include "Arduino.h"
#include "stdlib.h"

#include <menu.h>
#include <menuFields.h>
#include <keyStream.h>
#include <chainStream.h>

#include <multidesktop.h>

#define LED_WORK 3
#define BUTTON_1 4   
#define BUTTON_2 5  

void print(const char *buffer, int ms)
{
	Serial.println(buffer); delay(ms);
}

void up() { print("up pressed", 1000);  }

void down() { print("down pressed", 1000); }

void led_button() { digitalWrite(LED_WORK, !digitalRead(LED_WORK)); }

void menu_button() { print("MENU", 1000); }

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

keyMap encBtn_map[]={{BUTTON_1,menu::upCode}, {BUTTON_2,menu::enterCode}};//negative pin numbers means we have a pull-up, this is on when low
keyLook<2> encButton(encBtn_map);
Stream* in2[]={&encButton,&Serial};
chainStream<2> allIn(in2);


void loop() 
{	
	if (allIn.available()) 
	{
        char ch = allIn.read();
		
		multi_desktop.button_pressed(ch);
		
		if (ch != -1)
			multi_desktop.show(); 
    }
	
	//multi_desktop.show();  // if using LCD or show changed data
	
	delay(1); // delay in between reads for stability
}
