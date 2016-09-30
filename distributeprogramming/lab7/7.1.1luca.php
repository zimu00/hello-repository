<!DOCTYPE html>
<html lang="it">
  <head>
    <meta content="text/html; charset=utf-8" http-equiv="content-type">
    <title>Esercizio 7.1.1</title>
  </head>
  
  <body>
    <?php
      function check() {
	// Controlla il nome
	$nome = $_GET["nome"];
	if (strlen($nome) > 10)
	  return false; // più di 10 caratteri
	if (!ctype_alpha($nome))
	  return false; // non alfabetica
	if (!ctype_upper($nome[0]) || !ctype_lower(mb_substr($nome, 1)))
	  return false; // case non corretto
	  
	// Controlla l'età
	$eta = $_GET["eta"];
	if (!ctype_digit($eta))
	  return false;
	if ($eta < 0 || $eta > 199)
	  return false;
	  
	// Controlla il numero di telefono
	$telefono = $_GET["telefono"];
	$substrings = explode("-", $telefono);
	if (count($substrings) < 2 || count($substrings) > 3)
	  return false;
	if (strlen($substrings[0]) < 2 || strlen($substrings[0]) > 3)
	  return false;
	if ($substrings[0][0] != '0' || !ctype_digit($substrings[0]))
	  return false;
	if (!ctype_digit($substrings[1]))
	  return false;
	if (count($substrings) == 2) {
	  if (strlen($substrings[1]) < 6 || strlen($substrings[1]) > 7)
	    return false;
	} else { // strlen($substrings) == 3
	if (strlen($substrings[1]) != 3)
	    return false;
	  if (!ctype_digit($substrings[2]))
	    return false;
	  if (strlen($substrings[2]) < 3 || strlen($substrings[2]) > 4)
	    return false;
	}
	
	return true;
      }
    
      if ($_GET["datierrati"] == "si" || !check()) {
    ?>
    <h1>Dati errati</h1>
    <div><button onclick="javascript:window.location.assign('A.html');">Torna</div>
    <?php
      } else {
    ?>
     <h1>Dati corretti</h1>
    <p>Nome: <?php
      echo $_GET["nome"];
    ?></p>
    <p>Età: <?php
      echo $_GET["eta"];
    ?></p>
    <p>Numero di telefono: <?php
      echo $_GET["telefono"];
    ?></p>
    <?php
      }
    ?>
  </body>
</html>