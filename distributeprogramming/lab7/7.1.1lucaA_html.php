<!DOCTYPE html>
<html lang="it">
  <head>
    <meta content="text/html; charset=utf-8" http-equiv="content-type">
    <title>Esercizio 7.1.1</title>
    <script src="http://ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js"></script>
  </head>
  
  <body>
    <script>
      function check_inner() {
	// Controlla il nome
	var nome = document.getElementById("nome").value;
	if (nome.length > 10)
	  return false; // pi¨´ di 10 caratteri
	var re = /[A-Z][a-z]*/;
	if (!re.test(nome))
	  return false; // non alfabetica
	if (nome != nome.charAt(0).toUpperCase() + nome.toLowerCase().slice(1))
	  return false; // case non corretto
	  
	// Controlla l'et¨¤
	var eta = document.getElementById("eta").value;
	if (isNaN(eta))
	  return false;
	if (parseInt(eta) < 0 || parseInt(eta) > 199)
	  return false;
	  
	// Controlla il numero di telefono
	var telefono = document.getElementById("telefono").value;
	var substrings = telefono.split("-");
	if (substrings.length < 2 || substrings.length > 3)
	  return false;
	if (substrings[0].length < 2 || substrings[0].length > 3)
	  return false;
	if (substrings[0].charAt(0) != '0' || isNaN(substrings[0]))
	  return false;
	if (isNaN(substrings[1]))
	  return false;
	if (substrings.length == 2) {
	  if (substrings[1].length < 6 || substrings[1].length > 7)
	    return false;
	} else { // substrings.length == 3
	if (substrings[1].length != 3)
	    return false;
	  if (isNaN(substrings[2]))
	    return false;
	  if (substrings[2].length < 3 || substrings[2].length > 4)
	    return false;
	}
	
	return true;
      }
      
      function check() {
	if (!check_inner())
	  document.getElementById("datierrati").value = "si";
	else
	  document.getElementById("datierrati").value = "no";
      }
    </script>
  
    <form action="7.1.1luca.php" method="get">
      <div>Nome: <input type="text" name="nome" id="nome"></div>
      <div>Et¨¤: <input type="text" name="eta" id="eta"></div>
      <div>Numero di telefono: <input type="text" name="telefono" id="telefono"></div>
      <div><input type="submit" value="Invia" onclick="javascript:check();"></div>
      <input type="hidden" value="" id="datierrati" name="datierrati">
    </form>
  </body>
</html>