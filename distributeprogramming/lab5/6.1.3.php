<!DOCTYPE html>
<html lang="it">
  <head>
    <meta content="text/html; charset=utf-8" http-equiv="content-type">
    <title>Esercizio 6.1.3</title>
  </head>
  
  <body>
    <table>
      <tr>
	<th>Input</th>
	<th>Value</th>
      </tr>
  
      <?php
	function stampaArray($a) {
	  $c = count($a);
	  for ($i = 0; $i < $c; ++$i) {
	    $elem = each($a);
	    echo("<tr><td>" . $elem['key'] . "</td><td>" . $elem['value'] . "</td></tr>");
	  }
	}
      
	stampaArray($_REQUEST);
	stampaArray($_SERVER);
      ?>
    </table>
  </body>
</html>