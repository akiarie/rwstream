---
title: ‘What I Learned Implementing Lex’
date: 2022-12-05T07:51:20+03:00
---

Compiler construction is at the heart of computer science. As I continue to make
my way, if very slowly, though _The Dragon Book_, it is becoming absolutely
clear to me that some things cannot be understood without building them. 
What Halmos stated so strikingly about mathematics is applicable here:

> The only way to learn mathematics is to do mathematics [...] The right way to
> read mathematics is first to read the definitions of the concepts and the
> statements of the theorems, and then, putting the book aside, to try to
> discover the appropriate proofs. If the theorems are not trivial, the attempt
> might fail, but it is likely to be instructive just the same.  To the passive
> reader a routine computation and a miracle of ingenuity come with equal ease,
> and later, when he must depend on himself, he will find that they went as
> easily as they came. The active reader, who has found out what does not work,
> is in a much better position to understand the reason for the success of the
> author's method, and, later, to find answers that are not in books. 
> --- Paul Halmos, _A Hilbert Space Problem Book_.

The only way to understand compilers is to build them. It is my goal, therefore,
to attach a project to each chapter (or sequence of chapters) that addresses a
particular topic: and for chapter 3, which is on lexical analysis, the only
possible answer was an implementation of Lex.

In this post I will describe [my implementation](https://github.com/akiarie/lex)
at a high level and then speak briefly about what I learned in the process.

## My implementation

To understand this section one must be familiar with the material in chapter 3
of _The Dragon Book_, i.e. with the basics of finite automaton theory (and its
relationship to regular expressions). I will quote freely from the book without
attribution below.

### Lex

Lex is a lexical-analyser generator, which means that it takes in a Lex source
file and produces a lexical-analyser (a _lexer_, more briefly) in C.
A Lex source file has three components:

- _Definitions_ of patterns in the form of regular expressions

- _Rules_ which describe actions to be taken when certain patterns (which we
  shall call _tokens_) are encountered

- _User subroutines_ or _auxiliary functions_: blocks of code which are copied
  verbatim into the generated file.

The rules each have the form
```
pattern { action }
```
where `pattern` is basically[^def] defined in the _Definitions_ section,
and `action` is C code to be executed, usually referencing something in the
_User subroutines_ section.

Lex works by first building NFAs that match the patterns for each of the tokens
in the _Rules_ section, and then synthesizing these into a "total" NFA, as it
were, which accepts when any one of the patterns is matched (and contains the
necessary information to identify which token has been matched). The generated
lexer makes use of this total automaton and the input sequence, creating a
function `yylex` as beneath:

```C
/* nexttoken: obtain name of next matched pattern. return NULL on EOF */
char *
nexttoken();

/* yylex: lex until next token with returning action. return 0 on EOF */
int
yylex()
{
	char *tk = nexttoken();
	if (NULL == tk) {
		return 0; /* EOF */
	} else if (strcmp(tk, "pattern0") == 0) {
		/* pattern 0 action */
	} else if (strcmp(tk, "pattern1") == 0) {
		/* pattern 1 action */
	} /* else if ... {
		...
	} */

	fprintf(stderr, "unknown pattern %s\n", tk);
	exit(EXIT_FAILURE);
}
```

### Implementation

In order to construct such a `yylex` from a Lex source file, an implementation
must be able to do the following:

1. _Parse_ the input source file such that the distinct sections and their
   components are identified

2. _Build and combine NFAs_ corresponding to the patterns in the file 

3. _Simulate NFAs_ based on input sequences, identifying when they enter
   accepting (as well as failed) states

4. _Generate a lexer_ that makes use of the parsed output and the NFA
   functionality to provide a `yylex` routine as above.

My implementation is made up of four main modules:

- `parse.h`: parses Lex source file and treats the regexes simply as blocks of
  text (terminated by newlines)

- `thompson.h`: parses the regexes using a
  [grammar](https://github.com/akiarie/lex/blob/master/thompson.h) that
  describes simple regular expressions

- `automata.h`: builds, manipulates and simulates automata

- `gen.h`: generates lexers based on the output of the parser.

The `main` function simply reads in the input file and then invokes the
```C
struct lexer * 
parse(char *);
```
function from `parse.h` to obtain a `struct lexer` corresponding to the file.
It submits this to `gen.h`, using
```C
void
gen(FILE *, struct lexer *); /* simplified slightly */
```
with the output file being `lex.yy.c` (unless specified otherwise).

Because my goal was to get a simple implementation, I have used NFA simulation
rather than NFA-to-DFA conversion. Thus, all that the `gen` function above does
is hard-code the patterns it finds in the `struct lexer` provided to it,
together with outputting the _User subroutine_ section directly, in such a way
that when the `lex.yy.c` (otherwise named output) file is executed, it
immediately builds the total NFA using `thompson` and `automata`.

To facilitate this dependency, the `gen` function includes within an
`xxd`-generated copy of the source of the `thompson` and `automata` header and
source files, which is triggered in the Makefile using a script
`gen-preamble.sh` similar to this:
```bash
#!/bin/sh
cat thompson.h thompson.c automata.h automata.c \
	| xxd -name lex_gen_file -i -
```
The Makefile entry corresponding to this is
```make
# avails thompson and automata libraries
lex_gen.c: $(OBJECTS) gen-preamble.sh
	@./gen-preamble.sh > $@
```
Thus, the `gen.c` source file contains an `#include "lex_gen.c"`, meaning it has
access to the latest version of the `thompson` and `automata` libraries.

This whole process is summarised in the following diagram:
{{< figure src="./lex-diagram.png" >}}

## Things I learned

### C is almost perfect

One of the most interesting takeaways for me from this project has been the
wonderful experience I have had working in C. Of all the things that slowed me
down, at no point did I feel that if I were working in a higher-level language
(say in Go) that I would've moved faster. That realisation alone is
astounding given the fact that C is now past the half-century mark.

My main gripes with C are

- Lack of a module/package system leading to names like `foo_bar`, `foo_baz`,
  etc.

- Poor debugging support for simple errors (Surely runtime NULL dereferences
  could be traced by a good compiler in some debug mode? Are obscure segfaults
  the only way?)

- Lack of first-class type-support for functions.

I honestly cannot think of anything else to add to the above list. The language
is remarkably simple and predictable: in a word, it fits in your head. The more
I use it, the more I find myself relying on manpages and K&R rather than on
Stack Overflow, which points to its intuitiveness. Most modern languages require
large online communities to understand and use.
 
### Balancing top-down, bottom-up (and TDD)

I lost a lot of time by focusing too hard on the wrong details, and too
little on the right overall model. This kind of time-loss is sometimes
inescapable when the problem one is solving is novel. However, in this case, I
fully understood the theory from the beginning, so I would have spent my time
better if I began by fully writing out my high-level model in a diagram like the
one above.

When building a sophisticated program that involves several modules, the
question of whether to approach it top-down or bottom-up seems to turn on how
well one understands the problem. If it is perfectly, or nearly so, understood,
it makes sense to draw a rough sketch of the modules and then immediately begin
implementing them one-by-one. In fact, this is the case in which a test-driven
development model may be applicable (within reason and not in a slavish way).
Well-understood components, such as the regex-parsing and `automata` libraries,
are modules for which tests can be written in advance.

However, to the extent that one is solving a problem that is not well
understood, a top-down approach seem to be wisest. Going into this project,
although I knew what Lex was doing in theory, some things were not clear to me,
such as the fact that the `gen` library could generate code that relies on the
source of the `thompson` and `automata` libraries. If I had spent more time
diagramming and even writing header files in detail, with careful analysis I
would have saved myself countless hours (and indeed, days).

The moral of the story is that the right way to solve a complex problem is to
try and identify distinct centres of complexity within it, and to make these the
modules. If one is able to identify the centres accurately, then it is possible
to proceed in a bottom-up manner from this point onward. But until a clear model
emerges bottom-up development should be avoided.

One of the most time-consuming parts of my experience was in implementing the
`automata` library, yet I fully understood what it was supposed to do at a
high-level. The fact that its complexity was firmly bounded within itself as a
module meant that I was able to prevent this complexity from creeping into the
rest of the program.

### Lex is a great project but probably an unnecessary tool

Anyone who has written a few lexers can see that, although Lex incorporates many
interesting ideas (such as finite automata theory), it is overkill for almost
every use case conceivable, because lexers are actually fairly easy to
implement.

This notwithstanding, I would argue that Lex has amazing pedagogical value. It
is based on so many wonderful and beautiful ideas, and requires such a thorough
grasp of them, that implementing it is an excellent exercise for the mind. It is
also a great stepping stone, I hope, towards the next project, corresponding to
the chapter on parsing:
I will be back when, by God's grace, I have implemented Yacc.


[^def]: Lex implementations generally allow string literals here that have not
  been defined in the _Definitions_ section to match themselves, as well as
  regular definitions involving patterns defined in _Definitions_ within them.
  I have only included the former functionality in my implementation.
