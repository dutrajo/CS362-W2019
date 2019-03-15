/*
 * File: UrlValidatorTest.java
 * Created by: Joseph Dutra, Samantha Manubay, Myung Jin Song
 * Created Date: 03/02/2019
 * Class: CS362 - 400 - Winter 2019
 * Project: Final Project Part-B
 * Contains: Class UrlValidatorTest
 */

import junit.framework.TestCase;
import java.util.ArrayList;
import java.util.Random;


/*
 * class UrlValidatorTest
 * 
 * Description:
 * This class is used to test the isValid function of the UrlValidator class. It
 * contains the main method that is launched when the UrlValidatorTest program 
 * is ran. 
 */
public class UrlValidatorTest extends TestCase {
	

	/*
	 * public UrlValidatorTest(String testName)
	 * 
	 * Input Parameters: 
	 * 		String testName: string contaning the name of the test
	 * 
	 * Returns:
	 * 		Nothing.
	 * 
	 * Description:
	 * 		This is the only constructor, it requires an input string to 
	 * specify the class's test name.
	 */
	public UrlValidatorTest(String testName)
	{
		//pass the testName to the parent class
		super(testName);
	}
	
	/*
	 * public void testManualBasedTesting()
	 * 
	 * Input Parameters: 
	 * 		None.
	 * 
	 * Returns:
	 * 		Nothing.
	 * 
	 * Description:
	 * 		This function runs a series of manual tests (hard coded). For
	 * each test, it will output information to the screen and specify
	 * that the test passed. A failed test will cause the program to stop
	 * due to the 'assertEquals' function call for each test.
	 */
	public void testManualBasedTesting()
	{
		//print test information to screen
		System.out.println("***********************************************");
		System.out.println("***********************************************");
		System.out.println("Now Testing isValid() with manual based tests...");
		
		//initialize a UrlValidator object
		UrlValidator url = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
		boolean result = false;
		String urlTest = "";
		
		//begin the manual tests....
		
		urlTest = "http://www.google.com";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, true, result);
		System.out.println(" ....Passed");
		
