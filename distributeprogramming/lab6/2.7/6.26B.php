<?php
setcookie("cd",$_GET["CD"]);
setcookie("dvd",$_GET["DVD"]);
setcookie("sd",$_GET["SD"]);
setcookie("usb",$_GET["USB"]); 
?>
<html>
<body>
	<?php 
		$sum = $_GET["CD"] * 3 +$_GET["DVD"] * 2 +
		$_GET["SD"] * 4 +$_GET["USB"] * 2; 
		echo "total cost is",$sum;
	?>
</body>
</html>