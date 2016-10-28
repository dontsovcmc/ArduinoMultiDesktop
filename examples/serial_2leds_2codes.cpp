#include "Arduino.h"

#include "multidesktop.h"

#define LED1_PIN 2
#define LED2_PIN 3

#define BUTTON_1 4   
#define BUTTON_2 5  


void print(const char *buffer, int ms)
{
	Serial.println(buffer); delay(ms);
}


void toggle_led_1() { digitalWrite(LED1_PIN, !digitalRead(LED1_PIN)); }
void toggle_led_2() { digitalWrite(LED2_PIN, !digitalRead(LED2_PIN)); }

void show_1_screen() 
{
	digitalRead(LED1_PIN) ? print("LED 1 on", 0) : print("LED 1 off", 0);
	print("[ + toggle ]   [ * >2 ]", 0);
}

void show_2_screen()
{
	digitalRead(LED2_PIN) ? print("LED 2 on", 0) : print("LED 2 off", 0);
	print("[ + toggle ]   [ * >1 ]", 0);
}

#define BUTTONS 1
int8_t codes[] = { '+' };  // '+' ACTION BUTTON

func desktop1cb[] = { &toggle_led_1 };
func desktop2cb[] = { &toggle_led_2 };

MultiDesktop<BUTTONS> multi_desktop( '*' , codes); // '*' NEXT DESKTOP BUTTON
Desktop<BUTTONS> mainDesktop(&show_1_screen, desktop1cb); 
Desktop<BUTTONS> secondDesktop(&show_2_screen, desktop2cb);


void setup()
{
	pinMode(LED1_PIN, OUTPUT);
	pinMode(LED2_PIN, OUTPUT);
	
	pinMode(BUTTON_1,INPUT);
	pinMode(BUTTON_2,INPUT); 
	
	digitalWrite(LED1_PIN, LOW);
	digitalWrite(LED2_PIN, LOW);
	
	Serial.begin(9600);
	
	print("HELLO", 500);
	
	multi_desktop.add_desktop(&mainDesktop);
	multi_desktop.add_desktop(&secondDesktop);
	
	multi_desktop.show();
}

void loop() 
{	
	if (Serial.available()) 
	{
        char ch = Serial.read();
		
		multi_desktop.button_pressed(ch);
		multi_desktop.show();
    }
	
	delay(1);
}
