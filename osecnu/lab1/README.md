# README

This is a shell project for OS lab in ECNU.

It supports following function:

- regular commands

  (`cd`, `exit`, `ls`, `pwd`, ..., everything like normal shell, zsh and bash)

- history n(read the previous n command line including "history n")

**WARNING**: You have to modify your history path on `utils/include/def.h`

- mytop(like `top`Only support on **MINIX**)
- Pipeline
- Redirection(<, >, >>)
- Background(command &), no background process group since MINIX does not implement it

---

My code should be organized with `Makefile`, 

To build this project on minix(or other unix platform, whether x86 or arm), type:

```
> make myshell
```

To run shell, you need to type:

```
> $YOUTPATH/lab1/main
```

or on the same dir as Makefile:

```
> ./main
```

To enable debug mode:

```
> make debug
```

To clean the object file:

```
> make clean
```

---

If you want to quit my shell, just type `^C`or `exit`, it will handle it properly :)

