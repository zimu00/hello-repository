<?php 
//s$x = "cook";
if( !isset($_COOKIE[$_REQUEST['name']."-".$_REQUEST['surname']]) ){
//if( !isset($x) ){
$x = 'cook';
setcookie($_REQUEST['name']."-".$_REQUEST['surname'],$x);}
?>
<html>
<body>
<?php 

if( isset($_COOKIE[$_REQUEST['name']."-".$_REQUEST['surname']]) )
//if( isset($_COOKIE[$x]) )
{
//$x = $_COOKIE['name'];
//$y = $_COOKIE['surname'];
echo "welcome:",$_REQUEST['name']."-".$_REQUEST['surname'];
}
else 
{
	echo "welcome to visit my website for first time,if u refresh the page,
		u will find the name and surname";
}
?>
</body>
</html>