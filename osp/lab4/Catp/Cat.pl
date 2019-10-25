#/usr/bin/perl

use Getopt::Long;
use strict;
use warnings;

# флаги FLAG_NUMBERS -- вывод номеров строк на каждой строке
#       FLAG_NUMBER_B -- вывод номеров строк, пропуская пустые строки
#       FLAG_END_OF_LINE -- вывод с добавлением $ в конце каждой строки
#       FLAG_HELP -- вывод справки
use constant {
    FLAG_NUMBER => 1,
    FLAG_NUMBER_B => 2,
    FLAG_END_OF_LINE => 4,
    FLAG_HELP => 8,
};

# получение ключей из argv и создания маски с флагами $flags
my $flags = 0;
my $key_n;
my $key_e;
my $key_b;
my $key_h;		
GetOptions ('n' => \$key_n,
            'e' => \$key_e,
            'b' => \$key_b,
            'h' => \$key_h);

$flags = $key_n ? ($flags | FLAG_NUMBER) : $flags;
$flags = $key_e ? ($flags | FLAG_END_OF_LINE) : $flags;
$flags = $key_b ? ($flags | FLAG_NUMBER_B) : $flags;
$flags = $key_h ? ($flags | FLAG_HELP) : $flags;

if($key_b && $key_n) {
    die "Ключи n и b не могут использоваться вместе";
}

# вывод справки
if ($flags & FLAG_HELP) {
    open(my $fh, '<:encoding(UTF-8)', "help.txt");
    while (my $row = <$fh>) {
        chomp $row;
        print("$row\n");
    }
    close($fh);
}

# cat без файлов (из stdin)
if (@ARGV == 0) {
    my $i = 1;
    while(<>) {
        my $str = <STDIN>;
        if ($flags & FLAG_NUMBER) {
            print("$i ");
            $i = $i + 1;
        }
        if (($flags & FLAG_NUMBER_B) && $str ne "\n" ) {
            print("$i ");
            $i = $i + 1;
        }
        if ($flags & FLAG_END_OF_LINE) {
            $str =~ s/\n/\$\n/;
        }
        print($str);
    }
}

my $i = 1;
# cat для каждого файла
foreach my $arg (@ARGV) {
    open(my $f, '<:encoding(UTF-8)', $arg)
        or die "Could not open file '$arg' $!";
    while (my $row = <$f>) {
        chomp $row;
        if ($flags & FLAG_NUMBER) {
            print("$i ");
            $i = $i + 1;
        }
        if (($flags & FLAG_NUMBER_B) && $row ne "" ) {
            print("$i ");
            $i = $i + 1;
        }
        print("$row");
        if ($flags & FLAG_END_OF_LINE) {
            print("\$")
        }
        print("\n");
    }
    close($f);
}
