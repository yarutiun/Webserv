#!/usr/bin/env python
import cgi

print("<!DOCTYPE html>")


form = cgi.FieldStorage()
name = form.getvalue("name")


print("<html>")
print("<head><title>CGI Example</title></head>")
print("<body>")
if name:
	print(f"<h1>Hello, {name}!</h1>")
else:
	print("<h1>No name provided.</h1>")
print("</body>")
print("</html>")