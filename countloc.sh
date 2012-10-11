files=`find . -name \*.h -or -name \*.c`
wcinfo=`cat $files | sed '/^\s*$/d' | wc`
loc=`echo $wcinfo | awk '{print $1}'`
bytes=`echo $wcinfo | awk '{print $3}'`
echo "$loc lines of code and $bytes bytes"
