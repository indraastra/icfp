grep '\[PUB\]' notes.txt | tr -s " " | cut -f3 -d" " | sort -u
