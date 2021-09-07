#include <mbed.h>
#include <chrono>

/** A button input, used for reading the state of a button with wise debounce
 *
 * @note Synchronization level: Interrupt safe
 *
 * Example:
 * @code
 * // Change state of the led when button is pressed
 *
 * #include "mbed.h"
 * #include "button.h"
 *
 * Button button(p5);
 * DigitalOut led(LED1);
 *
 * int main() {
 *     while(1) {
 *         if(button) {
 *             led = !led;
 *         }
 *     }
 * }
 * @endcode
 */
class Button {
private:
    DigitalIn in;
    bool state = 0;
    Timer tim;

public:
    /** Create a Button connected to the specified pin
     *
     *  @param pin DigitalIn pin to connect to
     *  @param mode the initial mode of the pin
     */
    Button(PinName pin, PinMode mode = PullDown) : in(pin, mode) {
        tim.start();
    }
    inline void mode(PinMode mode) {
        in.mode(mode);
    }

    /** Read the button, represented as 0 or 1 (bool)
     *  The button will be read one time as pressed with debounce method
     *
     *  @returns
     *    An boolean representing the state of the button,
     */
    bool read() {
        long long elapsed =  std::chrono::duration_cast<std::chrono::milliseconds>(tim.elapsed_time()).count();
        if(in && !state && elapsed > 30) {
            state = true;
            tim.reset();
            return true;
        }
        if(!in && state && elapsed > 30) {
            state = false;
            tim.reset();
        }
        return false;
    }

    /** Read the time pressed button (long long)
     *
     *  @returns
     *    An long long representing the time of pressed button in milliseconds,
     *    Using this method disables the read or operator int method.
	 *
	 * \sa Button::pressedTime()
     * @code
     *      Button  button(BUTTON1);
     *      DigitalOut led(LED1);
	 *		if(button.pressedTime() > 500)
     *      	led = !led;
     * @endcode
     */
    long long pressedTime() {
        if(in && !this->state) {
		    this->state = 1;
            tim.reset();
	    }
	    else if(!in && this->state) {
		    this->state = 0;
		    long long elapsed =  std::chrono::duration_cast<std::chrono::milliseconds>(tim.elapsed_time()).count();
            tim.reset();
            return elapsed;
	    }
	    return 0;
    }

    /** Return the output setting, represented as 0 or 1 (int)
     *
     *  @returns
     *    Non zero value if pin is connected to uc GPIO
     *    0 if gpio object was initialized with NC
     */
    int is_connected() {
        // Thread safe / atomic HAL call
        return in.is_connected();
    }

    /** An operator shorthand for read()
     * \sa Button::read()
     * @code
     *      Button  button(BUTTON1);
     *      DigitalOut led(LED1);
     *      led = button;   // Equivalent to led.write(button.read())
     * @endcode
     */
    inline operator int() {
        return read();
    }

};
