# ArduinoMultiDesktop

MultiDesktop little library for easy buttons + screen coding.

![MultiDesktop library overview](/img/main.png)

I use https://github.com/neu-rah/ArduinoMenu for button support.

The first example with control through Serial:

## Quick Start

```
//We have 2 'buttons' for actions. They are characters readed from Serial, or push buttons of ArduinoMenu library.
int8_t codes[] = { '+' , '-' };

// The 3rd button - button to change desktop
MultiDesktop<2> multi_desktop( '*' , codes);

// Create desktop
void show_1_screen() { /* show smth */ }
void up() { /*do smth by press '+' */ }
void down() { /*do smth by press '-' */ }

// Callback list size == 2 !
func desktop1cb[] = { &up, &down };

Desktop<2> mainDesktop(&show_1_screen, desktop1cb); 

// Create another desktop
void show_2_screen() {  /* show smth 2 */ }
void led_button() { /* turn led */ }

// If you don't want to use some buttons, write 0 !
func desktop2cb[] = { &led_button, 0 };

Desktop<2> secondDesktop(&show_2_screen, desktop2cb);
```

Add desktop to MultiDesktop in setup function:
```
multi_desktop.add_desktop(&mainDesktop);
multi_desktop.add_desktop(&secondDesktop);

multi_desktop.show();
```

Enjoy:
```
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
```

## Examples

See examples in source folder:

1. serial_2leds_2codes.cpp
  * Input '+' '*' from Serial, 2 desktops, 1 actions on each desktop : toggle own LED

2. serial_3codes.cpp
  * Input '+' '-' '*' from Serial, 3 desktops, 1-2 actions: print smth

3. serial_button_3codes.cpp
  * Input '+' '-' '*' from Serial or 3 buttons, 3 desktops, 1-2 actions: print smth

4. serial_button_led_menu_3codes.cpp
  * Input '+' '-' '*' from Serial or 3 buttons:
  * Desktop 1: message
  * Desktop 2: toggle LED
  * Desktop 3: start menu

  * Menu: toggle LED





