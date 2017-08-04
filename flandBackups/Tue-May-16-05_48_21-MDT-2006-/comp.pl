#!/usr/bin/perl


for( $i=0; $i<200; $i++) { print "\n"; }

$file = shift;
print system "g++ -pthread -lglut $file -ofland";
