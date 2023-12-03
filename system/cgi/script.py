#!/usr/bin/env python3
import os, sys

request_method = os.environ.get("REQUEST_METHOD", "N/A")
query_string = os.environ.get("QUERY_STRING")
user_agent = os.environ.get("HTTP_USER_AGENT", "N/A")
cookie = os.environ.get("HTTP_COOKIE")
ip_addr = os.environ.get("REMOTE_ADDR")
file_path = os.environ.get("OUTPUT_FILE", "")

sys.stderr.write("\nPython: processing GET request.\n")

html_content = """
<!DOCTYPE html>
<html>
<head>
	<title>Python CGI Example</title>
	<link rel="stylesheet" type="text/css" href="/styles.css"/>
</head>
<body>
	<h1>Python CGI:<br>HTTP Request Data</h1>
	<p>Here are some values from the HTTP request:</p>
	<ul>
		<li>Request Method: {}</li>
		<li>Query String: {}</li>
		<li>User-Agent: {}</li>
		<li>Cookie: {}</li>
		<li>Your IP: {}</li>
	</ul>
	<br>
	<p>
		"Query string" is empty? Try appending "?key=value" to the URL!
		<br>
		(wont have any other effect, though.)
	</p>

</body>
</html>
"""

with open(file_path, 'w') as file:
	file.write(html_content.format(request_method, query_string, user_agent, cookie, ip_addr))

sys.stderr.write("Python: request processed.\n")
