#!/bin/bash
set -x
NAME=msim
CHANGELOG=${NAME}.changes
( 
cd ..
make printversion
)
VERSION=`../print_msim_version`
PACKDIR=${HOME}/packaging/msim

makearchive.sh ${NAME}-${VERSION} master
cp ${NAME}-${VERSION}.tar.gz ${PACKDIR}
cp ${CHANGELOG} ${PACKDIR}
echo ${NAME} > ${PACKDIR}/pack-name
echo ${VERSION} > ${PACKDIR}/pack-version
ls -l ${PACKDIR}/${NAME}-${VERSION}.tar.gz
ls -l ${PACKDIR}/pack-*

if [ x$2 == "xfresh" ]
then
  rm ${PACKDIR}/pack-count
fi
  

if [ x$1 == "xmake" ]
then
  cd ${PACKDIR}
  make
fi
