# Indroduction to aclocal

# function of aclocal

Generate 'aclocal.m4' by scanning configure.ac' or 'configure.in'

gegerate aclocal.m4 by expanding the define of macro of configure.ac using the *.m4 file.
I

where is the *.m4 file?
1)--print-ac-director
              print name of directory holding system-wide third-party m4 files, then exit
➜  src  aclocal --print-ac-dir
/usr/local/Cellar/automake/1.15/share/aclocal


2)default dir

➜  src  ls /usr/local/share/aclocal
aclocal/       aclocal-1.15@
note：different version of aclocal

3）user defile the dir of m4 files
aclocal -I /usr/share/aclocal1.10 -I /usr/share/libtool/m4 --install
the flag `--install` will install the m4 files  to the first dir of `-I`

Q&A
Q1.Libtool library used but `LIBTOOL' is undefined  
A1.you do not have libtool installed.
S1:install autoconf, libtool,automake
A1.you do not have your aclocal configured right.
config it according to the doc.


#autoconf && automake Generate Makefile

## Indroduction
when we compile program in linux OS, we usually do `./configure, make, make install`. However, to run the three commands above, we need to files:*configure* and *Makefile*. In this article , we will indroduce how to generate *Makefile* using toos `autoconf` and `automake`.

## Indroduction to these tools.
To get the `Makefile`, we need these tools:autoconf/automake/m4/perl/libtool. The function of these tools is :

antoconf :  configure.ac/configure.in >>>> configure

automake:   Makefile.am >>>> Makefile.in

automake support 3 structure：flat，shallow，deep.
flat: all the files in one Dir.
shallow: The main resource file is in the top Dir, others are in the sub directory.
deep: all the files are in sub Dir, the top directory only contains configure file.

