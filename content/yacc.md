---
title: ‘A Terribly Interesting Compiler and the Covenant of Induction’
date: 2022-12-05T07:51:20+03:00
draft: true
---

> And God said, "Let there be lights in the expanse of the heavens to separate
> the day from the night. And let them be for signs and for seasons, and for
> days and years, and let them be lights in the expanse of the heavens to give
> light upon the earth." And it was so.  
> — Genesis 1:14–15

> And when the LORD smelled the pleasing aroma, the LORD said in his heart, "I
> will never again curse the ground because of man, for the intention of man’s
> heart is evil from his youth. Neither will I ever again strike down every
> living creature as I have done. While the earth remains, seedtime and harvest,
> cold and heat, summer and winter, day and night, shall not cease."  
> — Genesis 8:21–22

> The contrary of every matter of fact is still possible; because it can never
> imply a contradiction, and is conceived by the mind with the same facility and
> distinctness, as if ever so conformable to reality. That the sun will not rise
> to-morrow is no less intelligible a proposition, and implies no more
> contradiction than the affirmation, that it will rise. We should in vain,
> therefore, attempt to demonstrate its falsehood. Were it demonstratively
> false, it would imply a contradiction, and could never be distinctly conceived
> by the mind.  
> — Hume, _An Enqiury Concerning Human Understanding_, §4.21

Having [implemented Lex](/posts/lex), the natural challenge which presented
itself to me in reading chapter four of _The Dragon Book_ is implementing Yacc.
Yacc, as you may know, is a compiler-compiler, and as I thought about how I
would implement it, a beautiful idea came into my mind: _What prevents Yacc from
compiling itself?_ In this article I will attempt to describe this idea in
detail, my success (and failures!) in bringing it to life, and its connection to
the goodness of the gracious God who gives us sunshine every morning.
