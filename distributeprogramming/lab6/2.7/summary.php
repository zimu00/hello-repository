<?php 
//if(isset($_REQUEST['banana']))
if(isset($_REQUEST['banana']) && $_REQUEST['banana']!=0)
	setcookie("banana",0.5);
//if(isset($_REQUEST['car']))
if(isset($_REQUEST['car'])  && $_REQUEST['car']!=0)
	setcookie("car",5000);
//if(isset($_REQUEST['bike']))
if(isset($_REQUEST['bike'])  && $_REQUEST['bike']!=0)
	setcookie("bike",50);
?>
<html>
<body>
	<form action="buy.php" method="get">
		<table>
			<tr>
			<td>product</td>
			<td>quantity</td>
			</tr>
			<?php 
			if(isset($_REQUEST['banana']) && $_REQUEST['banana']!=0)
			{
				$count1=$_REQUEST['banana'];
				echo "<tr><td>banana</td><td><input type='text' name='banana1' value="
					,$count1,"></td></tr>";
			}
			/*else
				echo "<tr><td>banana</td><td><input type='text' name='banana1' value="
					,0,"></td></tr>";*/
			if(isset($_REQUEST['car'])  && $_REQUEST['car']!=0)
			{
				$count2=$_REQUEST['car'];
				echo "<tr><td>car</td><td><input type='text' name='car1' value="
					,$count2,"></td></tr>";
			}
			/*else echo "<tr><td>banana</td><td><input type='text' name='car1' value="
					,0,"></td></tr>";*/
			if(isset($_REQUEST['bike'])  && $_REQUEST['bike']!=0)
			{
				$count3=$_REQUEST['bike'];
				echo "<tr><td>bike</td><td><input type='text' name='bike1' value="
					,$count3,"></td></tr>";
			}/*else 
				echo "<tr><td>banana</td><td><input type='text' name='bike1' value="
					,0,"></td></tr>";*/
			
			?>
		</table>
		<input type="submit" value="Buy">
		<a href="6.2.7A">return to buy more!</a>
	</form>
</body>
</html>
