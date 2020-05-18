# SPO

Language's grammar is in the regexp.txt

The language support:
Assignment, if-else and while statements
If-else and while statements can have either one expression or several expressions(with the usage of {body statement}
Initialization and modyfication of int variables

The language DOES NOT support:
Scope of variables
Any function creation or usage
body statements outside of if-else and while statements
Infinite parentheses

Program also not optimized and expects to get program code in one input(in one string) otherwise it won't work correctly
Program doesn't check if variable was initialized in reachable/unreachable bodies (example: variable is initialized in if statement that won't be reached, but this variable will be considered as initialized in the rest of the program)
