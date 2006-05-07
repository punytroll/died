#!/bin/sh
cd `dirname "$0"`

echo -e "Welcome ...
This is the autogen.sh script to help you build \"died\" from repository sources."
echo

echo "Running autoheader ..."
autoheader
echo "Done. [Return = $?]"
echo

echo "Running aclocal ..."
aclocal
echo "Done. [Return = $?]"
echo

echo "Running autoconf ..."
autoconf
echo "Done. [Return = $?]"
echo

echo "Running libtoolize ..."
echo -e "\tOptions are:"
echo -e "\t\t--copy\tto create real files and not symbolic links"
libtoolize --copy
echo "Done. [Return = $?]"
echo

echo "Running automake ..."
echo -e "\tOptions are:"
echo -e "\t\t--copy\t\tto create real files and not symbolic links"
echo -e "\t\t--add-missing\tto create the files install-sh mkinstalldirs and missing"
automake --copy --add-missing
echo "Done. [Return = $?]"
echo

echo "You may run ./configure now."
echo

exit
