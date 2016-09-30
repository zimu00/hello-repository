<?php

$number1 = $_REQUEST["n1"];
$number2 = $_REQUEST["n2"];
if($number1=="" || $number2==""){
	echo "Please input";
	header('HTTP/1.1 307');
	header('Location:' ."6.1.5html.php");
}
else {
	$op = $_REQUEST["operation"];
	switch($op){
		case '+':
			$result = $number1 + $number2;
			break;
		case '-':
			$result = $number1 - $number2;
			break;
		case '*':
			$result = $number1 * $number2;
			break;
		case '/':
			$result = $number1 /$number2;
			break;

	}
	echo "the result of ",$op," for ",$number1," and",$number2," is ",$result;
}
?>