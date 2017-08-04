#!/usr/bin/perl
# this script sticks a bunch of #include statements into
# main.cpp of fland to include all the other cpp files.
# It puts these commands between the comments // <START_CPP_INCLUES>
# and // <END_CPP_INCLUDES>
# It just creates a bunch of #include ".cpp" statements for
# every .cpp file in the current directory.

$MAIN_CPP = "includecpps.cpp";

# create the include string
my $includeString = "";
opendir( DIR, "./" );
while( $file = readdir( DIR ) )
{
	if( $file =~ /\.cpp/ && $file !~ /\.cpp./ && $file !~ /includecpps/ )
	{
		$includeString .= "#include \"$file\"\n";
	}
}
closedir( DIR );

# read in main.cpp
open( MAIN, "$MAIN_CPP" );
my @lines = <MAIN>;
close( MAIN );
my $mainLines = join( '', @lines );

# save the file before processing
$before = $mainLines;

# process main.cpp
$mainLines = $includeString;

# rewrite main.cpp
if( $before ne $mainLines ) #only rewrite it if it changed.
{
	open( MAIN, ">$MAIN_CPP" );
	print MAIN $mainLines;
	close( MAIN );
}
