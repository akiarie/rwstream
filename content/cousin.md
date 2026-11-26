---
title: ‘A Generalisation of Cousin’
date: 2022-12-24T12:15:00+03:00
draft: false
---

The term _cousin_ has several meanings. In the first place it is used for the
relationship between children of siblings: the child of your mother's brother is
your cousin. By analogy, we speak of _second cousins_, referring to the
relationship between children of cousins, and of _third cousins_ for that
between children of second-cousins, and so on.

Thus we have a definition for the term _nth-cousin_ when n ≥ 1. This raises the
question: can we speak of a zeroth-cousin, i.e. a cousin where n = 0? But to
answer this question we must first answer: _What is n?_

This brief article offers an interesting answer to these questions; an answer
which, if nothing else, I find very satisfying. If you have never thought about
these questions, I would encourage you to stop reading at this point and come up
with your own answers first, because

> Trying to solve [a problem] on your own seems the only way in which you can
> assess how difficult the problem is; it gives you the opportunity to compare
> your own solution with mine; and it may give you the satisfaction of having
> discovered yourself a solution that is superior to mine.  
> Dijkstra, _A Discipline of Programming_.

<!--more-->

## What is n?

Our English word _cousin_ comes to us (courtesy of the French) from the Latin
_cōnsobrīnus_ (and _cōnsobrīna_, but I will focus on the masculine forms to save
time), which is derived from _sobrīnus_, referring to a maternal first-cousin.
Clearly, the etymology here, however interesting, does not answer our question.

The question _What is n?_ came to my mind when I noticed that we were counting
upwards, but not from zero. Somehow in the history of languages, whenever the
terms _first_ and _second-cousin_ were first introduced, whether deliberately or
by sheer instinct, someone once knew that the relationship between children of
cousins should be assigned the number 2 rather than the number 1. They began
counting _cousinhood_ from 1, when they could've began at 0.

It is easy to see why this was done. Intuitively one feels that the word
_cousin_ implies a kind of distance in the relationship. The _n_ in _nth-cousin_
really tells us how distant the two families are: how many generations one has
to go up the family tree before the relationship is one of siblinghood. Thus a
first cousin is a relative with whom the minimum distance to sibling ancestors
is 1.

By now the answer to the first question is obvious: your zeroth-cousin is your
brother or your sister. But I think saying that _n_ tells us how many
generations we are from siblinghood is still an imperfect definition. After all,
what is so special about siblings? What do they share? The answer is again
obvious: they share parents. We shall take this as our definition:

> An _nth-cousin_ is a relative with whom one has to travel _n+1_ generations
> up the family tree before encountering a common ancestor.

But we cannot stop here: mathematics has been called "the science of
abstraction" and this definition is now ripe for generalisation. Our definition
is really only applicable if n ≥ 0. What if _n_ is negative? Can we define
_negative-cousinhood_?

## Negative Cousins

The answer to this question (as well as the question itself, incidentally) came
into my mind last night, while I was speaking with my wife. Interestingly, as I
have Googled around, I found a similar definition [here](https://qr.ae/prho7J)
to what I will propose now.

![Man asks babe to become his negative-first-cousin](/images/negative-cousin-proposal.jpeg)

Negative numbers are the same as positive ones, except that they denote
quantities that increase in the opposite direction. So the correct way to speak
of a negative cousin is to think of one with whom one moves _down_ the family
tree towards a common _descendant_. Thus _-1st_-cousins are spouses (or
technically, people who share a child). _-2nd_-cousinhood is the relationship
that sustains between the parents of a couple: between the bride's and
bridegroom's parents. (Strictly speaking this definition would only apply after
a child is born to the couple, but you get the idea!)

I like this definition because it gives us new and entertaining words for
relationships that already exist in the family. It shows that the nuclear family
is the nexus in the family tree. Streams flow into it and out of it in a kind of
symmetry that is beautiful.

Moreover, if we introduce the "removed" concept that is sometimes used (e.g.
your first cousin's child is your first cousin _once removed_) we get the
delightful expression that your parents-in-law are your
_-2nd-cousins once removed_. If that doesn't make you smile then nothing
I say will ever make you smile.

But if one wants a better term than _negative cousin_, my suggestion is we use
_coprogenitor_. Thus your spouse is your _first coprogrenitor_, and
your parents-in-law become your _second coprogrenitors once removed_.

__PS: I added the above meme on 23/05/24.__
