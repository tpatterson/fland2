#!/usr/bin/perl
# this script sticks a bunch of #include statements into
# main.cpp of fland to include all the other cpp files.
# It puts these commands between the comments // <START_CPP_INCLUES>
# and // <END_CPP_INCLUDES>
# It just creates a bunch of #include ".cpp" statements for
# every .cpp file in the current directory.

$MAKEFILE = "Makefile";

# create the include string
my $srcsString = "";
my $objsString = "";
opendir( DIR, "./" );
while( $file = readdir( DIR ) )
{
	if( $file =~ /includecpps/ ){ next; }
	if( ( $file =~ /\.cpp/ && $file !~ /\.cpp./ ) || ( $file =~ /\.h/ && $file !~ /\.h./ ) )
	{
		$srcsString .= "$file ";

		if( $file =~ /.cpp/ )
		{
			$file =~ s/\.cpp/\.o/;
			$objsString .= "$file ";
		}
	}
}
closedir( DIR );


# read in main.cpp
$rewrite = 0;
$fullFile = '';
open( MAIN, "$MAKEFILE" ) || die "couldn't open $MAKEFILE " . `pwd`;
while( my $line = <MAIN> )
{
	my $beginLine = $line;
	$line =~ s/(SRCS = ).*/$1$srcsString/;
	$line =~ s/(OBJS = ).*/$1$objsString/;
	$fullFile .= $line;
	if( $beginLine ne $line ){ $rewrite = 1; }
}
close( MAIN );


# rewrite main.cpp
if( $rewrite ) #only rewrite it if it changed.
{
	print "source changes, updating makefile...\n";
	open( MAIN, ">$MAKEFILE" );
	print MAIN $fullFile;
	close( MAIN );
}
