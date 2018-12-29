C++ code coverage profiling with GCC/GCOV
The coverage analysis with GCC/GCOV includes three following steps
* instrumented application build — libraries, executable(s), and profiling artifacts (*.gcno files) are created
* the application test run(s) — the runtime coverage statistics (*.gcda files) is collected
* coverage statistics post-processing with GCOV/LCOV — text/HTML coverage reports are generated

##Instrumented application build
To enable the instrumented compilation use GCC/G++ with --coveragee flag. The know-how here is to specify the full path to source files during the compilation in order to be able to perform a cross-profiling and ease the use of LCOV (described below).

$ g++ -c -g -O0 --coverage -o $PWD/obj/myclass.o $PWD/myclass.cpp
$ g++ -c -g -O0 --coverage -o $PWD/obj/main.o $PWD/main.cpp
$ g++ -g -O0 --coverage -o $PWD/bin/myapp $PWD/obj/*.o

The GNU Make has two useful functions to convert filenames to absolute ones: $(abspath ...) and $(relpath ...)
When compiling with --coverage flag, the *.gcno file is created in the same location, as the object file . This file is used by GCOV for post-processing application's statistics collected at runtime and contains profiling arcs information.

$ ls $PWD/obj
main.gcno main.o myclass.gcno myclass.o
$
Coverage statistics collection
An instrumented application collects coverage statistics at runtime and creates a set of *.gcda files (or updates existing ones) on exit. For every *.gcno file created during the build a corresponding *.gcda file is created by the instrumented application upon exit. For *.gcda files to be generated, the application must exit cleanly by either returning from main() or by calling exit().
For client/server applications I typically install SIGTERM handler to ensure a clean application termination.
The directory, where *.gcda files are to be created should exist and be writable by the application. A *.gcda file is created by default in the directory where the corresponding *.gcno file was created during the build.
To find out the exact location the following command can be used

$ strings $PWD/bin/myapp | egrep '.gcda$'
/home/bobah/Work/coverage/obj/main.gcda
/home/bobah/Work/coverage/obj/myclass.gcda

*Here, we will use GCOV_PREFIX_STRIP to assign directory where the \*.gcda file is generated.*

In many cases the coverage statistics should be collected from the application running on the environment (host, user etc.) other then the one where the application was built, so creating *.gcda files in the build directory may be impossible or impractical. To override the location to store the *.gcda files two environment variables can be used: GCOV_PREFIX and GCOV_PREFIX_STRIP. For example, if we want to replace the four leading elements in the path ("/home/bobah/Work/coverage") where myapp stores *.gcda with "/home/bobah/Work/cov_rpt" we would need to define the variables as follows (bash syntax) in the application's environment

export GCOV_PREFIX="/home/bobah/Work/cov_rpt"
export GCOV_PREFIX_STRIP=4

As a result of the override, the file myclass.gcda will be created in /home/bobah/Work/cov_rpt and not in /home/bobah/Work/coverage
Note, that for the post processing of the coverage data the most convenient way is to release source files and *.gcno artifacts to under $GCOV_PREFIX, so that a *.gcda file is created in the same directory as corresponding *.gcno
I do it using rsync like

rsync -acv --filter='+ */' --filter='+ *.cpp' --filter='+ *.h' --filter='+ *.gcno' --filter='- *' /home/bobah/Work/coverage/ /home/bobah/Work/cov_rpt
The coverage data is accumulated during subsequent application runs. To reset the counters either delete all *.gcda files under $GCOV_PREFIX directory

$ find $GCOV_PREFIX -type f -name '*.gcda' -print | xargs /bin/rm -f

Or use LCOV functionality

$ lcov --directory $GCOV_PREFIX --zerocounters
Post-processing runtime coverage statistics
I prefer using LCOV wrapper for the coverage data processing because it generates nicely looking HTML reports.
The essential bit here is that the source code tree is available in exactly the same place as it was during the build. This is required because gcov application behavior can't be manipulated using GCOV_PREFIX/GCOV_PREFIX_STRIP and it expects files exactly as they are stored in *.gcno files.
The data post processing:

$ lcov --directory $GCOV_PREFIX --capture --output-file $GCOV_PREFIX/app.info

The HTML reports generation:

$ genhtml --output-directory $PWD/cov_htmp $GCOV_PREFIX/app.info
