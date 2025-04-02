**List of things to do:**

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
