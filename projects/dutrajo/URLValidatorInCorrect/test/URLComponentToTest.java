/*
 * File: URLComponentToTest.java
 * Created by: Joseph Dutra, Samantha Manubay, Myung Jin Song
 * Created Date: 03/02/2019
 * Class: CS362 - 400 - Winter 2019
 * Project: Final Project Part-B
 * Contains: Class URLComponentToTest
 */

/*
 * Class URLComponentToTest
 * 
 * Description:
 * This class is used as a container object. It can be used to store and iterate through 
 * a collection of TestItem class objects.
 */
public class URLComponentToTest
{
		//array of TestItem objects that are stored and iterated over
		TestItem[] itemsToTry;
		
		//index value used to keep track of which object container
		//has currently iterated to.
		int currentItemIndex;
		
		/*
		 * public URLComponentToTest(TestItem[] inputItems)
		 * 
		 * Input Parameters: 
		 * 		TestItem[] inputItems: array of TestItem objects to store
		 * 
		 * Returns:
		 * 		Nothing.
		 * 
		 * Description:
		 * 		This is the only constructor for the URLComponentToTest class. The 
		 * item parameter array gets copied into the itemsToTry array variable. It
		 * also sets the currentItemIndex variable to zero.
		 */
		public URLComponentToTest(TestItem[] inputItems)
		{
			//set the length of the itemsToTry array variable
			this.itemsToTry = new TestItem[inputItems.length];
			
			//iterate through the input parameter array, and copy into
			//local array variable
			for(int i=0; i < inputItems.length; i++)
			{
				//create a new TestItem object and store it in the
				//local itemsToTry variable
				this.itemsToTry[i] = 
						new TestItem(inputItems[i].urlComponent, inputItems[i].isValid);
			}
			
			//set the currentItemIndex to zero
			this.currentItemIndex = 0;
		}
		
		/*
		 * public boolean hasMoreTests()
		 * 
		 * Input Parameters: 
		 * 		None.
		 * 
		 * Returns:
		 * 		A boolean representing if the are more items to iterate over
		 * 
		 * Description:
		 * 		Function returns true if there are more objects in the
		 * itemsToTry variable to iterate over, otherwise it returns false.
		 */
		public boolean hasMoreTests()
		{
			//check if we've iterated past the array length
			if(this.currentItemIndex < itemsToTry.length)
			{
				//still good, return true
				return true;
			}
			
			//nothing left, return false
			return false;
		}
		
		/*
		 * public void reinitItemsToTry()
		 * 
		 * Input Parameters: 
		 * 		None.
		 * 
		 * Returns:
		 * 		None.
		 * 
		 * Description:
		 * 		Function sets current iterative index to zero, in
		 * a sense it resets the iteration back to zero.
		 */
		public void reinitItemsToTry()
		{
			//set to zero
			this.currentItemIndex = 0;
		}
		
		/*
		 * public String getCurItem()
		 * 
		 * Input Parameters: 
		 * 		None.
		 * 
		 * Returns:
		 * 		String: The TestItem object string value at the current 
		 * iterative index in the itemsToTry array.
		 * 
		 * Description:
		 * 		Function gets the current TestItem object from the
		 * itemsToTry array and returns the string component that it stores.
		 */
		public String getCurItem()
		{
			//get the string value from the current index
			return this.itemsToTry[this.currentItemIndex].urlComponent;
		}
		
		/*
		 * public String getCurValid()
		 * 
		 * Input Parameters: 
		 * 		None.
		 * 
		 * Returns:
		 * 		boolean: The TestItem object boolean value at the current 
		 * iterative index in the itemsToTry array.
		 * 
		 * Description:
		 * 		Function gets the current TestItem object from the
		 * itemsToTry array and returns the boolean component that it stores.
		 */
		public boolean getCurValid()
		{
			//get the boolean value from the current index object
			return this.itemsToTry[this.currentItemIndex].isValid;
		}
		
		/*
		 * public String incr()
		 * 
		 * Input Parameters: 
		 * 		None.
		 * 
		 * Returns:
		 * 		None.
		 * 
		 * Description:
		 * 		Function increments the index counter variable store
		 * in the class container object.
		 */
		public void incr()
		{
			//increment index counter
			this.currentItemIndex++;
		}
}