#!/usr/bin/perl

use strict;
use warnings qw(FATAL all);
use threads;
use IO::Socket::INET;

use constant USAGE => "Используйте ./server port\n";
my ($port) = shift =~ /(\d+)/ or die USAGE;

my $server = IO::Socket::INET->new(
  LocalPort => $port, 
  Proto => 'tcp', 
  Listen => SOMAXCONN					# максимальный размер буфера для клиентов
) or die "Невозможно присоедениться к серверу: $!\n";

print "Listening on $port\n";

while (my $client = $server->accept()) {
  threads->create(\&process_request, $client)->detach();
}

sub process_request {
  my $client = shift;

  my $cltaddr = $client->peerhost;
  my $cltport = $client->peerport;
  print "Клиент: $cltaddr:$cltport\n";

  my $request = "";
  while (<$client>) {
    $request .= $_;
    last if $request =~ /\n\n$/;
  }

  my @dirs = split /\n/, $request;

  for my $dir (@dirs) {
    if (opendir my $dirent, $dir) {
      my @files = readdir $dirent;
      closedir $dirent;

      print $client "$dir:\n" . join("\n", @files) . "\n\n";
    }
    else {
      print $client "Невозможно открыть $dir: $!\n\n";
    }
  }

  close $client;
}
