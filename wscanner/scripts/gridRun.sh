#!/bin/bash

folderName=$1
jobID=$2

echo "Gonna subtract 1 from input job ID to make it go from 0, to n-1, as the random seed goes from 1 to N"

jobID="$(($jobID-1))"
jobIDStr=job${jobID}_

echo "------------ enviornment -------------------"
export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
alias setupATLAS='source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh'
source source/setup.sh


# create the directory structure that we need, locally
echo "------------ ls -------------------"
ls -l *

echo "------------ make code -------------------"
mkdir build
cd build
cmake ../source
make
source wscanner/setup.sh
cd ../

ls -l *

echo "------------ create run -------------------"
mkdir run
mv gridJobList run
mv workspaces run
cd run/


echo "------------ Trying to run -------------------"
# run the command
for files in $folderName/*.sh
do
    if [[ $files == *${jobIDStr}* ]]; then

        echo 'Running', ${files}
        source ${files}
    else
      echo 'Skipping', ${files}
    fi

done

echo "------------ Consolidating output files -------------------"
outList=""
if [ -d "root-files" ]; then
  outList=root-files/ $outList
fi
if [ -d "uncondWs" ]; then
  outList=uncondWs/ $outList
fi
# create the output tar
tar -cvzf JobSummary.tgz.gz ${outList}
mv JobSummary.tgz.gz ../

