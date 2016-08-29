#! /bin/sh

here=`pwd`
if test $? -ne 0; then exit 2; fi
tmp=/tmp/$$
mkdir $tmp
if test $? -ne 0; then exit 2; fi
cd $tmp
if test $? -ne 0; then exit 2; fi

fail()
{
    echo "FAILED" 1>&2
    cd $here
    chmod -R u+w $tmp
    rm -rf $tmp
    exit 1
}

pass()
{
    echo "PASSED" 1>&2
    cd $here
    chmod -R u+w $tmp
    rm -rf $tmp
    exit 0
}

trap "fail" 1 2 3 15

export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
# execute test here. PWD is temporary, refer to classdesc home directory 
# with $here

if [ -n "$AEGIS_ARCH" ]; then
  BL=`aegis -cd -bl`
  BL1=$BL/../../baseline
else #standalone test
  BL=.
  BL1=.
fi

cat >test.cc <<EOF
#include <classdesc.h>
#include "classdesc_epilogue.h"
using classdesc::string;
int main()
{
    string foo("hello");
    if (foo!="hello") return 1;
    if (foo!=string("hello")) return 1;
    if (foo=="foo") return 1;
    if (foo==string("foo")) return 1;
    if (foo=="hello" && foo==string("hello")) return 0;
}
EOF

g++ -DTR1 -I$here -o a.out test.cc
if test $? -ne 0; then fail; fi
./a.out
if test $? -ne 0; then fail; fi

pass
