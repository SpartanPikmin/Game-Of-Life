#!/bin/bash
#2 5 10 15 20 25 30 35 40 45 50 60 70 80 90 100
for J in 1 2 3 4 5
do
  for I in 1 2 5 10 15 20 25 30 35 40 45 50 60 70 80 90 100
  do
      TIME_FILE="Results_Threads_${I}.txt"
      touch ${TIME_FILE}
      ./life -c 50000 -r 50000 -t 10 -n ${I} -x 20 -y 20 -f ggun.life -x 1000 -y 1000 -f ggun.life -x 3000 -y 2000 -f ggun.life >> ${TIME_FILE}

      TIME_FILE_OPENMP="Results_Openmp_${I}.txt"
      touch ${TIME_FILE_OPENMP}
      ./mplife -c 50000 -r 50000 -t 10 -n ${I} -x 20 -y 20 -f ggun.life -x 1000 -y 1000 -f ggun.life -x 3000 -y 2000 -f ggun.life >> ${TIME_FILE_OPENMP}

      TIME_FILE_OPENMP_FOR="Results_Openmp_FOR_${I}.txt"
      touch ${TIME_FILE_OPENMP_FOR}
      ./mplife -c 50000 -r 50000 -t 10 -n ${I} -x 20 -y 20 -f ggun.life -x 1000 -y 1000 -f ggun.life -x 3000 -y 2000 -f ggun.life -l >> ${TIME_FILE_OPENMP_FOR}

  done
done
