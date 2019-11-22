#!/usr/bin/perl -T
use strict;
use warnings qw(FATAL all);
use IO::Socket::UNIX;

my $SOCKET_PATH = '/tmp/socket';

my $client = IO::Socket::UNIX->new(
        Type => SOCK_STREAM(),
	Peer => $SOCKET_PATH,) or die "Не удалось приконнектиться к серверу\n";

while(<$client>) {
	print("$_");
}

close($client);
