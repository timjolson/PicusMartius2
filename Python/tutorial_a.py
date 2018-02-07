import time

# An excellent resource for Python information can be found at: https://www.tutorialspoint.com/python/
#
# Python, C++, and MATLAB can all be used to do many of the same things. Each has their own pros and cons, however.
# For example, C++ can be used to complete a wide variety of tasks quickly and efficiently.
# MATLAB is excellent for manipulation of matrices.
# Python is very useful in data manipulation, especially when that data is stored in lists or .txt files.

# One major thing to note is that both Python and MATLAB are scripting languages whereas C++ is a compiling language.
# A compiling language like C++ runs a compiler before running. This compiler goes through all of the written code to
# ensure that the necessary relations, syntax, and logic are there. If everything is present an executable file (.exe)
# is created and then executed.
# In a scripting language such as Python, the code is evaluated line-by-line as it is executed. This is done through
# an interpreter, NOT a compiler. This can cause the code to execute perfectly, but if the last line fails, the entire
# program fails and you won't know until you run the entire thing.
# Additionally, Python is a high level language, meaning that it is generally better at interpreting what the
# programmer meant/was trying to accomplish. This means that the syntax used is less restricting or specific. For
# example, when declaring a variable:
# C++:    int x = 0;
# Python: x = 0
# You do not need to declare the variable type as it will be interpreted when the script is running. No semi-colon is
# necessary either as Python groups things by indentation. Let me repeat that because that's one major difference
# between Python and C++.
#
# PYTHON GROUPS BY INDENTATION
#
# IF ELSE STATEMENTS / PRINT STATEMENTS -------------------------------------------------------------------------------
#
# In Python, all conditional statements must be followed by a colon. That is, if (conditional):
# Then all statements to be executed should the conditional be true need to be indented 1 tab. To end the if-statement,
# simply backspace so that the code is no longer tabbed in.
#
# Output is different from C++ and MATLAB as well. In C++, the keyword 'cout' is used to output information to the
# screen. In MATLAB, 'display()', 'sprintf()', and a variety of other functions are used to send information to the
# screen. In Python, the keyword 'print' is used to display information on the screen. 'print' does not require
# parenthesis to be placed around the item to be printed and is very versatile.
#
# When printing multiple data types, i.e., printing ints, floats, etc. with strings (text), the '+' operator can be used
# to combine things to be printed. Additionally, the number needs to be typecasted to a string in order to be printed
# with the other items. This can be done using str(variable). Examples of this can be seen below.

x = 0
if x == 0: # Note the usage of the colon
    print 'This is an example of how indentation groups ' \
          'statements and line wrapping can be done in Python.\n' # The \ notation allows the print statement to be broken into two+ lines
    print 'This third line of the if-statement will still be executed.\n'

print 'The following print statement is only printing the variable "x" so the variable does not need to be typecasted.'
print x    # Because we are only printing the number by itself, it does not need to be typecasted
print '\n' # Formatting

print 'Because the following print statement is displaying both strings and numeric variables, \nthe numeric variables ' \
      'need to be typecasted as strings.\n'
print 'This can be done by making use of the str(x) function where x = ' + str(x) + '\n' # Typecasting 'x' to a string, allowing it to be printed with the other text

# For if-else statements, Python does not use 'else if' like C++, nor does it use 'elseif' like MATLAB.
# Instead, Python uses 'elif' where the 'if' statement, 'elif' statement, and 'else' statement are separated by
# indentation.

x = 1
y = 2

if x > y:
    print str(x) + ' > ' + str(y)
elif x < y:
    print str(x) + ' < ' + str(y)
else:
    print str(x) + ' = ' + str(y)

x = 2
y = 1

if x > y:
    print str(x) + ' > ' + str(y)
elif x < y:
    print str(x) + ' < ' + str(y)
else:
    print str(x) + ' = ' + str(y)

x = 3
y = 3

