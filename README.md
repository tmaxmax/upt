# My university assignments

This is my personal work on the assignments I've received at university and other work directly or indirectly related to them.

I use this repository to back up my work and sharing it with the friends that I help.

## The courses

Here is a short presentation of the courses:
- **Computer Programming**: the basics of C. Pretty much follows the classic book "The C Programming Language", albeit with a little less depth.
- **Logic and Discrete Structures**: touches on the fundamentals of functional programming, discrete structures (lists, sets, relations, graphs etc.), propositional & predicate logic, finite automata, grammars and Turing Machines. I'm not fond of the choice to teach the programming bits in Python, so I also did the assignments in OCaml on the side.
- **Advanced Programming Techniques**: recursion, divide et impera, Greedy, backtracking, graphs, dynamic programming & friends. There is also a non-"advanced" version of the course – the difference lies in the difficulty and depth and in that only students which have had remarkable results in "Computer Programming" can attend it.

## Organization

This is the top level folder structure:
- `algebra`: Programs implemented to consolidate my linear algebra knowledge.
- `c`: My own C libraries implemented for learning purposes and for usage in school assignments.
- `lsd`: Assignments from the course "Logic and Discrete Structures". The teaching language was Python – I chose to solve some of them in OCaml for a deeper understanding of the subject.
- `pc`: Assignments from the course "Computer Programming". These ususally go beyong the required scope by implementing error handling and paying attention to code design and performance concerns.
- `thinkocaml`: Exercises from the book "Think OCaml" by Allen Downey and Nicholas Monje. I've solved them as a supplement to the "Logic and Discrete Structures".
– `tpa`: Assignments from the course "Advanced Programming Techniques". They go beyong the required scope in the same way the solutions to the "Computer Programming" assignments do.

Beyond it there isn't really a coherent organization system.

## Notable work...

...where by "notable" I mean "I'm proud of it" :)

- `c/hash_table`: A generic hash table implementation in C with various other utilities added for ergonomics. It uses closed addressing and xxhash under the hood.
- `c/ticker`: A utility which calls a function in a separate thread at a given interval. Used in conjunction with the utility in `c/try_read.h` for implementing a CLI roulette game (`pc/bets.c`).
- `tpa/polish`: An infix to polish notation converter and a polish notation calculator. The cool thing about it is the input validation and error handling, and that hacky generic stack implementation.
- `algebra/vectors`: OCaml code which checks whether a tuple of vector are linearly dependent.
- `thinkocaml/chapter8.ml`: That binary search inside a list (the `binary_exists` function) which does the bisection using the slow-fast pointers technique. The implementation looks very cool in OCaml :D

The Python code inside `lsd` is full of fat iterator chains, if that's something you're into. Finding the thickest Python one-liners is hell of an exciting mental sport!
