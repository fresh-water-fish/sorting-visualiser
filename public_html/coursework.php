<!DOCTYPE html>
<html>
	<head>
		<title>Coursework</title>
		<meta name="author" content="Sebastian Kjallgren">
		<meta name="keywords" content="Web,HTML,CSS,PHP,SQL">
		<meta name="viewport" content="width=device-width, initial-scale=1">
		<link rel="stylesheet" type="text/css" href="css/sidebar.css">
		<link rel="stylesheet" type="text/css" href="css/style.css">
		<script async src="js/init.js"></script>
		<script async src="js/sidebarFuncs.js"></script>
		<script async src="js/add.js"></script>
		<script async src="js/delete.js"></script>
		<script async src="js/updateRestricts.js"></script>
		<script async src="js/validateFuncs.js"></script>

		<?php
			require_once 'php/login.php';
			
			$conn = new mysqli($db_host, $db_user, $db_pass, $db_name);
			if ($conn->connect_error) {
				die($conn->connect_error);
				echo "alert('The server cannot be connected to')";
			}

			$conn->close();
		?>
	</head>

	<body>
		<div class="mainheader">
			<center>
				<h1>Databases and Interfaces Coursework</h1>
				<br />
			</center>
		</div>

		<!-- Side bar with buttons and checkboxes -->
		<div class="sidenav">
			<!-- <button type="button" onclick="alert('Hello world!')">Click Me!</button> -->
			<center>
				<button type="button" class="sidebutton switch-button" id="switch-button" onclick="switchTables(this);">Switch to Movies</button><br /><br />
				<hr><br />
				<button type="button"  class="sidebutton add-button" id="add-button" onclick="addClicked();">Add</button><br /><br />
				<hr><br /><br />
				<button type="button" class="sidebutton delete-button" id="delete-button" onclick="delClicked();">Delete</button><br /><br />
				<hr><br />
				<hr><br />
				<hr><br />
				<hr><br /><br />
				<button type="submit" class="sidebutton submit-button" id="submit-button">Hide Column</button><br /><br />
			</center>

			<div id="actor-buttons" class="actor-buttons">
				<input id="actor-id-button" type="checkbox" name="actor-id-button" onclick="upRestrict(this, 'actid');">
				<label for"actor-id-button">ID</label>
				<br />
				<input id="actor-name-button" type="checkbox" name="actor-name-button" onclick="upRestrict(this, 'actName');">
				<label for"actor-name-button">Name</label>
				<br />
			</div>
			<div id="movie-buttons" class="movie-buttons">
				<input id="movie-actid-button" onclick="upRestrict(this, 'movactid');" type="checkbox" name="movie-actid-button">
				<label for="movie-actid-button">Actor</label>
				<br />
				<input id="movie-movid-button" onclick="upRestrict(this, 'movid');" type="checkbox" name="movie-movid-button">
				<label for="movie-movid-button">Movie ID</label>
				<br />
				<input id="movie-title-button" onclick="upRestrict(this, 'movtitle');" type="checkbox" name="movie-title-button">
				<label for="movie-title-button">Title</label>
				<br />
				<input id="movie-price-button" onclick="upRestrict(this, 'movprice');" type="checkbox" name="movie-price-button">
				<label for="movie-price-button">Price</label>
				<br />
				<input id="movie-genre-button" onclick="upRestrict(this, 'movgenre');" type="checkbox" name="movie-genre-button">
				<label for="movie-genre-button">Genre</label>
				<br />
				<input id="movie-year-button" onclick="upRestrict(this, 'movyear');" type="checkbox" name="movie-year-button">
				<label for="movie-year-button">Year</label>
				<br />
			</div>

		</div>
		
		<div class="main">
			
			<div id="actor-table">
				<?php
					require 'php/actorTable.php';
				?>
			</div>
			<div id="movie-table" class="start-hidden">
				<?php
					require 'php/movieTable.php';
				?>
			</div>
			<div id="add-del">
			</div>
		</div>
	</body>
</html>
