PROGRESS.md is a running LLM-generated summary of where we are.

In C observe the following rules:

- K&R style generally, but put types of functions on separate lines

    int
    foo()

  not 

    int foo()


- The above applies also to forward declarations.

- Forward declare called functions instead of putting them above their callers.
  If a function calls two (as-yet-undefined) functions put them in forward
  declarations in the order that they're called.

- Use only C89 stuff. This doesn't mean that all declarations must be at the
  beginning, but that there should be no declarations in the middle. I.e.
  there's no reason to avoid a series of declaration-inits. 
