# Welcome to the SGK wiki!

### SGK is a Simple GUI Kit, used to create forms.

## Brief Introduction
you can use `SGK_Container` to represent some widgets in a specific layout,<br>
we have defined some Widgets:<br>
`SGK_Button`, `SGK_InputBox`, `SGK_RadioButton`, `SGK_CheckBox` 

## How to use it
library is still under development and testing, so you can read the header files, it's still easy to understand with comments.<br>
also you can read test files from `test` directory, it will help a lot.
<br>
<br>

## Install and test

to compile the library, just simply type :

    make
    make intall
    
to test the library type for example:

    make test/system-test.out # or another target 
    cd test # because ttf file is inside the test directory, we will fix it later
    ./system-test.out
    
read `Makefile` to get the target available 
    

# TODO list
1. Add events structure and event handler.
2. Add and Update more Containers Layout.
3. Add System global variable to save current Data like Active input texts and running states.
