import time  # import time module

# This script can be used as a module


# Define a function that takes no arguments, prints the system time, and delays 0.5 seconds
def showtimefunction():
    print(time.time())
    delay(0.5)


# Define a function that delays t seconds
def delay(t):
    time.sleep(t)


# If you'd like to run a module or script directly, you can put an example of it's use at the bottom
# The following condition is True when a script is executed directly (not imported or used externally)
if __name__ == "__main__":

    # Set 'start' to the current system time
    start = time.time()

    # Run the functions from above
    showtimefunction()
    delay(1.0)
    showtimefunction()
    delay(0.5)
    showtimefunction()

    # Set 'now' to the current system time
    now = time.time()

    # Get the elapsed time 'dt'
    dt = now - start

    # Print 'dt'
    print('\nElapsed: ' + str(dt))

