# ArduinoMultiDesktop

MultiDesktop little library for easy buttons + screen coding.

I use https://github.com/neu-rah/ArduinoMenu for button support.

The first example with control through Serial:

# Quick Start

'''
//We have 2 'buttons' for actions. They are characters readed from Serial, or push buttons of ArduinoMenu library.
#define BUTTONS 2
int8_t codes[] = { '+' , '-' };

// The 3rd button - button to change desktop
MultiDesktop<BUTTONS> multi_desktop( '*' , codes);

// Create desktop
void show_1_screen() { /* show smth */ }
void up() { /*do smth by press '+' */ }
void down() { /*do smth by press '-' */ }

// Callback list size == BUTTONS !
func desktop1cb[] = { &up, &down };

Desktop<BUTTONS> mainDesktop(&show_1_screen, desktop1cb); 

// Create another desktop
void show_2_screen() {  /* show smth 2 */ }
void led_button() { /* turn led */ }

// If you don't want to use some buttons, write 0 !
func desktop2cb[] = { &led_button, 0 };

Desktop<BUTTONS> secondDesktop(&show_2_screen, desktop2cb);
'''

Add desktop to MultiDesktop in setup function:
'''
multi_desktop.add_desktop(&mainDesktop);
multi_desktop.add_desktop(&secondDesktop);

multi_desktop.show();
'''

Enjoy:
'''
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
'''
