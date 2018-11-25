#/bin/bash

CC=clang++
if ! clang -v > /dev/null 2>&1;then
    CC=g++
fi
PATH_PREFIX=$(pwd)

OBJF="$PATH_PREFIX/src/main/vshell_main \
    $PATH_PREFIX/src/format/out_format \
    $PATH_PREFIX/src/shell_parser/shell_parser \
    $PATH_PREFIX/src/shell_parser/shell_parser_base \
    $PATH_PREFIX/src/line_parser/line_parser \
    $PATH_PREFIX/lib/memory_file  \
    $PATH_PREFIX/lib/command "

function build_or_not()
{
    if [[ ! -f $1.o || $1.cpp -nt $1.o || $1.h -nt $1.o ]];then
        cd $(dirname $1);
            echo "$CC -std=c++11 -c $1.cpp"
            $CC -std=c++11 -c ${1##*/}.cpp
        cd - > /dev/null 2>&1
    fi
}

function build_all()
{
    set -e

    ofile=""
    for cppfile in $OBJF;
    do
        build_or_not $cppfile
        ofile="$ofile$cppfile.o "
    done
    echo "$CC ++ -std=c++11 -o vshell $ofile"
    $CC -std=c++11 -o vshell $ofile
    echo "done"
    set +e
}

function clean()
{
    set -e
    for cppfile in $(find $PATH_PREFIX -name '*.o');
    do
        echo "rm -f $cppfile"
        rm -f $cppfile
    done
    if [[ -f vshell ]];then
        echo "rm vshell"
        rm vshell
    fi
    echo "done"
    set +e
}

function uninstall()
{
    set -e
    rm /usr/local/bin/vshell > /dev/null 2>&1 || true
    echo "vshell unstall done"
    set +e
}

function install()
{
    set -e
    if [[ ! -f $PATH_PREFIX/vshell ]];then
        build_all
    fi
    uninstall > /dev/null 2>&1
    mkdir -p /usr/local/bin
    ln -s $PATH_PREFIX/vshell /usr/local/bin/vshell
    echo "vshell install done"
    set +e
}

function reinstall()
{
    set -e
    rm $PATH_PREFIX/vshell > /dev/null 2>&1 || true
    install;
}

case $1 in
clean)
    clean
;;
uninstall)
    uninstall
;;
install)
    install
;;
reinstall)
    reinstall
;;
all|*)
    build_all
;;
esac


