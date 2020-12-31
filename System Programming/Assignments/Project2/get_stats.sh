#!/bin/bash



old_IFS="$IFS"		#saving value of $IFS

IFS=

# Variables

ID_counter=0
ID_list=()
maxID=0
minID=9999


bytes_sent=0
bytes_received=0

files_sent=0
files_received=0

clients_exited=0

Exit_counter=0







while read -r line1; do		# read a line


	IFS=' '
	read -a line <<< "${line1}"



	first_word="${line[0]}"
	
#	echo "$first_word"

	if [ "$first_word" == "Connected" ]; then		# If first word is "Connected"
	
		ID="${line[3]}"
	
		ID_list+=( "$ID" )			# the fourth word is the ID of the client
		
		ID_counter=$((ID_counter+1))
		
		
		
		# Check for min/max IDs
		if [ "$ID" -gt "$maxID" ]; then		# check for max
		
			maxID="$ID"		# new maxID
		fi
		
		
		if [ "$ID" -lt "$minID" ]; then		# check for min
		
			minID="$ID"		# new minID
		fi
					
	
	elif [ "$first_word" == "Sent" ]; then		# Sent a file
	
		bytes="${line[1]}"
		
		bytes_sent=$((bytes_sent + bytes))		# incrementing by bytes

		files_sent=$((files_sent + 1))			#incrementing counter
		
		
	elif [ "$first_word" == "Received" ]; then		# Received a file
	
		bytes="${line[1]}"
		
		bytes_received=$((bytes_sent + bytes))		# incrementing by byte
		
		files_received=$((files_sent + 1))			#incrementing counter
	
	
	
	elif [ "$first_word" == "Exiting" ]; then		# Exiting
	
		Exit_counter=$((Exit_counter + 1))			#incrementing counter
	
	fi


done


# Printing results

echo "Number of clients connected is $ID_counter and the list is: "

for k in "${ID_list[@]}"; do		#print List
	echo "$k"
done

echo "Max_ID is $maxID and min_ID is $minID"

echo "(Total BYTES) bytes sent: $bytes_sent and received: $bytes_received"

echo "(Total FILES) files sent: $files_sent and received: $files_received"

echo "Number of clients that left the system: $Exit_counter"



IFS="$old_IFS"		# reseting IFS

exit 0


