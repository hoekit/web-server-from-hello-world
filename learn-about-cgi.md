<small>[Home](README.md) / CGI</small>

----
# About CGI

- Common gateway interface (CGI)
- A standard way for web servers to handle web forms using CGI scripts
- A CGI script:
    1. Reads the form input
    2. Generate a response based on given input data
    3. Write response to STDOUT
- To develop a CGI-script, just write a program that does the above and
  place it in the `cgi-bin` directory.
- CGI is has performance, security issues but it is really simple to
  develop.

**Sample CGI Script**

Call the script below via `http://example.com/printenv.pl`
```
    #!/usr/bin/env perl

    =head1 DESCRIPTION

    printenv — a CGI program that just prints its environment

    =cut
    print "Content-Type: text/plain\n\n";

    for my $var ( sort keys %ENV ) {
        printf "%s=\"%s\"\n", $var, $ENV{$var};
    }
```

# References

- [James Marshal's CGI Made Really Easy](https://www.jmarshall.com/easy/cgi/)
- [CGI on Wikipedia](https://en.wikipedia.org/wiki/Common_Gateway_Interface)
- [Issues with CGI](https://medium.com/adobetech/2017-will-be-the-year-of-the-cgi-bin-err-serverless-f5d99671bc99)


----
```C
/*
# This: learn-about-cgi.md
# Prev: learn-about-sockets.md
# Next: hello-server.c
*/
```
