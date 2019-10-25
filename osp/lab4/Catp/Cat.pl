#/usr/bin/perl

use Getopt::Long;

# флаги FLAG_NUMBERS -- вывод номеров строк на каждой строке
#       FLAG_NUMBER_B -- вывод номеров строк, пропуская пустые строки
use constant {
    FLAG_NUMBER => 1,
    FLAG_END_OF_LINE => 2,
};

# получение ключей из argv и создания маски с флагами $flags
my $flags = 0;
my $key_n, $key_e;		
GetOptions ('n' => \$key_n,
            'e' => \$key_e);


$flags = $key_n ? ($flags | FLAG_NUMBER) : $flags;
$flags = $key_e ? ($flags | FLAG_END_OF_LINE) : $flags;

print($flags);

#if (@ARGV == 0) {
#    while(true) {
#        my $str = <STDIN>;
#        print($str);
#    }
#}

