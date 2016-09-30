<html>
<head>

</head>
<body>
		<form action="7.1.1B.php" method="get">
		name<input type="text" id="name" name=¡°name¡± maxlength=10/><br>
		age(must be ranging from 0 to 199)<input type="text" id="age" name=¡°age¡± maxlength=10/><br>
		phone number<input type="text" id="number" name=¡°number¡± maxlength=10/><br>
		<input type="submit" value="submit" onClick=check()>
		</form>
	
		<script>
	function check(){
	var name=document.getElementById("name").value;
	var age = document.getElementById("age").value;
	var number = document.getElementById("number").value;
	var i;
	if(name==""||age==""||number==""){
		window.alert('input uncompletely');
		return;
		}
	if(name[0].toUpperCase() != name[0])
	{
		window.alert('First character must be upper case.');
		return;
	}
	for(i=1;i<name.length;i++)
	{
		if(name[i].toUpperCase() != name[i])
		{
			window.alert('Other characters must be lower case.');
			return;
		}
	}

	
	if( (parseInt(age)>199)||(parseInt(age)<0) ){
		window.alert('must be ranging from 0 to 199');
		return;
	}
	
	var subnumber=number.split("-");
	if(subnumber.length<2||subnumber.length>3){
		window.alert('number  error!');
		return;
	}
	if(subnumber[0].charAt(0)!='0'){
		window.alert('first number must be 0');
		return;
	}
	if(subnumber[1].length<6||subnumber[1].length>8)
		if( !((aubnumber[1].length==3)&&(subnumber.length==3)) ){
		window.alert('length limit!');
		return;
		}
		else{
			window.alert('success');
			return;
			}
	}
	</script>
</body>
</html>