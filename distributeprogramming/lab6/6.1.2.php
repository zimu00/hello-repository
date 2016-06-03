<html>
<body>
<table>
<tr>
<th>square</th>
<th>cube power</th>
</tr>
<?php
if(isset( $_POST['submit']))
if (isset($_POST["number"])&&($_POST["number"]>0))
{
	$x = $_POST['number'];
	echo $x;
	echo "The number is correct<br>";
	for($i=1;$i<=$x;$i++){
		$j = $i * $i;
		$k = $j*$i;
		echo("<tr><td>".$j."</td><td>".$k."</td></tr>");
	}
}
else
	echo "The number invalid!<br>";

?>
</table>
</body>
</html>