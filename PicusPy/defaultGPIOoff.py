import sys
import wiringpi2 as droid

pinlist = [21, 22, 29, 188, 25, 190, 24, 19, 173, 171, 172, 31, 30, 28, 187, 189, 191, 192, 174, 18, 210, 209]

def defaultoff():
    try:
        droid.wiringPiSetupGpio()
        try:
            for pin in pinlist:
                droid.pinMode(pin, 1)
                droid.digitalWrite(pin, 0)
            print('initialized GPIO pins off')
            exit(0)
        except Exception:
            print('error setting pins off')
            exit(1)
    except Exception:
        print("error - PiSetupGpio")
        exit(1)


if __name__ == '__main__':
    defaultoff()
