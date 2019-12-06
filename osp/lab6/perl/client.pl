#!/usr/bin/perl -T

use strict;
use warnings qw(FATAL all);
use IO::Socket::INET;

use constant USAGE => "Используйте ./client host port [files...]\n";

my ($host) = shift =~ /([A-Za-z0-9.]+)/ or die USAGE;
my ($port) = shift =~ /(\d+)/ or die USAGE;

scalar @ARGV or die USAGE;
my $request = join("\n", @ARGV) . "\n\n";

my $clt = new IO::Socket::INET(
  PeerHost => $host, PeerPort => $port, Proto => 'tcp'
) or die "Невозможно приконнектиться к $host:$port: $!\n";

$clt->send($request);

while (<$clt>) {
  print;
}
