<small>[Home](README.md) / Sending Images</small>

----
# Sending Images

- To send an image e.g. novice.jpg, send a header
    - `Content-Type: image/jpeg`

- `image/jpeg` is an example of a MIME type

- MDN has a [good explanation](https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types) of what MIME types

- The fallback MIME type for binary data is: `application/octet-stream`

- Also need to send the size of message body in bytes
    - `Content-Length: <length>`

- The content itself is an array of bytes


**Example**

- A sample HTTP response:

        HTTP/1.0 200 OK
        Content-Type: image/jpeg
        Content-Length: <length>

        <content>

