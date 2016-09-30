<html>
<body>
<?php
phpinfo();
if( isset($_POST['submit']) )
if( isset($_POST['name'])&&isset($_POST['age']) )
{
$name = clean_input($_POST['name']);
$age = clean_input($_POST('age'));
	if( (strlen($name)<=30) && ($age<=120) )
	{
		if($age<10)
			$call = "young boy";
		else {
			if($age<30)
				$call = "young friend";
			else 
				$call = "gentleman";
		}
		echo "hello",$call,",","how are you today?";
	}
	else 
		echo "input wrongly!";
}
else 
	echo "please input";
?>

</body>
</html>
