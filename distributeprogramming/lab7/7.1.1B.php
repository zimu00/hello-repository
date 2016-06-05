<html>
<body>
	
	<?php 
	function check(){
		$name=$_REQUEST['name'];
		$age = $_REQUEST['age'];
		$number = $_REQUEST['number'];
		$subnumber = explode("-", $number);
		echo $name,"<br>",$age,"<br>",$number,"<br>";
		//check name
		if($name[0]!=strtoupper($name))
			echo "first letter mush be capital!";
		else {
			$k=1;
			$i = strlen($name);
			if($i<11){
			for($j=0;$j<$i;$j++)
			{
				if($name[$j]===strtoupper($name[j])){
					echo "from second letter must be lower case";
					$k=0;
					break;}
			}
			if($k==1)
				echo "name is:",$name;
			}
			else 
				echo "name can't have more than 10 characters!";
			
		}
		//check age
		if($age<199&&$age>0)
			echo "age is:",$sge;
		else 
			echo "error in age format";
		if(count($subnumber)<2||count($subnumber)>3)
			echo "error in -";
		else {
			if(strlen($subnumber[0])<2 || strlen($subnumber[0]>3))
				echo "error in length";
			if($subnumber[0][0]!='0' || !ctype_digit($subnumber[0]) )
			echo "error in digit";
			if(count($subnumber)==2)
			{
				if(strlen($subnumber[1])<6||strlen($subnumber)>7)
					echo "error in number length";
				else 
					echo "the number is :",$number;
			}
			else{
				if(strlen($subnumber[1])!=3)
					echo "error in number length";
				echo "the number is :",$number;
			}
		}
		}
		?>
</body>
</html>