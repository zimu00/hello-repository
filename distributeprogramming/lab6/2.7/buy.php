<html>
<body>
	<form action="conclude.php" method="get">
		<table>
			<tr>
			<td>product</td>
			<td>quantity</td>
			</tr>
			<?php 
			$sum =0;
			//if(($_REQUEST["banana1"]!=0))
			if(isset($_COOKIE['banana']))//if has cookie,means it has value
			{
				$count1=$_REQUEST['banana1'];
				echo "<tr><td>banana</td><td><input type='text' name='banana1' value="
					,$count1,"></td><td>",$count1*0.5,"</td></tr>";
				$sum+=$count1*0.5;
			}
			
			//if(($_REQUEST["car1"]!=0))
			if(isset($_COOKIE['car']))
			{
				$count2=$_REQUEST['car1'];
				echo "<tr><td>car</td><td><input type='text' name='car1' value="
					,$count2,"></td><td>",$count2*5000,"</td></tr>";
				$sum+=$count2*5000;
			}
			//if(($_REQUEST["bike1"]!=0))
			if(isset($_COOKIE['bike']))
			{
				$count3=$_REQUEST['bike1'];
				echo "<tr><td>bike</td><td><input type='text' name='bike1' value="
					,$count3,"></td><td>",$count3*50,"</td></tr>";
				$sum+=$count3*50;
			}
			echo "total cost is:",$sum;
			?>
		</table>
		<input type="submit" value="Conclude">
		<a href="6.2.7A.php">return to buy more!</a>
	</form>
</body>
</html>
