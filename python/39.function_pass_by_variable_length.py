#!/usr/bin/python3
# Function definition is here
def printinfo( arg1, *vartuple ):
    "This prints a variable passed arguments"
    print ("Output is: ")
    #print (arg1)
    for var in vartuple:
        print (var)
    return
# Now you can call printinfo function
#printinfo( 10 )
printinfo( 10,20 )
printinfo( 70, 60, 50 )