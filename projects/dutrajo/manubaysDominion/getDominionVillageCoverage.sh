
echo "*******************************************"
echo "********** COVERAGE SECTION ***************"
echo "*******************************************"

rm -f dominion.c.gcov > /dev/null

echo "Getting dominion.c Coverage..."
gcov -b dominion.c

echo "Getting Village Section of dominion.c.gcov..."
tail -n +965 dominion.c.gcov > testCoverageOutput
head -n +23 testCoverageOutput > finalCoverageOutput
cat finalCoverageOutput
rm -f testCoverageOutput finalCoverageOutput

echo "*******************************************"
echo "********* END OF COVERAGE SECTION *********"
echo "*******************************************"
echo " "
echo " "
