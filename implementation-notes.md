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


**ERROR binding socket to address: Address already in use**

Problem: Restarting the server at the same port has errors

    $ ./server 9001
    ERROR on binding: Address already in use

Cause: Client closed socket after server

    The server has closed the socket but before the client did so it is
    in a TIME_WAIT state.

Solution: Use SO_REUSEADDR

    This solution allows bind to use the socket if it is in a TIME_WAIT state.
    See SO_REUSEADDR in:
    - https://www.man7.org/linux/man-pages/man7/socket.7.html
    See setsockopt() in:
    - https://www.man7.org/linux/man-pages/man2/setsockopt.2.html

    Use SO_REUSEADDR like so:

    #include <sys/types.h>          /* See NOTES */
    #include <sys/socket.h>
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
            &(int){1}, sizeof(int)) < 0)
        error("setsockopt(SO_REUSEADDR) failed");

