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
			$conn = new mysqli($db_host, $db_user, $db_pass, $db_name);

			$actId = $_GET['actid'];

			if ($conn->connect_error) {
				echo "alert('could not connect');";
				die('failed to connect to the database\n</body></html>');
			}

			$sql = "DELETE FROM Actor WHERE actID='".$actId."'";

			if (mysqli_query($conn,$sql)) {
				echo "<p>Actor deleted successfully</p>";
			} else {
				echo "<p>Actor was unsuccessfully deleted:".$conn->connect_error."</p>";
			}
			
			$conn->close();
	?>
	</body>
</html>
