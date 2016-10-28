
#ifndef MULTI_DESKTOP_H
#define MULTI_DESKTOP_H

extern "C" {
  typedef void (*func)(void);
}

/**
@brief Desktop class
@param show_ - callback function when this Desktop is active
@param cb    - callback functions for all N buttons. i function called when user press i button.
*/
template <int8_t N>  //keys
class Desktop
{
public: 
	Desktop<N>(func show_, func cb[])
		: show(show_)
		, buttons_callbacks(cb)
		, next(0)
	{ }
	
	/**
    @brief callback function when this Desktop is active
    */
	func show;
	
	/**
    @brief link to next Desktop in MultiDesktop configuration
    */
	Desktop *next;
	
	/**
    @brief callback functions for all N buttons. i function called when user press i button.
    */
	func *buttons_callbacks;
private:
};


/**
@brief MultiDesktop class
You can change Desktops for show different info and call different 
functions in each of them by pressing same buttons.

@param N         - number of keys you want to use except 'next desktop' key.
@param next_code - key code for go to next Desktop  
@param codes     - N key codes you want to press
*/
template <int8_t N> 
class MultiDesktop
{
public:
	MultiDesktop<N>(int8_t next_code, int8_t codes[])
		: current(0)
		, first(0)
		, key_codes(codes)
		, next_desktop_code(next_code)
	{}
	
	/**
	@brief add_desktop add new Desktop. Desktops are 1-direction looped list.
	@param new_d     - link to new Desktop
	*/
	void add_desktop(Desktop<N> *new_d)
	{
		if (!first)
		{
			first = new_d;
			current = new_d;
		}
		else
		{
			Desktop<N> *d = first;
            while (d->next && d->next != first) 
				d = d->next;
			
			d->next = new_d;
			new_d->next = first;
		}
	}
	
	/**
	@brief call this function when user press button. call show() to redraw screen.
	@param code     - button code
	*/
	void button_pressed(int8_t code)
	{
		if (next_desktop_code == code)
		{
			if (current->next)
				current = current->next;
		}
		else
		{
			for (int8_t i=0; i < N; i++)
			{
				if (key_codes[i] == code)
				{
					if (current->buttons_callbacks[i])
						current->buttons_callbacks[i]();
				}
			}
		}
	}
	
	/**
	@brief call show function of current desktop
	*/
	void show()
	{
		if (current->show)
			current->show();
	}
	
private:
	Desktop<N> *current;
	Desktop<N> *first;
	
	int8_t next_desktop_code;
	int8_t *key_codes;
};

#endif
