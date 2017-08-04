#!/usr/bin/perl


# ask the system for the date
$date = `date`;
$files = "*.cpp *.h *.pl fland *.fml";

$usbFolder = "/media/usbdisk/flandBackups";

system "rm -r html/";

$date =~ s/\s/-/g;
$date =~ s/\:/_/g;

print "Backing up fland into $date.\n";

mkdir "../$date";
mkdir "../$date/fml";

$result = `cp -r $files ../$date/`;
$result .= `cp -r fml/*.fml ../$date/fml/`;

print "Finished backing up to parent folder.\n";

print "Checking for usbdisk at $usbFolder/...\n";

if( -e "$usbFolder/" )
{
	print "usbdisk found. Creating folder '$usbFolder/$date' ...\n";
	mkdir "$usbFolder/$date";

	`cp -r ../$date/* $usbFolder/$date/`;
	print "Done backing up to $usbFolder/$date\n";
}
else
{
	print "no usbdisk found.\n";
}

mkdir "html";
