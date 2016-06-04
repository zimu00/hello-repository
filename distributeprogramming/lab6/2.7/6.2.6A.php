<html>
<body>
<form action="6.26B.php" method="get">
	<table>
		<tr>
			<td>product name</td> 
			<td>product description</td>	
			<td>product price</td>
			<td>quantity</td>
		</tr>
		<tr>
			<td>CD</td> 
			<td>good CD</td>	
			<td class="price">3</td>
			<td><input type="text" name="CD" value=
			<?php 
			if(isset($_COOKIE['cd']))
				echo $_COOKIE['cd'];
			else 
				echo 0;
			?>></td>
		</tr>
		<tr>
			<td>DVD</td> 
			<td>good DVD</td>	
			<td class="price">2</td>
			<td><input type="text" name="DVD" value=
			<?php 
			if(isset($_COOKIE['dvd']))
				echo $_COOKIE['dvd'];
			else 
				echo 0;
			?>></td>
		</tr>
		<tr>
			<td>SD memory</td> 
			<td>SD memory</td>	
			<td class="price">4</td>
			<td><input type="text" name="SD" value=
			<?php 
			if(isset($_COOKIE['sd']))
				echo $_COOKIE['sd'];
			else 
				echo 0;
			?>></td>
		</tr>
		<tr>
			<td>USB memory</td> 
			<td>USB memory</td>	
			<td class="price">2</td>
			<td><input type="text" name="USB" value=
			<?php 
			if(isset($_COOKIE["usb"]))
				echo $_COOKIE["usb"];
			else 
				echo 0;
			?>></td>
		</tr>
	</table>
	
	<input type="submit" value="select">
	</form>
</body>
</html>
