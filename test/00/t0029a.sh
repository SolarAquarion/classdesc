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
CC=${CC:-g++}
PATH=$here:$BL:$BL1:$PATH

cat >test.cc <<EOF
#include "pack_base.h"
#include "classdesc_epilogue.h"
namespace foo
{
  namespace bar
  {
    class foobar
    {
    };

  }
}
#include "test.cd"

main() {}
EOF

classdesc pack unpack <test.cc >test.cd
if test $? -ne 0; then fail; fi

$CC -I$here -I$BL -I$BL1 -DTR1 test.cc 
if test $? -ne 0; then fail; fi

pass
