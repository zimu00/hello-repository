<html><head>
<meta charset="UTF-8">
<title>Insert title here</title>
</head>
<body>
<form action="<?php echo $_SERVER['PHP_SELF'];?>" method="post">
name<input type="text" name="name" />&nbsp;
age<input type="text" name="age" /><br><br>
<input type="submit" >
<input type="reset">
</form>


<?php
if( isset($_POST['submit']) ){
	echo "press submited";
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
}}
else 
	echo "please input";
?>

</body>
</html>