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
function reset(){
	/*var numbers = ["input1","input2","input3","input4","input5"]; 
	var i;
	var count=5;
	for(i=0;i<count;i++)
		document.getElementById(numbers[i]).value="";*/
	document.getElementById("input1").value="";
	document.getElementById("input2").value="";
	document.getElementById("input3").value="";
	document.getElementById("input4").value="";
	document.getElementById("input5").value="";
	//document.getElementById("form1").reset();
	
}