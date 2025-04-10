## List of things to do:

1. Make it so that the map capacity expansion does not cause a seg fault.
2. Implement actual unit tests for Map. Some cases I need to test:

    1. Capacity expansion
    3. Retrieving non-existant keys
    4. Map destruction
    5. Creating the map in one scope and returning it to another scope (make sure all data is preserved)

3. Implement functions to load json data from the AST to a Map. (Have not even begun to think about this).
4. I also probably want to implment some more/actual unit tests for the lexer and parser.


## Issues:

ISSUE: I'm pretty sure I have memory leaks whenever I insert into a map since I malloc the value in insertInt, insertFloat, insertString,
      and insertMap functions and re-copy (using malloc) again in the insert function. The I don't think the pointers allocated in the
      upper lever insert functions are ever freed.


## Notes:

- When it comes to storing booleans, I have two options: store them as strings or store them as 0/1

- I'm wondering whether `initMap` should allocate a map from the heap and return a pointer or create a map on the stack
then return the map. If I use the stack there is less memory management to worry about but then it is dropped if you don't
return it from whatever function you are in. Maybe that is not a big deal but we also have to consider that the KeyValuePair array
is allocated from the heap (and it must be since it's size is determined at runtime) and I think there will be a memory leak
if the map gets dropped without destroying the array (which seems very likely to happen).
