#!/bin/bash
# gwn_modtool : creates new block an converts code from block template


### verify syntax and number of parameters, exec from build directory
CURDIR=`pwd`
GREPBUILD=`echo $CURDIR | grep /build$`

if [ $# -ne 5 ]           # verify number of parameters
then
  echo "gwn_modtool syntax:"
  echo "  gwn_modtool block_name nr_inputs nr_outputs nr_timers nr_timeouts"
  echo "Must be executed from the build directory"
  echo
  exit
elif [ -z $GREPBUILD ]    # verify execution from ./build directory"
then
  echo "gwn_modtool:"
  echo "  must be executed from the build directory"
  exit
fi

# block attributes from positional parameters
BLOCK_NAME=$1
NR_IN=$2
NR_OUT=$3
NR_TIMERS=$4
NR_TIMEOUTS=$5

### add parameter list
DEFAULT_PARS="std::string $BLOCK_NAME, int $NR_IN, int $NR_OUT, int $NR_TIMERS, int $NR_TIMEOUTS"
echo -n "Enter parameter list: "
read OPT_PARS
echo $OPT_PARS
if [ "$OPT_PARS" ]
then
  PARAMETERS=${DEFAULT_PARS}", "${OPT_PARS}
else
  PARAMETERS=${DEFAULT_PARS}
fi
echo Parameter list: $PARAMETERS

### create block with gr_modtool
echo -n "About to create block $1; proceed(YyNn)? "
read ANSWER
if [ $ANSWER != "y" -a $ANSWER != "Y" ]
then
  echo Answer was not "y" nor "Y": $ANSWER
  exit
fi
echo "Creating block $1..."

# sed substitution expressions
BLOCK_NAME_CAPS=`echo $BLOCK_NAME | tr [a-z] [A-Z]`
SED_GWNBLOCKC="s/gwnblockc/${BLOCK_NAME}/g"
SED_GWNBLOCKC_CAPS="s/GWNBLOCKC/${BLOCK_NAME_CAPS}/g"

## create files for new blocks
cd ..   # from build directory to module root directory
echo -en "... to module root directory to execute gr_modtool:\n      "; pwd

echo ... gr_modtool, creating $BLOCK_NAME
GRMODTOOL=`which gr_modtool`
python $GRMODTOOL add --block-name=$BLOCK_NAME --block-type=general --lang=cpp \
  --argument-list="${PARAMETERS}" --add-python-qa

echo ...gr_modtool, creating ${BLOCK_NAME}_pdata
python $GRMODTOOL add --block-name=${BLOCK_NAME}_pdata --block-type=noblock --lang=cpp --argument-list="" 

cd $CURDIR
echo -en "... returned to module build directory:\n      "; pwd


### transform files by substitution in template files

# block files
echo "... processing ../include/gwncppvgb/${BLOCK_NAME}.h"
sed -e $SED_GWNBLOCKC -e $SED_GWNBLOCKC_CAPS \
  ../libgwn/gwnblockc.h > ../include/gwncppvgb/${BLOCK_NAME}.h

echo "... processing ../lib/${BLOCK_NAME}_impl.h"
sed -e $SED_GWNBLOCKC -e $SED_GWNBLOCKC_CAPS \
  ../libgwn/gwnblockc_impl.h > ../lib/${BLOCK_NAME}_impl.h

echo "... processing ../lib/${BLOCK_NAME}_impl.cc"
sed -e $SED_GWNBLOCKC -e $SED_GWNBLOCKC_CAPS \
  ../libgwn/gwnblockc_impl.cc > ../lib/${BLOCK_NAME}_impl.cc


# QA file
echo "... processing ../python/qa_${BLOCK_NAME}.py"
sed -e $SED_GWNBLOCKC \
  ../libgwn/qa_gwnblockc.py > ../python/qa_${BLOCK_NAME}.py


# process data files
echo "... processing ../include/gwncppvgb/${BLOCK_NAME}_pdata.h"
sed -e $SED_GWNBLOCKC -e $SED_GWNBLOCKC_CAPS \
  ../libgwn/gwnblockc_pdata.h > ../include/gwncppvgb/${BLOCK_NAME}_pdata.h

echo "... processing ../lib/${BLOCK_NAME}_pdata.cc"
sed -e $SED_GWNBLOCKC -e $SED_GWNBLOCKC_CAPS \
  ../libgwn/gwnblockc_pdata.cc > ../lib/${BLOCK_NAME}_pdata.cc

echo "... block $1 created."

