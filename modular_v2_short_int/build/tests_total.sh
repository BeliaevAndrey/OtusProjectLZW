#! /bin/bash

clear

testpath_in="../../TestData/plain"
testpath_outC="../../TestData/compressed"
testpath_outD="../../TestData/back"
reportfile="report.txt"
logfile="lzw_project_v1_21.log"


if [ ! -d $testpath_outC ]; then
    mkdir -p $testpath_outC;
fi

if [ ! -d $testpath_outD ]; then
    mkdir -p $testpath_outD;
fi

echo "Tests starter"
echo "Files for tests are in $testpath_in"
echo "Compressed files will be in $testpath_outC"
echo "Decompressed files will be in $testpath_outD"
echo "The /bin/sha256sum utility is used to compute chekcsums"
echo


echo "clearing paths..."
rm $testpath_outC/*
rm $testpath_outD/*

date > $reportfile
echo "File list" >> $reportfile
echo "==============================" >> $reportfile

for file in $(ls $testpath_in); do
    echo "$testpath_in/$file" >> $reportfile;
done

echo "==============================" >> $reportfile
echo >> $reportfile

for testfilename in $(ls $testpath_in); do
    echo "Test compressing: $testpath_in/$testfilename";
    bin/lzw_otus_v1_21 -c \
    $testpath_in/$testfilename \
    $testpath_outC/$testfilename.lzwbin &>>$logfile;
    
    echo "Test decompressing: $testpath_outC/$testfilename"
    bin/lzw_otus_v1_21 -d \
    $testpath_outC/$testfilename.lzwbin \
    $testpath_outD/$testfilename &>>$logfile;

    sleep 1;
    
    echo;
    echo >> $reportfile;
    date >> $reportfile;
    echo $testfilename  | tee -a $reportfile;
    echo "counting crc..."  | tee -a $reportfile;

    src_crc=$(/bin/sha256sum $testpath_in/$testfilename | awk '{print $1}');
    if [ -f $testpath_outD/$testfilename ]; then
        rst_crc=$(/bin/sha256sum $testpath_outD/$testfilename  | awk '{print $1}');
        echo "source:" $src_crc | tee -a $reportfile;
        echo "result:" $rst_crc | tee -a $reportfile;
        echo "compare..." | tee -a $reportfile;
        if [ $src_crc == $rst_crc ]; then
            echo "correct" | tee -a $reportfile;
        else
            echo "wrong: cecksums are not equal." | tee -a $reportfile;
        fi
    else
        echo "wrong: file not found." | tee -a $reportfile
    fi
done