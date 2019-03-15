
echo "*******************************************"
echo "********** COVERAGE SECTION ***************"
echo "*******************************************"

rm -f dominion.c.gcov > /dev/null

echo "Getting dominion.c Coverage..."
gcov -b dominion.c

echo "Getting Sea Hag Section of dominion.c.gcov..."
tail -n +987 dominion.c.gcov > testCoverageOutput
head -n +34 testCoverageOutput > finalCoverageOutput
cat finalCoverageOutput
rm -f testCoverageOutput finalCoverageOutput

echo "*******************************************"
echo "********* END OF COVERAGE SECTION *********"
echo "*******************************************"
echo " "
echo " "
