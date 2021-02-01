# Cosmopolitan

[Cosmopolitan libc](https://justine.lol/cosmopolitan/index.html) makes C
a build-once run-anywhere language, like Java, except it doesn't need an
interpreter or virtual machine. Instead, it reconfigures stock GCC to
output a POSIX-approved polyglot format that runs natively on Linux +
Mac + Windows + FreeBSD + OpenBSD + BIOS with the best possible
performance and the tiniest footprint imaginable.

-----

This is a restructuring of Justine’s Cosmopolitan repository to maintain
compliance with ADPs 1 and 4. It provides no functional changes compared to
that upstream, and strives to maintain an opt-out hermetically-sealed
build approach using Slick.

## References & additional reading

1. Aquefir. “ADP 1.” Project repository filesystem schema.
<https://aquefir.co/adp1>
2. Aquefir. “ADP 4.” Slick Makefiles. <https://aquefir.co/adp4>
3. Justine Tunney. “Cosmopolitan”. Cosmopolitan C library.
<https://github.com/jart/cosmopolitan>
