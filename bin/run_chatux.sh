#/bin/bash

ip=
port=
repeat_question=1
while [ $repeat_question -eq 1 ] ; do
	read -p "Launch [S]erver or [C]lient ? " launched_type
	case $launched_type in
		[Ss]* ) read -p "IP to use: " ip ; read -p "Port to use: " port ; repeat_question=0 ; break ;;
		[Cc]* ) read -p "IP to connect to: " ip ; read -p "Port to connect: " port ; repeat_question=0 ; break ;;
		* ) echo "Unknown answer, please type S or C." ;;
	esac
done

read -p "Alias to use? " user_alias


export LD_LIBRARY_PATH=../lib/:$LD_LIBRARY_PATH
./chatux $launched_type $user_alias $ip $port