		urlTest = "https://www.google.com";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, true, result);
		System.out.println(" ....Passed");
		
		urlTest = "http://www.oregonstate.edu";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, true, result);
		System.out.println(" ....Passed");
		
		urlTest = "https://www.oregonstate.edu";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, true, result);
		System.out.println(" ....Passed");
		
		urlTest = "http://a.com";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, true, result);
		System.out.println(" ....Passed");
		
		urlTest = "http://123.com";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, true, result);
		System.out.println(" ....Passed");
		
		urlTest = "ftp://127.0.0.1";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, true, result);
		System.out.println(" ....Passed");
		
		urlTest = "http://1.com:1/endpoint";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, true, result);
		System.out.println(" ....Passed");
		
		urlTest = "http://1.com:1/endpoint";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, true, result);
		System.out.println(" ....Passed");
		
		urlTest = "htp://1.com:64435/";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, true, result);
		System.out.println(" ....Passed");
		
		urlTest = "htp://1.com:1/test/garbage/hello";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, true, result);
		System.out.println(" ....Passed");
		
		urlTest = "htp://1.com:1/test/garbage/hello?action=hello";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, true, result);
		System.out.println(" ....Passed");
		
		urlTest = "htp://1.1.1.1:6534/test/garbage/hello?action=hello";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, true, result);
		System.out.println(" ....Passed");
		
		urlTest = "htp://[2001:0db8:85a3:0000:0000:8a2e:0370:7334]/test/garbage/hello?action=hello";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, true, result);
		System.out.println(" ....Passed");
		
		urlTest = "htp://[2001:db8:85a3:0:0:8a2e:370:7334]/test/garbage/hello?action=hello";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, true, result);
		System.out.println(" ....Passed");
		
		urlTest = "htp://[2001:0db8:85a3:0000:0000:8a2e:0370:7334]:20/test/garbage/hello?action=hello";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, true, result);
		System.out.println(" ....Passed");
		
		urlTest = "htp://[0:0:0:0:0:0:0:1]:1";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, true, result);
		System.out.println(" ....Passed");
		
		urlTest = "htp://1.com:65536/";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, false, result);
		System.out.println(" ....Passed");
		
		urlTest = "garbage:/1.com:1/endpoint";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, false, result);
		System.out.println(" ....Passed");
		
		urlTest = "http://.aaa";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, false, result);
		System.out.println(" ....Passed");
		
		urlTest = "http://test.com:-1";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, false, result);
		System.out.println(" ....Passed");
		
		urlTest = "http://1.2.3.4.";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, false, result);
		System.out.println(" ....Passed");
		
		urlTest = "http://1.2.3.4.5";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, false, result);
		System.out.println(" ....Passed");
		
		urlTest = "3htp://1.com:1/";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, false, result);
		System.out.println(" ....Passed");
		
		urlTest = "3htp://.com:1/";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, false, result);
		System.out.println(" ....Passed");
		
		urlTest = "htp://[-1:-1:0:0:0:0:0:1]:1";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, false, result);
		System.out.println(" ....Passed");
		
		urlTest = "htp://[1:1:0:0:0:0:0:1:0]:1";
		System.out.print("Manual Testing: " + urlTest);
		result = url.isValid(urlTest);
		assertEquals(urlTest, false, result);
		System.out.println(" ....Passed");
		
		//output that manual testing has finished to screen
		System.out.println("Finished Manual Based Testing.");
		System.out.println("***********************************************");
		System.out.println("***********************************************");
	}

	/*
	 * public void testProgrammingBasedTesting()
	 * 
	 * Input Parameters: 
	 * 		None.
	 * 
	 * Returns:
	 * 		Nothing.
	 * 
	 * Description:
	 * 		This function runs a series of programmatically created tests. It
	 * uses the class variables (defined at bottom of class), to create a 
	 * series of URL strings to test the isValid function with. This function
	 * uses a brute force method to try every combination of the URL components
	 * (again stored at bottom of this class). For each URL tested, the function
	 * will output the current url tested and whether or not it passed to the
	 * screen. If a test fails it will cause the program to abort due to
	 * the assertTrue function calls.
	 */
	public void testProgrammingBasedTesting() 
	{
		//output current test info to screen
		System.out.println("***********************************************");
		System.out.println("***********************************************");
		System.out.println("Now Testing isValid() with programming based tests...");
		
		//initialize the UrlValidator object
		UrlValidator url = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
		
		StringBuilder urlToTest = new StringBuilder();
		boolean validityExpectation = true;
		String urlComplete = new String("");
		
		//iterate through all schemes
		while(schemesToTest.hasMoreTests())
		{
			//iterate through all authorities
			while(authoritiesToTest.hasMoreTests())
			{
				//iterate through all ports
				while(portsToTest.hasMoreTests())
				{
					//iterate through all paths
					while(pathsToTest.hasMoreTests())
					{
						//iterate through all queries
						while(queriesToTest.hasMoreTests())
						{
							//re-init stringbuilder
							urlToTest.setLength(0);
							
							//assume validity is valid at first
							validityExpectation = true;
							
							//generate the url string to test
							urlToTest.append(
								schemesToTest.getCurItem() + 
								authoritiesToTest.getCurItem() + 
								portsToTest.getCurItem() + 
								pathsToTest.getCurItem() + 
								queriesToTest.getCurItem()
								);
								
							//determine the validity of this url string
							validityExpectation = 
								schemesToTest.getCurValid() & 
								authoritiesToTest.getCurValid() & 
								portsToTest.getCurValid() & 
								pathsToTest.getCurValid() & 
								queriesToTest.getCurValid();
								
							//create the string object from the stringbuilder
							urlComplete = urlToTest.toString();
							
							//output current url to string
							System.out.print("Programatically Testing: " + urlComplete);
							
							//get the result from isValid function with this url
							boolean foundResult = url.isValid(urlComplete);
							
							//verify isValid returned expected result
							assertEquals(urlComplete,validityExpectation, foundResult);
							
							//output info to screen
							System.out.println(" ....Passed");
							
							//increment the queries container
							queriesToTest.incr();
						}
						
						//re-init the queries container
						queriesToTest.reinitItemsToTry();
						
						//increment the paths container
						pathsToTest.incr();
					}
					
					//re-init the paths container
					pathsToTest.reinitItemsToTry();
					
					//increment the ports container
					portsToTest.incr();
				}
				
				//re-init the ports container
				portsToTest.reinitItemsToTry();
				
				//increment the authorities container
				authoritiesToTest.incr();
			}
			
			//re-init the authorities container
			authoritiesToTest.reinitItemsToTry();
			
			//increment the schemes container
			schemesToTest.incr();
		}
		
		//output information to screen that tests are finished
		System.out.println("Finished Programming Based Testing.");
		System.out.println("***********************************************");
		System.out.println("***********************************************");
	}
	
	/*
	 * public TestItem generateRandomURLTestItem(ArrayList<String> testsAlreadyTried)
	 * 
	 * Input Parameters: 
	 * 		ArrayList<String> testsAlreadyTried: container of strings (indexes combined)
	 * 
	 * Returns:
	 * 		TestItem: object containing a random URL to test and it's validity
	 * 
	 * Description:
	 * 		This function generates a randomly selected URL by randomly selecting
	 * different URL Components (from class variables at bottom of class) of known
	 * validity. This function also verifies that the randomly selected URL hasn't
	 * been attempted already by storing all previous tests in the testAlreadyTried
	 * container (as string input so "10000" means scheme = 1, authority = 0, ports = 0
	 * paths = 0, and queries = 0). 
	 */
	public TestItem generateRandomURLTestItem(ArrayList<String> testsAlreadyTried)
	{
		//create a random object for use in generating random numbers
		Random rand = new Random();
		
		//determine random index values using length of containers going to use
		int schemesToTestIndex = rand.nextInt(schemesToTest.itemsToTry.length);
		int authoritiesToTestIndex = rand.nextInt(authoritiesToTest.itemsToTry.length);
		int portsToTestIndex = rand.nextInt(portsToTest.itemsToTry.length);
		int pathsToTestIndex = rand.nextInt(pathsToTest.itemsToTry.length);
		int queriesToTestIndex = rand.nextInt(queriesToTest.itemsToTry.length);
		
		//build a string containing these randomly selected indexes
		StringBuilder testIndexes = new StringBuilder();
		testIndexes.append(schemesToTestIndex);
		testIndexes.append(authoritiesToTestIndex);
		testIndexes.append(portsToTestIndex);
		testIndexes.append(pathsToTestIndex);
		testIndexes.append(queriesToTestIndex);
		
		//check if we've already tried this combination of indexes, if so keep randomizing until 
		//we find something new.
		while(testsAlreadyTried.contains(testIndexes.toString()))
		{
			//we've already used these indexes, so reset our string back to zero
			testIndexes.setLength(0);
			
			//select new random index values
			schemesToTestIndex = rand.nextInt(schemesToTest.itemsToTry.length);
			authoritiesToTestIndex = rand.nextInt(authoritiesToTest.itemsToTry.length);
			portsToTestIndex = rand.nextInt(portsToTest.itemsToTry.length);
			pathsToTestIndex = rand.nextInt(pathsToTest.itemsToTry.length);
			queriesToTestIndex = rand.nextInt(queriesToTest.itemsToTry.length);
			
			//and append them to our string to check if we've used them already
			testIndexes.append(schemesToTestIndex);
			testIndexes.append(authoritiesToTestIndex);
			testIndexes.append(portsToTestIndex);
			testIndexes.append(pathsToTestIndex);
			testIndexes.append(queriesToTestIndex);
		}
		
		//store the indexes we settled on so we don't repeat this random test
		testsAlreadyTried.add(testIndexes.toString());
		
		//create a new url string builder to store the final url to test
		StringBuilder url = new StringBuilder();
		
		//assume the url is valid to begin with
		boolean valid = true;
		
		//add all the selected URLs to the url string builder, and logically add
		//the validity (to keep track of all component validity)
		url.append(schemesToTest.itemsToTry[schemesToTestIndex].urlComponent);
		valid &= schemesToTest.itemsToTry[schemesToTestIndex].isValid;
		
		url.append(authoritiesToTest.itemsToTry[authoritiesToTestIndex].urlComponent);
		valid &= authoritiesToTest.itemsToTry[authoritiesToTestIndex].isValid;
		
		url.append(portsToTest.itemsToTry[portsToTestIndex].urlComponent);
		valid &= portsToTest.itemsToTry[portsToTestIndex].isValid;
		
		url.append(pathsToTest.itemsToTry[pathsToTestIndex].urlComponent);
		valid &= pathsToTest.itemsToTry[pathsToTestIndex].isValid;
		
		url.append(queriesToTest.itemsToTry[queriesToTestIndex].urlComponent);
		valid &= queriesToTest.itemsToTry[queriesToTestIndex].isValid;
		
		//return the built url string and validity via a TestItem object
		return new TestItem(url.toString(),valid);
		
	}
	
	/*
	 * public void testRandomBasedTesting()
	 * 
	 * Input Parameters: 
	 * 		None.
	 * 
	 * Returns:
	 * 		None.
	 * 
	 * Description:
	 * 		This function performs random testing of the isValid function. It iterates
	 * a hardcoded number of times (numberOfTestsToTry) and generates a new non-tested
	 * url with the isValid function and compares the results with the expected output.
	 * Each test will be written to the screen, and if any test fails the function will
	 * throw an exception containing the url that failed (via the assertEquals function).
	 */
	public void testRandomBasedTesting()
	{
		//generate UrlValidator object to use for isValid
		UrlValidator url = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
		
		//determine number of tests to attempt (500 seemed good enough)
		int numberOfTestsToTry = 500;
		
		//Create an object to hold indexes of already attempted tests
		ArrayList<String> testsAlreadyTried = new ArrayList<String>();
		
		//output information to screen that tests are finished
		System.out.println("Starting Random Based Testing.");
		System.out.println("***********************************************");
		System.out.println("***********************************************");
		
		//iterate through the number of tests, and perform test
		for(int i=0; i < numberOfTestsToTry; i++)
		{
			//create a random URL with a known validity
			TestItem urlItem = generateRandomURLTestItem(testsAlreadyTried);
			
			//output url info to screen
			System.out.print("Random Testing: " + urlItem.urlComponent);
			
			//test the isValid function
			boolean result = url.isValid(urlItem.urlComponent);
			
			//verify it returned the expected results, if not throw exception
			assertEquals(urlItem.urlComponent,urlItem.isValid, result);
			
			//output status to screen
			System.out.println(" ....Passed");
		}
		
		
		//output information to screen that tests are finished
		System.out.println("Finished Random Based Testing.");
		System.out.println("***********************************************");
		System.out.println("***********************************************");
	}
   
	/*
	 * public static void main(String[] argv)
	 * 
	 * Input Parameters: 
	 * 		String[] argv: command line arguments
	 * 
	 * Returns:
	 * 		None.
	 * 
	 * Description:
	 * 		This is the main launching point for the program. It performs manual, programatic, and 
	 * random based testing.
	 */
	public static void main(String[] argv)
	{
		//create a UrlValidatorTest object to perform the test
		UrlValidatorTest urlTest = new UrlValidatorTest("Main Function URLValidatorTest Object"); 
		
		//run the manual based testing
		urlTest.testManualBasedTesting();
		
		//run the programatic based testing
		urlTest.testProgrammingBasedTesting();
		
		//run the random based testing
		urlTest.testRandomBasedTesting();
	}
	
	
	//***************************************************************************************************
	//************************** FOLLOWING VARIABLES ARE USED IN THE FUNCTIONS ABOVE ********************
	//***************************************************************************************************
	
	URLComponentToTest schemesToTest = new URLComponentToTest(new TestItem[]{
													//valid schemes
													new TestItem("http://",true),
													new TestItem("ftp://",true),
													new TestItem("https://",true),
													new TestItem("garbage://",true),
													new TestItem("blanket://", true),
													
													//invalid schemes
													new TestItem("http:/",false),
													new TestItem("http:",false),
													new TestItem("3ht://",false),
													new TestItem("://",false),
													new TestItem("apple",false),
													new TestItem("banana:",false),
													new TestItem("lkajsdlfoiwjef:///",false)
													
	});
	
	URLComponentToTest authoritiesToTest = new URLComponentToTest(new TestItem[]{
													//valid authorities
													new TestItem("www.google.com",true),
													new TestItem("google.com",true),
													new TestItem("oregonstate.edu",true),
													new TestItem("old.reddit.com",true),
													new TestItem("0.0.0.0",true),
													new TestItem("255.255.255.255",true),
													new TestItem("www.geeksforgeeks.org",true),
													new TestItem("[2001:db8:85a3:0:0:8a2e:370:7334]",true),
													new TestItem("[ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff]",true),
													
													//invalid authorities
													new TestItem("-1.-1.-1.-1",false),
													new TestItem("256.256.256.256",false),
													new TestItem("1.2.3.4.",false),
													new TestItem("1.2.3.4.5",false),
													new TestItem("go.a",false),
													new TestItem("aaa",false),
													new TestItem(".aaa",false),
													new TestItem("[-1:db8:85a3:0:0:-1:370:7334]",false),
													new TestItem("[fffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff]",false)
	});
	
	URLComponentToTest portsToTest = new URLComponentToTest(new TestItem[]{
													//valid ports
													new TestItem(":0",true),
													new TestItem(":80",true),
													new TestItem(":65535",true),
													new TestItem("",true),
													
													//invalid ports
													new TestItem(":-1",false),
													new TestItem(":65536",false),
													new TestItem(":65a",false),
													new TestItem(":abc",false)
			
	});
	
	URLComponentToTest pathsToTest = new URLComponentToTest(new TestItem[]{
													//valid paths
													new TestItem("/test1",true),
													new TestItem("/t123",true),
													new TestItem("/$23",true),
													new TestItem("/test1/",true),
													new TestItem("/test1/file",true),
													new TestItem("",true),
													
													//invalid paths
													new TestItem("/..",false),
													new TestItem("/../",false),
													new TestItem("/..//file",false),
													new TestItem("/test1//file",false)
	});
	
	URLComponentToTest queriesToTest = new URLComponentToTest(new TestItem[]{
													//valid queries
													new TestItem("?action=view",true),
													new TestItem("?action=edit&mode=up",true),
													new TestItem("",true)										
													
	});
	
	

}