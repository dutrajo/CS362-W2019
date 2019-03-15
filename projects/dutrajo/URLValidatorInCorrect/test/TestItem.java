/*
 * File: TestItem.java
 * Created by: Joseph Dutra, Samantha Manubay, Myung Jin Song
 * Created Date: 03/02/2019
 * Class: CS362 - 400 - Winter 2019
 * Project: Final Project Part-B
 * Contains: Class TestItem
 */


/*
 * Class TestItem
 * 
 * Description:
 * This class is used to create objects that contain a string and a boolean value. Its
 * original intent was to hold a URL Component and a boolean to indicate if that component
 * was a valid component or not. It can also be used to indicate if an entire URL (or any string)
 * is valid or not.
 */
public class TestItem
{
	
	String urlComponent; //string item
	boolean isValid; //indicator if item is valid or not
	
	/*
	 * public TestItem(String item, Boolean valid)
	 * 
	 * Input Parameters: 
	 * 		String item: string value to store in object
	 * 		Boolean valid: boolean value to store in object
	 * 
	 * Returns:
	 * 		Nothing.
	 * 
	 * Description:
	 * 		This is the only constructor for the TestItem class. The item parameter gets
	 * stored as the urlComponent variable, and the valid parameter gets stored as the 
	 * isValid variable. 
	 */
	public TestItem(String item, Boolean valid)
	{
		//set the class variables to the input parameters
		this.urlComponent = item;
		this.isValid = valid;
	}
}
