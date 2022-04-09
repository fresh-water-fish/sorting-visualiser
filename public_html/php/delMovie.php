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
		<p>Trying to add a movie to the database...</p>
		<?php
			$errmsg = "";
			if (!isset($_GET['submit'])) {
				echo "<p>Could not get value for actor name</p>";
				die();
			}

			/* validating requirements */
			if (empty($_GET['movid'])) {
				echo "<p>Movie ID was not provided</p>";
				die();
			}

			$movId = $_GET['movid'];
		
			require './login.php';
			$conn = new mysqli($db_host, $db_user, $db_pass, $db_name);

			if ($conn->connect_error) {
				echo "<script>alert('could not connect');</script>";
				die('failed to connect to the database\n</body></html>');
			}

			$sql = "DELETE FROM Movie WHERE movID='".$movId."'";

			if (mysqli_query($conn,$sql)) {
				echo "<p>Movie deleted successfully</p>";
			} else {
				echo "<p>Movie was unsuccessfully deleted:".$conn->connect_error."</p>";
			}
		
			$conn->close();
	?>
	</body>
</html>
