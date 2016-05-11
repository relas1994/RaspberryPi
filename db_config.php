<?php 
	$link = mysqli_connect('localhost', 'root', 'raspberry', 'data');
	if(!$link)
	{
	    trigger_error('Fout bij verbinden met database: '.mysqli_connect_error());
	}
?>

