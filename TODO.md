## List of things to do:

1. Make it so that the map capacity expansion does not cause a seg fault.
2. Implement something to handle hash collisions in the Map structure.
3. Implement actual unit tests for Map. Some cases I need to test:
    
    1. Capacity expansion
    2. Hash collisions
    3. Retrieving non-existant keys
    4. Map destruction
    5. Creating the map in one scope and returning it to another scope (make sure all data is preserved)

4. Implment functions to load json data from the AST to a Map. (Have not even begun to think about this).
5. I also probably want to implment some more/actual unit tests for the lexer and parser.


## Issues:

ISSUE: When I expand the map, I allocate a new, bigger map. I then assign the pointer to the new map to the
    pointer to the old map. I recently changed all insert functions to require passing the pointer to the map
    by reference which should mean that I can update the contents and that will be reflected in the caller.
    That doesn't seem to be happening, the final address of the original map is different than the newly allocated
    address. Also, since the updates are not reflected, this is vulnerable to bugs like use-after-free and double-free.

ISSUE: In key-value pairs, values of type char* get truncated after 4 characters. Keys are not affected.


## Notes:

When it comes to storing booleans, I have two options: store them as strings or store them as 0/1

I'm wondering whether `initMap` should allocate a map from the heap and return a pointer or create a map on the stack
then return the map. If I use the stack there is less memory management to worry about but then it is dropped if you don't
return it from whatever function you are in. Maybe that is not a big deal but we also have to consider that the KeyValuePair array
is allocated from the heap (and it must be since it's size is determined at runtime) and I think there will be a memory leak
if the map gets dropped without destroying the array (which seems very likely to happen).
