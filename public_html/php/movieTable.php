<?php
	require 'login.php';

	$conn = new mysqli($db_host, $db_user, $db_pass, $db_name);
	if ($conn->connect_error) {
		die($conn->connect_error);
		echo "alert('The server cannot be connected to')";
	}

	//$sql = "SELECT Movie.*, Actor.actName FROM Movie JOIN Actor ON Movie.actID = Actor.actID";
	$sql = "SELECT Movie.movID, Movie.movTitle, Movie.movPrice, Movie.movYear, Movie.movGenre, Actor.actName FROM Movie INNER JOIN Actor ON Movie.actID = Actor.actID ORDER BY Movie.movID ASC";
	//$stmt = $conn->prepare($sql);
	//$stmt->execute();
	//$stmt->bind_result($ID, $Title, $Price, $Year, $Genre);
	$result = $conn->query($sql);
	if ($result->num_rows > 0) {
		echo "<table class='table' border='1' cellpadding='1' cellspacing='1' style='background-color:#212121' style='color:#ff7043'><tr>";
		echo "<th class='movid'>ID</th>";
		echo "<th class='movactid'>Actor</th>";
		echo "<th class='movtitle'>Title</th>";
		echo "<th class='movprice'>Price</th>";
		echo "<th class='movyear'>Year</th>";
		echo "<th class='movgenre'>Genre</th></tr>";
		while ($row = $result->fetch_assoc()) {
			echo "<tr><td class='movid'>".$row["movID"]."</td><td class='movactid'>".$row["actName"]."</td><td class='movtitle'>".$row["movTitle"]."</td><td class='movprice'>".$row["movPrice"]."</td><td class='movyear'>".$row["movYear"]."</td><td class='movgenre'>".$row["movGenre"]."</td></tr>";
		}
		echo "</table>";
	} else {
		echo "0 results";
	}

	$conn->close();

?>
