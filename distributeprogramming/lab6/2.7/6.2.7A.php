<?php
// unset cookies
if (isset($_SERVER['HTTP_COOKIE'])) {
    $cookies = explode(';', $_SERVER['HTTP_COOKIE']);
    foreach($cookies as $cookie) {
        $parts = explode('=', $cookie);
        $name = trim($parts[0]);
        setcookie($name, '', time()-1000);
        setcookie($name, '', time()-1000, '/');
    }
}
?>
<html>
<body>
<form action="summary.php" method="post">
	<table>
		<tr>
			<td><a href="bike.php">bike</a></td>
			<td><input type="text" name="bike" value="0"></td>
		</tr>
		<tr>
			<td><a href="car.php">car</a></td>
			<td><input type="text" name="car" value="0"></td>
		</tr>
		<tr>
			<td><a href="banana.php">banana</a></td>
			<td><input type="text" name="banana" value="0"></td>
		</tr>
	</table>
	
		<input type="submit" value="summary">
	</form>
</body>
</html>