if x > y:
    print str(x) + ' > ' + str(y)
elif x < y:
    print str(x) + ' < ' + str(y)
else:
    print str(x) + ' = ' + str(y)

print '\n' # Formatting

# LISTS ---------------------------------------------------------------------------------------------------------------
# Arrays are a very powerful aspect of C++, matrices are a very powerful aspect of MATLAB, and lists are a very
# powerful aspect of Python. An important thing to note about lists is that the
# INDEXING STARTS AT 0. This is similar to C++, but different from MATLAB, which starts indexing at 1.
# To access members of a list, treat them similar to arrays and use the bracket [] notation where the first element
# is at index [0] and the furthest element is at index [size-1].
# The benefits of lists are that they do not have to be the same data type. The contents of
# a list need to be placed within square brackets [] and separated by commas. An example can be seen below where a
# list is used to represent various information about a person in this order:
# Name -         string
# Age -          integer
# Phone Number - string
# Address -      string
# Height -       float (feet.inches)

person1 = ['Boaty Mcboatface', 25, '312-867-5309', '101 Strait Street', 30.5]
print 'Name: ' + person1[0]
print 'Age: ' + str(person1[1]) # NOTE: this needs to be typecasted because it is not a string
print 'Phone Number: ' + person1[2]
print 'Address: ' + person1[3]
print 'Height: ' + str(person1[4]) # NOTE: this needs to be typecasted because it is not a string

# Another advantage of lists is how they can be integrated into loops, but this will be covered later in this tutorial.
# The number of elements in the list can be interpreted to be the length of the list. Luckily Python has a built-in
# function enabling us to easily calculate the length.
length1 = len(person1)
print 'Pieces of information: ' + str(length1)
print '\n' # Formatting

# Concatenation is very easy to accomplish with lists too. Let's create a second list for another person and then
# combine the two lists to create a database of people.

person2 = ['Seas Theday', 30, '773-202-5862', '200 Tide Turnabout', 25.6]
print 'Name: ' + person2[0]
print 'Age: ' + str(person2[1]) # NOTE: this needs to be typecasted because it is not a string
print 'Phone Number: ' + person2[2]
print 'Address: ' + person2[3]
print 'Height: ' + str(person2[4]) # NOTE: this needs to be typecasted because it is not a string

length2 = len(person2)
print 'Pieces of information: ' + str(length2)
print '\n' # Formatting

# To create the database we're simply going to concatenate the two lists together
database = person1 + person2
print 'The database list now contains:'
print database
print '\n' # Formatting

# There are many built-in functions for Python lists and can be found online at https://www.tutorialspoint.com/python/
# list.append(obj) - adds obj to the end of the list
# list.count(obj) - counts how many times obj appears in the list
# list.insert(index, obj) - inserts obj into list at index
# list.pop(obj=list[-1]) - removes and returns last obj from list
# list.remove(obj) - removes obj from the list
# list.reverse() - reverses list
# list.sort([func]) - sorts list using comparison function

# DICTIONARIES -------------------------------------------------------------------------------------------------------
#
# Dictionaries are another useful tool in Python. These are very similar to what we did above by creating a database.
# Dictionaries can be thought of as a more primitive class as you can "store" data into them based on keys.
# For initialization, curly braces {} need to be used, rather than brackets, and each data member stored inside of it
# needs to be paired with a corresponding key, separated by a colon. The data member can later be accessed by using its
# key inside of square brackets []. If the data member is accessed based on its index, it will break the script.
# For each key, there needs to be a corresponding data member separated by a colon. Let's reuse the person example
# from above:

dictionary = {'Name1': 'Boaty Mcboatface', 'Age1':25, 'Phone Number1': '312-867-5309', 'Address1': '101 Strait Street',
              'Height1': 30.5, 'Name2': 'Seas Theday', 'Age2': 30, 'Phone Number2': '773-202-5862', 'Address2':
              '200 Tide Turnabout', 'Height2': 25.6}

