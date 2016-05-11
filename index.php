<!doctype html>
	<html lang="nl">
	<charset="utf-8"/>
	<head>
		<link rel = "stylesheet"href="styles.css"/>
		<title> </title>
	</head>

	<body>
		<?php
			require_once 'db_config.php';
		?>


		<?php
			$sql = "SELECT data FROM data WHERE datatype = 'temperature' ORDER BY timestamp DESC LIMIT 1";
			if(!$res = mysqli_query($link,$sql))
			{
				trigger_error('Fout in query: '.mysqli_error());
			}
			else
			{
				echo "Temperature: ";
				while($row = mysqli_fetch_assoc($res))
				{
					echo $row['data'].'<br>';
				}
			}
		?>
		<br></br>
		<img src="local.png"width="500"height="500"alt="QR code"class="qr"/>
	</body>
</html>
