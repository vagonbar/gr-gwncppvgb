#!/bin/bash
# gwn_modtool : creates new block an converts code from block template


### verify syntax and number of parameters, exec from build directory
CURDIR=`pwd`
GREPBUILD=`echo $CURDIR | grep /build$`

if [ $# -ne 4 ]           # verify number of parameters
then
  echo "gwn_modtool syntax:"
  echo "  gwn_modtool block_name nr_inputs nr_outputs nr_timers"
  echo "Must be executed from the build directory"
  echo
  exit
elif [ -z $GREPBUILD ]    # verify execution from ./build directory"
then
  echo "gwn_modtool:"
  echo "  must be executed from the build directory"
  exit
fi


### parameters, default and user
# block attributes from positional parameters
BLOCK_NAME=$1
NR_IN=$2
NR_OUT=$3
NR_TIMERS=$4

### get user parameters list
MSG_PARS="\"GWN_parameter_list\""
MSG_VARS="\"GWN_variable_list\""
echo -n "Enter parameter list: "; read USER_PARS
#USER_PARS="std::string message, int counter"
echo "User parameters:     " $USER_PARS

# extracts variable names from list of "type variable, ..."
I=0
for WORD in $USER_PARS
do
  if [ $(($I % 2)) -eq 1 ]
  then
    if [ $I -eq 1 ]
    then
      USER_VARS=$WORD
    else
      USER_VARS=${USER_VARS}" "${WORD}
    fi
  fi
  ((I += 1))
done
echo "User variables:" $USER_VARS


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
#SED_PARAMETERS=\"s/$DEFAULT_PARS/$PARAMETERS/g\"

# create files for new blocks
cd ..   # from build directory to module root directory
echo -en "... to module root directory to execute gr_modtool:\n      "; pwd
GRMODTOOL=`which gr_modtool`
echo ... gr_modtool, creating $BLOCK_NAME
python $GRMODTOOL add --block-name=$BLOCK_NAME \
  --block-type=general --lang=cpp \
  --argument-list="" --add-python-qa
echo ...gr_modtool, creating ${BLOCK_NAME}_pdata
python $GRMODTOOL add --block-name=${BLOCK_NAME}_pdata --block-type=noblock --lang=cpp --argument-list="" 
cd $CURDIR
echo -en "... returned to module build directory:\n      "; pwd


### transform files by substitution in template files

# new block files
echo "... processing ../include/gwncppvgb/${BLOCK_NAME}.h"
sed -e $SED_GWNBLOCKC -e $SED_GWNBLOCKC_CAPS \
  -e "s/$MSG_PARS/$USER_PARS/g" \
  ../libgwn/gwnblockc.h > ../include/gwncppvgb/${BLOCK_NAME}.h

echo "... processing ../lib/${BLOCK_NAME}_impl.h"
sed -e $SED_GWNBLOCKC -e $SED_GWNBLOCKC_CAPS \
  -e "s/$MSG_PARS/$USER_PARS/g" \
  ../libgwn/gwnblockc_impl.h > ../lib/${BLOCK_NAME}_impl.h

echo "... processing ../lib/${BLOCK_NAME}_impl.cc"
sed -e $SED_GWNBLOCKC -e $SED_GWNBLOCKC_CAPS \
  -e "s/$MSG_PARS/$USER_PARS/g" \
  -e "s/$MSG_VARS/$USER_VARS/g" \
  -e "s/d_name = \"no_name\"/d_name = \"$BLOCK_NAME\"/g" \
  -e "s/d_number_in = 0/d_number_in = $NR_IN/g" \
  -e "s/d_number_out = 0/d_number_out = $NR_OUT/g" \
  -e "s/d_number_timers = 0/d_number_timers = $NR_TIMERS/g" \
  ../libgwn/gwnblockc_impl.cc > ../lib/${BLOCK_NAME}_impl.cc

# new block QA file
echo "... processing ../python/qa_${BLOCK_NAME}.py"
sed -e $SED_GWNBLOCKC \
  ../libgwn/qa_gwnblockc.py > ../python/qa_${BLOCK_NAME}.py

# new block process data files
echo "... processing ../include/gwncppvgb/${BLOCK_NAME}_pdata.h"
sed -e $SED_GWNBLOCKC -e $SED_GWNBLOCKC_CAPS \
  ../libgwn/gwnblockc_pdata.h > ../include/gwncppvgb/${BLOCK_NAME}_pdata.h

echo "... processing ../lib/${BLOCK_NAME}_pdata.cc"
sed -e $SED_GWNBLOCKC -e $SED_GWNBLOCKC_CAPS \
  ../libgwn/gwnblockc_pdata.cc > ../lib/${BLOCK_NAME}_pdata.cc

echo "... block $1 created."

