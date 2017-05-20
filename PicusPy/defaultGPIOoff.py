# If you're using GPIO on the ODROID, use wiringPi to turn signal lines off

import sys
import wiringpi2 as droid

# list of GPIOs in use TODO: update for your case (include ALL pins)
pinlist = [21, 22, 29, 188, 25, 190, 24, 19, 173, 171, 172, 31, 30, 28, 187, 189, 191, 192, 174, 18, 210, 209]

# this function is callable from other scripts if imported
def defaultoff():
    # try to initialize wiringPi
    try:
        droid.wiringPiSetupGpio()
        # try to set all pins as input, then output, then off
        try:
            for pin in pinlist:
                droid.pinMode(pin, 0) # set as input
                droid.pinMode(pin, 1) # set as output
                droid.digitalWrite(pin, 0) # set pin low
            print('initialized GPIO pins off')
            # return good exit code (useful if run from bash script)
            exit(0)
        except Exception:
            print('error setting pins off')
            # return bad exit code
            exit(1)
    # could not initialize wiringPi
    except Exception:
        print("error - PiSetupGpio")
        # return bad exit code
        exit(1)


# if you run script directly, just call the function
if __name__ == '__main__':
    defaultoff()
