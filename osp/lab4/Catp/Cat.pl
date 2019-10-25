#/usr/bin/perl

use Getopt::Long;
use strict;

# флаги FLAG_NUMBERS -- вывод номеров строк на каждой строке
#       FLAG_NUMBER_B -- вывод номеров строк, пропуская пустые строки
#       FLAG_END_OF_LINE -- вывод с добавлением $ в конце каждой строки
use constant {
    FLAG_NUMBER => 1,
    FLAG_NUMBER_B => 2,
    FLAG_END_OF_LINE => 4,
};

# получение ключей из argv и создания маски с флагами $flags
my $flags = 0;
my $key_n;
my $key_e;
my $key_b;		
GetOptions ('n' => \$key_n,
            'e' => \$key_e,
            'b' => \$key_b);

$flags = $key_n ? ($flags | FLAG_NUMBER) : $flags;
$flags = $key_e ? ($flags | FLAG_END_OF_LINE) : $flags;
$flags = $key_b ? ($flags | FLAG_NUMBER_B) : $flags;

# cat без файлов (из stdin)
if (@ARGV == 0) {
    my $i = 1;
    while(1) {
        my $str = <STDIN>;
        if ($flags & FLAG_NUMBER) {
            print("$i ");
            $i = $i + 1;
        }
        if (($flags & FLAG_NUMBER_B) && $str != "\n" ) {
            print("$i ");
            $i = $i + 1;
        }
        print($str);
        if ($flags & FLAG_END_OF_LINE) {
            print("\$");
        }
    }
}

foreach my $arg (@ARGV) {
    print($arg);
    print("\n");
}



