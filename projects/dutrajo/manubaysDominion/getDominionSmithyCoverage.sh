
echo "*******************************************"
echo "********** COVERAGE SECTION ***************"
echo "*******************************************"

rm -f dominion.c.gcov > /dev/null

echo "Getting dominion.c Coverage..."
gcov -b dominion.c

echo "Getting Smithy Section of dominion.c.gcov..."
tail -n +940 dominion.c.gcov > testCoverageOutput
head -n +26 testCoverageOutput > finalCoverageOutput
cat finalCoverageOutput
rm -f testCoverageOutput finalCoverageOutput

echo "*******************************************"
echo "********* END OF COVERAGE SECTION *********"
echo "*******************************************"
echo " "
echo " "
