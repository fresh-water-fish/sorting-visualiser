function validateActor(form) {
	var ok;
	var msg="";
	for (var i = 0; i < form.length; i++) {
		if (form.elements[i].value.trim() == "") {
			msg = msg + form.elements[i].name + "is void. ";
			ok=0
		}
	}
	if (ok == 1) return true;

	alert("alert:" msg);
	return false;
}
