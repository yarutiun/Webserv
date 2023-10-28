#!/usr/bin/php-cgi

<?php

$stderr = fopen('php://stderr', 'w');

$request_method = getenv('REQUEST_METHOD');
$outfile_path = getenv('OUTPUT_FILE');

if ($request_method === 'GET')
{
	fwrite($stderr, "PHP: processing GET request.\n");
	
	// get query string from env
	$query_string = getenv('QUERY_STRING');
	
	// parse the query string into key value pairs
	parse_str($query_string, $query_vars);
	
	// extract value for specific keys into vars
	$first_name = $query_vars['first_name'] ?? '';
	$last_name = $query_vars['last_name'] ?? '';
}

if ($request_method === 'POST')
{
	fwrite($stderr, "PHP: processing POST request.\n");

	$infile_path = getenv('INPUT_FILE');

	$in_file = fopen($infile_path, 'r');
	
	$input_data = fread($in_file, filesize($infile_path));
	$input_data = trim($input_data);
	
	// Close the file handle
	fclose($in_file);

	// Parse the input data as query string (key=value) pairs
	parse_str($input_data, $post_vars);

	// Extract values for specific keys into variables
	$first_name = $post_vars['first_name'] ?? '';
	$last_name = $post_vars['last_name'] ?? '';
}

/* 
// for reading from STDIN
if ($request_method === 'POST')
{
	// Open stdin as a file handle
	$stdin = fopen('php://stdin', 'r');
	
	// Read input from stdin
	$input_data = fgets($stdin);
	$input_data = trim($input_data);
	
	// Close the file handle
	fclose($stdin);

	// Parse the input data as query string (key=value) pairs
	parse_str($input_data, $post_vars);

	// Extract values for specific keys into variables
	$first_name = $post_vars['first_name'] ?? '';
	$last_name = $post_vars['last_name'] ?? '';
}
*/

$html_content = <<<HTML
<!DOCTYPE html>
<html>

<head>
	<title>PHP CGI Example</title>
	<link rel="stylesheet" type="text/css" href="/styles.css"/>
</head>

<body>
	<h1>PHP CGI:<br>$request_method</h1>
	<p>Hello $first_name $last_name!</p>
</body>

</html>
HTML;

file_put_contents($outfile_path, $html_content);

fwrite($stderr, "PHP: request processed.\n");
fclose($stderr);

?>
