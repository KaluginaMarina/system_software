#!/usr/bin/perl

use strict;
use warnings qw(FATAL all);

my $filename = shift @ARGV;
die "Используйте ./unnamed_pipe.pl FILENAME" if !$filename;

open my $fd, '<', $filename or die "Невозможно открыть $filename: $!\n";
pipe my $pipefd_rd, my $pipefd_wr or die "Невозможно создать pipe: $!\n";

my $pid = fork();
if (!$pid) {
	# Child process
	close $pipefd_wr or die "Невозможно заблокировать родительский pipe: $!\n";
	open STDIN, '<&', $pipefd_rd or die "Невозможно заменить STDIN: $!\n"; 

	$ENV{'PATH'} = '/usr/bin';
	exec "wc -c" or die "Невозможно исполнить wc: $!\n";
} else {
	# Parent process
	close $pipefd_rd or die "Невозможно открыть дочерний подпоток: $!\n";

	my $buf = "";
	while (read $fd, $buf, 2) {
		print $pipefd_wr substr($buf, 1, 1);
	}
}
