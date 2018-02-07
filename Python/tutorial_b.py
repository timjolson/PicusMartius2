# To use these functions and the stopwatch class, you need tutorial_b_sub.py in the same folder

###
# Import an entire module
import tutorial_b_sub

# Use a function from the module
tutorial_b_sub.showtimefunction()
tutorial_b_sub.delay(0.5)
tutorial_b_sub.showtimefunction()
print('Module was used to call functions\n')


###
#  Import specific functions from the module
from tutorial_b_sub import delay
from tutorial_b_sub import showtimefunction

# Use the functions directly
showtimefunction()
delay(0.5)
showtimefunction()
print('Functions were imported directly\n')


###
# Import module as another name
import tutorial_b_sub as tb

# Use the functions
tb.showtimefunction()
tb.delay(0.5)
tb.showtimefunction()
print('Module was under another name\n')


###
# From the module, import the class we made
from stopwatch import StopWatch

# Make and start a big timer object
bigT = StopWatch()
# Notice the constructor is called without specifying any arguments ( 'dec' will be set to the default ).
# Also notice 'self' in the method definition is not passed in, it is implicit.

# Run this loop for decimal precisions in the list below
listOfPrecision = [0, 3]
for i in listOfPrecision:

    # We're going to create a StopWatch that rounds it's output to 'i' decimal places.
    # Print that out.
    print('\nObject was created with %d decimal rounding' % i)

    # Create instance of the class, this also sets the start time
    t = StopWatch(i)
    # Notice an argument is passed in ( 'dec' will be set to the input argument ).
    # Also notice 'self' is still not passed in.

    # Use methods of the StopWatch class on the 't' instance, print the results
    delay(0.4001)
    print(str(t.lap()) + ' lap')  # get elapsed
    delay(0.5001)
    print(str(t.lap()) + ' lap')  # get elapsed again
    delay(2.6)
    print(str(t.restart()) + ' restart')  # get elapsed and start again
    delay(1.4007)
    print(str(t.lap()) + ' lap')  # get elapsed since the restart

# Get the elapsed time from bigT, print it as a float (default precision 6 decimal places,
# but bigT returns to 2 decimal places)
print('\nTime for loop: %f' % bigT.lap())

###
# You can do stuff now!
print('Now you can create and import modules, classes, and functions.')
print('Oh, and print %s types of other things like %d, %1.2f, and %1.4f' % ('many', 7.1, 99.9999, 99.9999))

