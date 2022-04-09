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
			if (empty($_GET['movname'])) {
				$errmsg = $errmsg." is required.\n";
			}
			if (empty($_GET['actid'])) {
				$errmsg = $errmsg." is required.\n";
			}
			if (empty($_GET['movgenre'])) {
				$errmsg = $errmsg." is required.\n";
			}
			if (empty($_GET['movprice'])) {
				$errmsg = $errmsg." is required.\n";
			}
			if (empty($_GET['movyear'])) {
				$errmsg = $errmsg." is required.\n";
			}

			require './login.php';

			$movName = $_GET['movname'];
			$actId = $_GET['actid'];
			$movGenre = $_GET['movgenre'];
			$movPrice = $_GET['movprice'];
			$movYear = $_GET['movyear'];
		
			$conn = new mysqli($db_host, $db_user, $db_pass, $db_name);

			if ($conn->connect_error) {
				echo "<script>alert('could not connect');</script>";
				die('failed to connect to the database\n</body></html>');
			}

			$sql = "INSERT INTO Movie (actID, movTitle, movPrice, movYear, movGenre) VALUES ('".$actId."','".$movName."','".$movPrice."','".$movYear."','".$movGenre."')";

			if (mysqli_query($conn,$sql)) {
				echo "<p>Data inserted successfully</p>";
			} else {
				echo "<p>Data was unsuccessfully inserted:".$conn->connect_error."</p>";
			}
		
			$conn->close();
	?>
	</body>
</html>
