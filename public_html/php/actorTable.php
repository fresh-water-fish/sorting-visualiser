<?php
	require 'login.php';

	$conn = new mysqli($db_host, $db_user, $db_pass, $db_name);
	if ($conn->connect_error) {
		die($conn->connect_error);
		echo "alert('The server cannot be connected to')";
	}

	$sql = "SELECT * FROM Actor ORDER BY actID ASC";
	//$stmt = $conn->prepare($sql);
	//$stmt->execute();
	//$stmt->bind_result($ID, $Title, $Price, $Year, $Genre);
	$result = $conn->query($sql);
	if ($result->num_rows > 0) {
		echo "<table class='table' border='1' cellpadding='1' cellspacing='1' style='background-color:#212121' style='color:#ff7043'><tr>";
		echo "<th class='actid'>ID</th>";
		echo "<th class='actname'>Name</th></tr>";
		while ($row = $result->fetch_assoc()) {
			echo "<tr><td class='actid'>".$row["actID"]."</td><td class='actname'>".$row["actName"]."</td></tr>";
		}
		echo "</table>";
	} else {
		echo "0 results";
	}

	$conn->close();

?>
