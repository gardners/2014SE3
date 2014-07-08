#!/usr/bin/env perl
use strict;
use warnings;

my %checkpoints;
my @grades = (
	"0,F",
	"10,F",
	"20,F",
	"30,F",
	"40,F",
	"50,P",
	"60,P",
	"65,CR",
	"74,CR",
	"80,DN",
	"90,HD",
        "DUPLICATES",
        "DUPLICATES",
        "DUPLICATES",
        "DUPLICATES",
        "DUPLICATES",
        "DUPLICATES",
        "DUPLICATES",
        "DUPLICATES",
        "DUPLICATES",
        "DUPLICATES",
        "DUPLICATES",
        "DUPLICATES",
        "DUPLICATES",
        "DUPLICATES",
);

for my $dir (qw/assignment1 assignment2/) {
	while (glob "$dir/prac*checkpoints.txt") {
                `sort -u $_ > "temp.txt"`;
		open(my $file, "<", "temp.txt");
		while (my $fan = <$file>) {
			chomp $fan;
			$checkpoints{lc $fan}++;
		}
	}
}

for my $fan (sort keys %checkpoints) {
	my $grade = $grades[$checkpoints{$fan}];
	print "$fan,$grade\n";
}
