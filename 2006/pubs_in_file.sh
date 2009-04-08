sed -e "s///g" $1 | grep -e "[A-Z]\+.[A-Z]\+=.*@[0-9]\+|[0-9a-f]\{31\}" | sort -u
