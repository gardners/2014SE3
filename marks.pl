#!/usr/bin/env perl

use strict;
use warnings;
use FindBin '$Bin';

# List of marks for a given number (array index) of completed checkpoints
my @marks = (
	'0,F',
	'10,F',
	'20,F',
	'30,F',
	'40,F',
	'50,P',
	'60,P',
	'65,CR',
	'74,CR',
	'80,DN',
	'90,HD',
);

# List of files containing FANs of people who have completed checkpoints,
# separated by newlines
my @files = map { glob "$Bin/grades/$_/prac*checkpoints.txt"; }
              qw/assignment1 assignment2/;

# Map of FANs to number of checkpoints completed
my %checkpoints;

for my $file (@files) {
	open my $fh, '<', $file;
	while (my $line = $fh->getline) {
		chomp $line;
		$checkpoints{lc $line}++;
	}
}

for my $fan (sort keys %checkpoints) {
	my $mark = $marks[$checkpoints{$fan}];
	print "$fan,$mark\n";
}

__END__

=pod

=head1 AUTHOR

Cameron Thornton <thor0250@flinders.edu.au>

=cut
