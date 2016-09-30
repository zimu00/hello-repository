/**
 * 
 */
function greatest(){
	var n = 0;
	var temp;
	var i;
	var count=5;
	var numbers = ["input1","input2","input3","input4","input5"]; 
	
	n = document.getElementById(numbers[0]).value;//to prevent all are negative numbers
	//document.write("<p>"+n+"</p>");
	for(i=1;i<count;i++)
	{
		temp = document.getElementById(numbers[i]).value;
		if(temp!="")
		{
			if(n<=temp)
				n=temp;
		}
		
	}
	alert("The greatest value is"+n);
}