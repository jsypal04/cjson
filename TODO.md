## List of things to do:

1. Implement actual unit tests for Map. Some cases I need to test:

    1. Capacity expansion
    3. Retrieving non-existant keys
    4. Map destruction
    5. Creating the map in one scope and returning it to another scope (make sure all data is preserved)

2. I also probably want to implment some more/actual unit tests for the lexer and parser.
3. Implement capacity expansion for appending arrays
4. Store booleans as booleans not as strings
5. Store nulls as NULL


## Issues:

ISSUE: I'm pretty sure I have memory leaks whenever I insert into a map since I malloc the value in insertInt, insertFloat, insertString,
      and insertMap functions and re-copy (using malloc) again in the insert function. The I don't think the pointers allocated in the
      upper lever insert functions are ever freed.
      EDIT: I think I fixed this but I'm going to keep this just to remind myself to test for it later.

ISSUE: Fix the map capcity expansion in insert, I don't think it actually expands the capacity.


## Notes:

- When it comes to storing booleans, I have two options: store them as strings or store them as 0/1

- I'm wondering whether `initMap` should allocate a map from the heap and return a pointer or create a map on the stack
then return the map. If I use the stack there is less memory management to worry about but then it is dropped if you don't
return it from whatever function you are in. Maybe that is not a big deal but we also have to consider that the KeyValuePair array
is allocated from the heap (and it must be since it's size is determined at runtime) and I think there will be a memory leak
if the map gets dropped without destroying the array (which seems very likely to happen).
