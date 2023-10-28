#!/usr/bin/perl
use warnings;
use CGI;

my $query_string = $ENV{'QUERY_STRING'};  # Get the entire query string

my $name;  # Declare the $name variable

# while (1) {}

# sleep(1);

# Parse the query string to extract the 'name' parameter
if ($query_string =~ /name=([^&]*)/) {
    $name = $1;  # Store the 'name' parameter value in $name
}

print "<!DOCTYPE html>\n";
print "<html>\n";
print "<head><title>CGI Example</title></head>\n";
print "<body>\n";
if ($name) {
    print "<h1>Hello, $name!</h1>\n";
} else {
    print "<h1>No name provided.</h1>\n";
}
print "</body>\n";
print "</html>\n";
