way1:
<?php

var_dump($_COOKIE);

?>

way2:
<!DOCTYPE html>
<html lang="it">
  <head>
    <meta content="text/html; charset=utf-8" http-equiv="content-type">
    <title>Esercizio 6.2.2</title>
  </head>
  
  <body>
    <table>
      <tr>
	<th>Campo</th>
	<th>Valore</th>
      </tr>
      
      <?php
	function stampaArray($a) {
	  $c = count($a);
	  for ($i = 0; $i < $c; ++$i) {
	    $elem = each($a);
	    echo("<tr><td>" . $elem['key'] . "</td><td>" . $elem['value'] . "</td></tr>");
	  }
	}
	stampaArray($_COOKIE);
      ?>
    </table>
  </body>
</html> 
