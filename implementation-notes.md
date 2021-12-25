<small>[Home](README.md) / Implementation Notes</small>

----
# Implementation Notes

**const keyword**

- What does `const char *str` mean?

- The `const` keyword changes the type so that it cannot be assigned to
  (except when initialization) effectively making the value read-only.

- So `const char *str` means that str is a read-only value and is
  typically used in function parameters to prevent inadvertent changes
  to variables


**Using sizeof**

- What does `sizeof(var)` mean

- Use `sizeof` to determine the number of bytes consumed by `var` in
  memory

- It is useful to fill a variable with zeros before using it



**Define array**

- Define arrays using braces

    `char *stooges[3] = { "Larry", "Curly", "Moe" };`


**String functions reference**

- Page 264 of **C in a Nutshell**