print 'Dictionary-----------------------'
print 'Name: ' + dictionary['Name1']
print 'Age: ' + str(dictionary['Age1'])
print 'Phone Number: ' + dictionary['Phone Number1']
print 'Address: ' + dictionary['Address1']
print 'Height: ' + str(dictionary['Height1'])
print '    ----    '
print 'Name: ' + dictionary['Name2']
print 'Age: ' + str(dictionary['Age2'])
print 'Phone Number: ' + dictionary['Phone Number2']
print 'Address: ' + dictionary['Address2']
print 'Height: ' + str(dictionary['Height2'])
print '\n' # Formatting

# It's easy to delete elements of a dictionary too, just use the 'del' keyword
del dictionary['Name1']
print dictionary

# It can be seen that 'Boaty Mcboatface' has been removed from the dictionary. We can also clear the entire dictionary
# using the built-in clear function.
dictionary.clear()
print dictionary
print '\n' # Formatting

# This leaves it clear. We can also delete the dictionary entirely in a similar fashion to deleting just one element.
del dictionary

# Dictionaries have many useful built-in functions, similar to lists.
# len(dict) - gives length of dictionary
# dict.clear() - clears all elements of dictionary
# dict.get(key) - returns the value stored at 'key' or 'None' if that key does not exist in the dictionary
# dict.has_key(key) - returns 'true' if the key is present in the dictionary, 'false' if not
# dict.keys() - returns a list of the keys within the dictionary
# dict.update(dict2) - adds the keys and associated values from dict2 to dict
# dict.values() - returns a list of all the values within the dictionary

# LOOPS---------------------------------------------------------------------------------------------------------------

# Loops are a powerful tool no matter what language is utilizing them. Python has the usual 'for' and 'while' loops,
# but does not have a 'do ... while' loop unlike C++. Because Python is a high-level language, the syntax for loops
# can be quite different from C++ or MATLAB. The best way to learn, however, is to see examples.
#
# In C++, a basic 'for' loop to count from 0 to 20 would be:
# for(int i = 0; i <= 20; i++) {
# // do stuff
# }
#
# In MATLAB, this same loop would be written as:
# for i=0:1:20
# % do stuff
# end
#
# In Python, the loop to do this could be written a few different ways, shown below:
end = 20
print 'For Loops counting from 0 to ' + str(end) + '\n-------------------------------'
print 'i in range(end+1)'
for i in range(end+1):
    print i

print '\ni in range(0,end+1)'

for i in range(0,end+1):
    print i

print '\ni in range(0,end+1,5)'

for i in range(0,end+1,5):
    print i

print '\n' # Formatting

# 'while' loops are closer to C++ and MATLAB than 'for' loops.
# In C++, a while loop counting 0 to 20 would be:
# while(j <= 20){
# // do stuff
# j++;
# }
#
# In MATLAB it would be:
# while j<=20
# % do stuff
# j = j + 1
# end
#
# Unlike 'for' loops, you DO have to declare the iteration variable first:
j = 0
print 'While Loops counting from 0 to ' + str(end) + '\n---------------------------------'
while j <= end:
    print j
    j = j + 1

# Looping through lists in Python can be extremely useful. However, it can be quite confusing if you're not familiar
# with it. When looping through a list, the iterator actual takes on the value of whatever is in that particular index
# of the list, rather than simply being the index. This makes it especially useful when dealing with lists contained in
# .txt files or other types of file I/O. This is easier to understand by example. Let's create a list of
# spices/condiments and their associated prices immediately following.
seasonings = ['salt', 1.29, 'pepper', 1, 'liquid-y tomato', 3.54]

# Now let us loop through this list using a 'for' loop:
print 'Looping through a list\n----------------------'
for i in seasonings:
    print i # NOTE: this is printing the iterator. This would normally represent the index

