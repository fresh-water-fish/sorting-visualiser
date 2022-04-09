<!DOCTYPE html>
<html>
	<head>
		<title>Coursework</title>
		<meta name="author" content="Sebastian Kjallgren">
		<meta name="keywords" content="Web,HTML,CSS,PHP,SQL">
		<meta name="viewport" content="width=device-width, initial-scale=1">
		<link rel="stylesheet" type="text/css" href="../css/form.css">
	</head>

	<body>
		<?php require './returnBtn.php'; ?>
		<p>Trying to add an actor to the database...</p>
		<?php
			if (!isset($_GET['submit'])) {
				echo "<p>Could not get value for actor name</p>";
				die();
			}
		
			require './login.php';

			$actName = $_GET['aname'];
		
			//$conn = mysqli_connect($db_host, $db_user, $db_pass, $db_name);
			$conn = new mysqli($db_host, $db_user, $db_pass, $db_name);

			if ($conn->connect_error) {
				echo "alert('could not connect');";
				die('failed to connect to the database\n</body></html>');
			}

			echo "<script>alert(".$actName.");</script>";

			$sql = "INSERT INTO Actor (actName) VALUES ('".$actName."')";

			if (mysqli_query($conn,$sql)) {
				echo "<p>Data inserted successfully</p>";
			} else {
				echo "<p>Data was unsuccessfully inserted:".$conn->connect_error."</p>";
			}
		
			/*if($conn->query($sql) === TRUE) {
				echo 'alert("Actor added!");'; 
			} else {
				echo 'alert("The actor could not be added to the database");';
			}*/
		
			$conn->close();
	?>
	</body>
</html>
