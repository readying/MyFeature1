#!/usr/bin/perl

#
# AVEC 2013 audio feature batch extraction script
# (c) 2013 by Florian Eyben
#

use File::Basename;
my @instances = glob("../Audio/*.mp4");

for (my $i = 0; $i <= $#instances; $i++) {
  my $inst = $instances[$i];
  $inst = basename($inst);
  $inst =~ s/_audio\.mp4$//;
  print "== Extracting features for instance \"$inst\" ==\n";
  my $ret = system("perl extractor_script.pl \"$inst\"");
  print "\n\n";
  if ($ret) { exit -1; }
}