# This is why, when looping through lists, the iterator is usually called something more meaningful than 'i'. Below is
# an example of a better name for the iterator using a simplified list:
print '\nLooping through a simpler list to illustrate that, when looping through a list,\nthe iterator is not the' \
    ' index within the list. Rather, it is the element itself.'

simple_seasonings = ['salt', 'pepper', 'ketchup']
index = 0

for element in simple_seasonings:
    if index == 0:
        print 'First Iteration\n---------------'
        print 'index: ' + str(index)
        print 'iterator (element): ' + element
        index = index + 1
    elif index == 1:
        print 'Second Iteration\n----------------'
        print 'index: ' + str(index)
        print 'iterator (element): ' + element
        index = index + 1
    elif index == 2:
        print 'Third Iteration\n---------------'
        print 'index: ' + str(index)
        print 'iterator (element): ' + element
        index = index + 1

print '\n' # Formatting

# INFINITE LOOPS AND TIME ---------------------------------------------------------------------------------------------
#
# Infinite loops can be useful when repeatedly executing a statement until certain conditions are met. An easy example
# of this is when you need certain pieces of code to execute for a certain amount of time. To streamline this timing
# process, Python has a module that can be included called time. If you look at the very top of this file, there is a
# line of code that reads "import time". With this, we can utilize several functions/methods that deal with time.
#
# time.time() is a very useful function. This receives the time since the last epoch. By calling this function
# prior to the loop, we get an initial time. We can then execute this within the loop to obtain an updated time.
# Comparing the two will give us an elapsed time. time.sleep() is another very useful function, allowing us to
# essentially pause the code execution for a specified amount of time.
#
# 'break' is a useful keyword for leaving loops, especially infinite loops. It is very convenient for timing purposes.
#
# This is an infinite loop that will execute until 10 seconds have elapsed. At this point it will break out of the
# infinite loop and continue executing the rest of the code below.

print 'Infinite loop making use of break statement\n-------------------------------------------'

initial = time.time()

while True:
    time.sleep(1)
    print 'This should print every 1.0 seconds.'
    current = time.time()
    elapsed = current - initial
    if elapsed >= 10:
        print 'Elapsed time: ' + str(elapsed)
        break

print '\n'

print 'Non-infinite loop using timer\n-----------------------------'
# This loop will do exactly the same thing as the loop above, but does not include a 'break' statement
initial = time.time()
current = initial

while (current - initial) < 10:
    time.sleep(1)
    print 'This should print every 1.0 seconds.'
    current = time.time()

print 'Elapsed time: ' + str(current - initial)
print '\n' # Formatting
# TRIES AND EXCEPTIONS ------------------------------------------------------------------------------------------------
#
# As you've all experienced, different things can break your code, causing it to crash when attempting to run it.
# If you know ahead of time where problems might pop up and potentially break your code, you can plan for it and
# take precautions to prevent your code from breaking. This is usually done by way of 'try' and 'except' statements.
# These statements allow you to run code that might potentially break and if it does break, print an error message for
# use when debugging later. Various error messages can be generated too depending on how the code broke.
#
# An excellent example of this would be division by 0. Obviously division by 0 will crash any program since it is
# impossible to perform. However, by placing the division within a 'try' statement, the code will execute normally
# until the denominator becomes 0. In that case, it will fail the 'try' statement and instead execute the 'except'
# statement which, in the following example, simply prints an error message stating why the code broke.

iterations = 10

while iterations > -1:
    result = 0

    # Place the code that could potentially break within the 'try' statement
    try:
        result = round(float(10) / iterations,2) # round() simply rounds the calculation to 2 decimal places

    # If the code in the 'try' statement breaks, this 'except' statement will execute
    except ZeroDivisionError as err:
        result = '---'
        print err
    # except:
    #     # other error
    #     pass

    print '(10 / ' + str(iterations) + ') = ' + str(result)
    iterations = iterations - 1