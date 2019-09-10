BEGIN { 
	FS = "[ :]"
	i = 0
	i1 = 0
	sum1 = 0
	sum2 = 0
	sum3 = 0
	sum_str = 0
}


{ 	
	if ($2 ~ /^Savage$/) {
		arr_sav[i] = sprintf("%s %s: %s %s", $1, $2, $3, $4)
		i = i + 1
	} 

	if ($1 ~ /^Chet$/) {
		chet = sprintf("$%s, $%s, $%s", $5, $6, $7)
	}

	if ($5 ~ /^250$/) {
		arr_250[i1] = sprintf("%s %s", $1, $2)
		i1 = i1 + 1
	}

	sum_str = sum_str + 1
	sum1 = sum1 + $5
	sum2 = sum2 + $6
	sum3 = sum3 + $7
		
}


END {
	print ("Полные имена и номера телефонов всех сотрудников по фамилии Savage:")
	for (j in arr_sav) {
		print (arr_sav[j])
	}

	print ("\nДенежные вклады сотрудника по имени Chet:")
	print (chet)

	print ("\nCотрудники, денежные вклады которых в первом месяце составили 250$:")
	for (j in arr_250) {
		print (arr_250[j])
	}

	print ("\nCумма вкладов за каждый месяц:")
	printf("%7s %10s %10s\n", "месяц 1", "месяц 2", "месяц 3")
	printf("%7d %10d %10d\n", sum1, sum2, sum3)
	
	print ("\nCредний вклад за каждый месяц:")
	printf("%7s %10s %10s\n", "месяц 1", "месяц 2", "месяц 3")
	printf ("%7.2f %10.2f %10.2f\n", sum1/sum_str, sum2/sum_str, sum3/sum_str)

	print("\nТекущее время:")
	print strftime("Time = %m/%d/%Y %H:%M:%S", systime())

	print("\nВывод команды ls:")
	print(system( "ls"))
}
