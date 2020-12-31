#!/bin/bash

OLD_LC_ALL="$LC_ALL"

LC_ALL=C					#in case an error pops up with "cat /dev/urandom ........"


#checking input parameters

echo number of input parameters=$#		# number of arguments given to the script

if [ $# -ne 4 ]; then	

	echo "Error input params (need 4)"
	exit 3		#generic error
fi	


start_dir_name=$1

num_of_files=$2

num_of_dirs=$3

levels=$4



if [ ! -e "$start_dir_name" ]; then	#if dir_name doesn't  exist

	mkdir "$start_dir_name"

else
	if [ ! -d "$start_dir_name" ]; then	#if dir_name exists but isn't a dir => remove file, then  make dir
		
		rm "$start_dir_name"
		mkdir -m 0777 "$start_dir_name"
	fi
fi





if [ "$num_of_files" -lt 1 ]; then

	echo "num_of_files must be >= 1"
	exit 3		#generic error
fi




if [ "$num_of_dirs" -lt 1 ]; then

	echo "num_of_dirs must be >= 1"
	exit 3		#generic error
fi



if [ "$levels" -lt 1 ]; then

	echo "levels must be >= 1"
	exit 3		#generic error
fi



#Create Dir Names

levels_temp="$levels"			# 2 temporary variables

dir_temp="$start_dir_name"

dir_name_array=( "$dir_temp" )					# an array with the relative paths of the directories


#length=$(cat dev/urandom | tr -dc '1-8' | head -n 1 | head -c 1)
	
#dir_names=$(cat dev/urandom | tr -dc 'a-zA-Z0-9' | head -n 1 | head -c 1)


for(( i = 0; i < num_of_dirs; i++)) ; do

	length=$(cat /dev/urandom | tr -dc '1-8'| fold -w 256  | head -n 1 | head -c 1)
	
	echo "$length"
	
	dir_name=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w "$length" | head -n 1 )
	
	
	
	if [ "$levels_temp" -ne 0 ]; then		# levels_temp != 0
	
		levels_temp=$((levels_temp-1))		#reduce the counter by 1
		
		dir_temp+="/$dir_name"
		
		echo "$dir_temp"
		
		mkdir -m 777 "$dir_temp"		#create a dir with rwx permissions
		
		dir_name_array+=("$dir_temp")		# add the new dir (relative path) into the array		
				
	else	#if levels_temp = 0
	
		levels_temp="$levels"	#restart the counter
		
		dir_temp="$start_dir_name"	# restart the temp_dir variable
	
		#doing the same thing as above
		
		levels_temp=$((levels_temp-1))		#reduce the counter by 1
		
		dir_temp+="/$dir_name"
		
		echo "$dir_temp"
		
		mkdir -m 0777 "$dir_temp"		#create a dir with rwx permissions
		
		dir_name_array+=("$dir_temp")		# add the new dir (relative path) into the array		
	fi
done





echo "Finished creating all the Dirs"
echo

# Create all the dirs ===> now we create the files

levels_temp="$levels"			# 2 temporary variables

#file_temp="$start_dir_name"

file_name_array=( )					# an array with the relative paths of the files

index=0

echo "Creating empty files"

for(( i = 0; i < num_of_files; i++)); do

	while :		# infinite loop
	do
		length=$(cat /dev/urandom | tr -dc '1-8'| fold -w 256  | head -n 1 | head -c 1)
	
		echo "$length"
	
		file_name=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w "$length" | head -n 1 )
		
		file_temp1="${dir_name_array[index]}"
		
		file_temp="$file_temp1/$file_name"	#relative path of new file
		
		echo "$file_temp"		
				
		if [ ! -e "$file_temp" ] ; then	 
			break							#break only if the filename doesn't exist
		fi	
		
		echo "filename $file_temp exists ==> continuing"
	done	
	
	#if we are here ==> file_temp doesn't exist
	
	touch "$file_temp"		#create empty file
	chmod 666 "$file_temp"	#change permissions of file
	
	file_name_array+=( "$file_temp" )	# add it to the array
	
	index=$((index + 1))		#increment index by 1
	
	if [ "$index" -eq "${#dir_name_array[@]}" ]; then		#if we passed the last indexed item ===>
		
		index=0		#restart index
	fi
		
done


echo "Finished creating files"
echo




for file in ${file_name_array[@]}; do		#for every file in the array

	digit_1=$(cat /dev/urandom | tr -dc '1-8'| fold -w 256  | head -n 1 | head -c 1)
	digit_2=$(cat /dev/urandom | tr -dc '1-8'| fold -w 256  | head -n 1 | head -c 1)
	digit_3=$(cat /dev/urandom | tr -dc '1-8'| fold -w 256  | head -n 1 | head -c 1)
	
	number="$digit_1$digit_2$digit_3" 			# concat the 3 digits together
	
	number=$(( number % 128 )) 		# range is 0-127 

	number=$((number+1))		# range is 1-128
	
	number=$((number * 1024))	# range is 1kB-128kB
	
	echo "$number"

	echo "$file"
	
	string=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w "$number" | head -n 1 )
	
	echo "$string" > "$file"	#input to file

done




# 2nd solution with dd

# Created all of the files ====> now we fill them (INCOMPLETE)

#for file in ${file_name_array[@]}; do		#for every file in the array

#	digit_1=$(cat /dev/urandom | tr -dc '1-8'| fold -w 256  | head -n 1 | head -c 1)
#	digit_2=$(cat /dev/urandom | tr -dc '1-8'| fold -w 256  | head -n 1 | head -c 1)
#	digit_3=$(cat /dev/urandom | tr -dc '1-8'| fold -w 256  | head -n 1 | head -c 1)
	
#	number="$digit_1$digit_2$digit_3" 			# concat the 3 digits together
	
#	number=$(( number % 128 )) 		# range is 0-127 

#	number=$((number+1))		# range is 1-128
	
#	echo "$number"

#	echo "$file"
	
#	dd if=/dev/urandom of="$file" bs="$number" count=1024		# read bytes=1024* number (===> number_Kilobytes)  and write them into "$file"
	
#done


LC_ALL="$OLD_LC_ALL"

echo "Finally Finished"

exit 0

#echo All parameters = $*






