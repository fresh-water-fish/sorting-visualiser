function switchTables(buttonprop) {
	document.getElementById("add-del").innerHTML = "";
	if (buttonprop.innerHTML === "Switch to Actors") {
		buttonprop.innerHTML = "Switch to Movies";
		switchMethod(true);
	} else if (buttonprop.innerHTML === "Switch to Movies") {
		// php request for Movies
		buttonprop.innerHTML = "Switch to Actors";
		switchMethod(false);
	} else {
		console.log("Error: switch tables button does not include the word actor/movies");
		// php request for Actors
		// default to showing actors
		buttonprop.innerHTML = "Switch to Movies";
		switchMethod(true);
	}
}

function switchMethod(bool) {
	actordiv = document.getElementById("actor-buttons");
	moviediv = document.getElementById("movie-buttons");

	//tablediv = document.getElementById("actmov-table");
	actorT = document.getElementById("actor-table");
	movieT = document.getElementById("movie-table");

	/* if true, show actor checkboxes */
	if (bool) { 
		hideSmt(moviediv);
		hideSmt(movieT);

		showSmt(actordiv);
		showSmt(actorT);
	} else {
		hideSmt(actordiv);
		hideSmt(actorT);

		showSmt(moviediv);
		showSmt(movieT);
	}
}

function hideSmt(hideme) {
	hideme.style.display = "none";
	hideme.style.visibility = "hidden";
}
function showSmt(showme) {
	showme.style.display = "block";
	showme.style.visibility = "visible";
}

