#include "mbed.h"
#include "button.h"

DigitalOut led1(LED1);
Button bt(BUTTON1);
	
int main() {
	
    printf("This is the button class example running on Mbed OS %d.%d.%d.\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);

	while (true) {

        long long time = bt.pressedTime();

        if(time) {

            printf("Pressed time: %lld\n", time);

            if(time > 500)
                led1 = !led1;

        }

    }
	
}
