// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {
	//Names of files stored in uploads
	var obj = {};
	var fileNames = new Array();
	document.getElementById("dbLogout").disabled = true;
	document.getElementById("dbFile").disabled = true;
	document.getElementById("dbClear").disabled = true;
	document.getElementById("dbDisplay").disabled = true;
	document.getElementById("dbQ").disabled = true;

	
	//Get all files stored in the database
	var xhrStart = new XMLHttpRequest();
	xhrStart.open('get', '/upload', true);
	//Once all stored files are retrieved, add them to the table
	xhrStart.onload = function(){
		fileNames = JSON.parse(xhrStart.response);
		//After file names have been added
		
		for(let i = 0; i < fileNames.length; i++){
			let xhrOut = new XMLHttpRequest();
			xhrOut.open('get', '/upload/' + fileNames[i], true);
			xhrOut.onerror = function(res){
				console.log(xhrOut.response);
			}
			//When the file has been retrieved
			xhrOut.onload = function(){
				console.log("File successfully retrieved from /uploads");
				if(xhrOut.response === '{}'){
					console.log("Invalid file");
				}
				else{
					//Add file name to GPX View options
					let x = document.getElementById("docSelect");
					let option = document.createElement("option");
					option.text = fileNames[i];
					x.add(option);
					obj = JSON.parse(xhrOut.response);
					//Add a new row to the table
					let table = document.getElementById("FilePanel");
					let row = table.insertRow(-1);
					let cell1 = row.insertCell(0);
					let cell2 = row.insertCell(1);
					let cell3 = row.insertCell(2);
					let cell4 = row.insertCell(3);
					let cell5 = row.insertCell(4);
					let cell6 = row.insertCell(5);
				
					//Create the path for the file download
					var path = "/uploads/"+fileNames[i];

				
					//Create the hyperlink
					cell1.innerHTML = "<a href='" + path + "'>" + fileNames[i]; + "</a>";
					cell2.innerHTML = obj.version;
					cell3.innerHTML = obj.creator;
					cell4.innerHTML = obj.numWaypoints;
					cell5.innerHTML = obj.numRoutes;
					cell6.innerHTML = obj.numTracks;
					
					if(document.getElementById("docSelect").length > 0){
						let t = document.getElementById("doesntExist");
						t.innerText = "";
					}
				}	
			}
			xhrOut.send();
		}
		
	}
	xhrStart.onerror = function(){
		console.log("Load error");
	}
	
	xhrStart.send();
	


	
	
	
    $('#dwnldBtn').click(function(){
	//If there is a file being uploaded
	if($('#upload').prop('files').length > 0){
		let fileInput = document.getElementById("upload");
		let file = fileInput.files[0];
		if(!file){
			console.log("File is empty");
		}
		else{
			if(file.name.split('.').pop() != "gpx"){
				alert("Invalid File Type");
			}
			else if(fileNames.includes(file.name)){
				alert("There is already a file with this name");
			}
			else{
				//Add file name to list of files
				fileNames.push(file.name);
			
				let formData = new FormData();
				formData.append('uploadFile', file, file.name);				
				
				var xhrIn = new XMLHttpRequest();
				xhrIn.open('post', '/upload', true);
				xhrIn.onerror = function(){
					console.log("Error uploading file");
				};
				//When file has been fully downloaded
				xhrIn.onload = function(){
					console.log("File successfully uploaded to /uploads");
					let xhrOut = new XMLHttpRequest();
					xhrOut.open('get', '/upload/' + file.name, true);
					xhrOut.onerror = function(res){
						console.log(xhrOut.response);
					}
					//When the file has been retrieved
					xhrOut.onload = function(){
						if(xhrOut.response === '{}'){
							console.log("Invalid File");
						}
						else{
							//Add file name to GPX View options
							let x = document.getElementById("docSelect");
							let option = document.createElement("option");
							option.text = file.name;
							x.add(option);
							
							console.log("File successfully retrieved from /uploads");
							obj = JSON.parse(xhrOut.response);
						
							//Add a new row to the table
							let table = document.getElementById("FilePanel");
							let row = table.insertRow(-1);
							let cell1 = row.insertCell(0);
							let cell2 = row.insertCell(1);
							let cell3 = row.insertCell(2);
							let cell4 = row.insertCell(3);
							let cell5 = row.insertCell(4);
							let cell6 = row.insertCell(5);
				
							//Create the path for the file download
							var path = "/uploads/"+file.name;

				
							//Create the hyperlink
							cell1.innerHTML = "<a href='" + path + "'>" + file.name; + "</a>";
							cell2.innerHTML = obj.version;
							cell3.innerHTML = obj.creator;
							cell4.innerHTML = obj.numWaypoints;
							cell5.innerHTML = obj.numRoutes;
							cell6.innerHTML = obj.numTracks;
							
							if(document.getElementById("docSelect").length > 0){
								let t = document.getElementById("doesntExist");
								t.innerText = "";
							}
							if(document.getElementById("dbLogin").disabled){
								if(document.getElementById("docSelect").length > 0){
									document.getElementById("dbFile").disabled = false;
								}
							}
						}	
					}
					xhrOut.send();
				}
				xhrIn.send(formData);	
			}
		}
	}
	else{
		alert("No File Selected");
		console.log("User tried to submit with no file");
	
	}	
    });
    
    $("#gpxView").click(function (){
	let name = document.getElementById("docSelect").value;
	//Check to see if there is a file selected
	if(name === ''){
		console.log("No file was selected");
	}
	else{
		let table = document.getElementById("GPX Panel");
		//If the table has any rows besides the headers, remove them
		if(table.rows.length > 1){
			let rows = table.rows.length-1;
			for(rows; rows > 0; rows--){
				table.deleteRow(-1);
			}
		}
		
		//Once table is correct size
		xhrOutR = new XMLHttpRequest();
		xhrOutR.open('get', '/routes/' + name, true);
		
		//If the request encounters an error
		xhrOutR.onerror = function(){
			console.log("Error loading routes");
		}
		
		//If the request loads
		xhrOutR.onload = function(){
			let choice = document.getElementById("componentSelect");
			
			//Remove all components from drop down menu
			for(let y = choice.length-1; y >= 0; y--){
				choice.remove(y);
			}
			
			let rArray = JSON.parse(xhrOutR.response);
			//Add each route to the table
			for(let i = 0; i < rArray.length; i++){
				let option = document.createElement("option");
				option.text = "Route " + Number(i+1);
				choice.add(option);
				let row = table.insertRow(-1);
				let cell1 = row.insertCell(0);
				let cell2 = row.insertCell(1);
				let cell3 = row.insertCell(2);
				let cell4 = row.insertCell(3);
				let cell5 = row.insertCell(4);
				
				cell1.innerHTML = "Route " + Number(i+1);
				if(rArray[i].name != 'None'){
					cell2.innerHTML = rArray[i].name;
				}
				cell3.innerHTML = rArray[i].numPoints;
				cell4.innerHTML = rArray[i].len;
				cell5.innerHTML = rArray[i].loop;
			}//Done adding all of the Routes
			
			//Once all of the routes have been loaded, load the tracks
			xhrOutT = new XMLHttpRequest();
			xhrOutT.open('get', '/tracks/' + name, true);
			xhrOutT.onerror = function(){
				console.log("Error getting the Tracks");
			}
			xhrOutT.onload = function(){
				let tArray = JSON.parse(xhrOutT.response);
				
				//Add each track to the table
				for(let j = 0; j < tArray.length; j++){
					let option2 = document.createElement("option");
					option2.text = "Track " + Number(j+1);
					choice.add(option2);
					let brow = table.insertRow(-1);
					let bcell1 = brow.insertCell(0);
					let bcell2 = brow.insertCell(1);
					let bcell3 = brow.insertCell(2);
					let bcell4 = brow.insertCell(3);
					let bcell5 = brow.insertCell(4);
				
					bcell1.innerHTML = "Track " + Number(j+1);
					if(tArray[j].name != 'None'){
						bcell2.innerHTML = tArray[j].name;
					}
					bcell4.innerHTML = tArray[j].len;
					bcell5.innerHTML = tArray[j].loop;
				
				}//Done adding tracks to the table
			}//Done track on load
			xhrOutT.send();		
		
		}//End of Route Load
		xhrOutR.send();
		
	}//End of if(name !=null)
    });//End of the function that executes ater clicking button
    
    $("#att").click(function(){
	let name = document.getElementById("componentSelect").value;
	let docName = document.getElementById("docSelect").value;
	//If there is no name, do nothing
	if(name == ""){
		alert("No component selected");
	}
	else{
		//Split up the component requested
		let splat = name.split(" ");
		
		let xhrOut = new XMLHttpRequest();
		
		//Get the thing
		if(splat[0] == 'Route'){
			xhrOut.open('get', '/rAtt/'+docName+'/'+splat[1], true);
		}
		else{
			xhrOut.open('get', '/tAtt/'+docName+'/'+splat[1], true);
		}
		xhrOut.onerr = function(){
			console.log("error");
		}
		xhrOut.onload = function(){
			console.log("Attributes have been loaded");
			if(xhrOut.response != '[]'){
				let p = JSON.parse(xhrOut.response);
				let attributeString = "";
			
				for(let i = 0; i < p.length; i++){
					attributeString += p[i].name + ": " + p[i].value + "\n";
				}
				alert(attributeString);
			}
			else{
				alert("No Attributes");
			}
		}
		xhrOut.onfail = function(){
			console.log("Failed");
		}
		xhrOut.send();
	}    
    });//End of attribute button being clicked
    
    $("#renameBtn").click(function(){
	let name = document.getElementById("componentSelect").value;
	let docName = document.getElementById("docSelect").value;
	let newName = document.getElementById("rename").value;
	if(name != ""){
		if(newName.includes('"')){
			alert("Invalid Input");
		}
		else{
			let splat = name.split(" ");
			let xhrI = new XMLHttpRequest(); 
			if(newName == ""){
				newName = "None";
			}
		
			if(splat[0] == "Route"){
				xhrI.open('get', '/rName/' + splat[1] + "/" + docName + "/" + newName, true);
			}	
			else{
				xhrI.open('get', '/tName/' + splat[1] + "/" + docName + "/" + newName, true);
			}
		
			xhrI.onerror = function(){
				console.log("Error changing name");
			}
			xhrI.onload = function(){
				if(xhrI.response == 'true'){
					alert(name + " name changed successfully");			
					//Re-load the GPX View table with new value
					let table = document.getElementById("GPX Panel");
					//If the table has any rows besides the headers, remove them
					if(table.rows.length > 1){
						let rows = table.rows.length-1;
						for(rows; rows > 0; rows--){
							table.deleteRow(-1);
						}
					}
		
					//Once table is correct size
					xhrOutR = new XMLHttpRequest();
					xhrOutR.open('get', '/routes/' + docName, true);
		
					//If the request encounters an error
					xhrOutR.onerror = function(){
						console.log("Error loading routes");
					}
		
					//If the request loads
					xhrOutR.onload = function(){
						let choice = document.getElementById("componentSelect");
			
						//Remove all components from drop down menu
						for(let y = choice.length-1; y >= 0; y--){
							choice.remove(y);
						}
				
						let rArray = JSON.parse(xhrOutR.response);
						//Add each route to the table
						for(let i = 0; i < rArray.length; i++){
							let option = document.createElement("option");
							option.text = "Route " + Number(i+1);
								choice.add(option);
							let row = table.insertRow(-1);
							let cell1 = row.insertCell(0);
							let cell2 = row.insertCell(1);
							let cell3 = row.insertCell(2);
							let cell4 = row.insertCell(3);
							let cell5 = row.insertCell(4);
							cell1.innerHTML = "Route " + Number(i+1);
							if(rArray[i].name !== "None"){
								cell2.innerHTML = rArray[i].name;
							}
							cell3.innerHTML = rArray[i].numPoints;
							cell4.innerHTML = rArray[i].len;
							cell5.innerHTML = rArray[i].loop;
						}//Done adding all of the Routes
			
						//Once all of the routes have been loaded, load the tracks
						xhrOutT = new XMLHttpRequest();
						xhrOutT.open('get', '/tracks/' + docName, true);
						xhrOutT.onerror = function(){
							console.log("Error getting the Tracks");
						}
						xhrOutT.onload = function(){
							let tArray = JSON.parse(xhrOutT.response);
				
							//Add each track to the table
							for(let j = 0; j < tArray.length; j++){
								let option2 = document.createElement("option");
								option2.text = "Track " + Number(j+1);
								choice.add(option2);
								let brow = table.insertRow(-1);
								let bcell1 = brow.insertCell(0);
								let bcell2 = brow.insertCell(1);
								let bcell3 = brow.insertCell(2);
								let bcell4 = brow.insertCell(3);
								let bcell5 = brow.insertCell(4);
				
								bcell1.innerHTML = "Track " + Number(j+1);
								if(tArray[j].name != 'None'){
									bcell2.innerHTML = tArray[j].name;
								}
								bcell4.innerHTML = tArray[j].len;
								bcell5.innerHTML = tArray[j].loop;
				
							}//Done adding tracks to the table
						}//Done track on load
						xhrOutT.send();		
		
					}//End of Route Load
					xhrOutR.send();
				}//Close If true
				else{
					alert("Could not change the name of " + name);
				}
			}//Close on load
			xhrI.send();
		}//Close if there is valid input
	}//Close if there is a component selected
	else{
		alert("No Component Selected");
	}
    });//end of Rename button being clicked

    $("#findPaths").click(function(){
	let sLat = document.getElementById("strtLa").value;
	let sLon = document.getElementById("strtLo").value;
	let eLat = document.getElementById("endLa").value;
	let eLon = document.getElementById("endLo").value;
	let acc = document.getElementById("accuracy").value;
	if(acc == "" || eLon== "" || eLat == "" || sLon == "" || sLat == ""){
		alert("Invalid Input");
	}
	else{
		alert("Could not find any paths");
	}//Close if there is valid input
    });//end of find path being clicked

$("#addRte").click(function(){
	let fileName = document.getElementById("docSelect").value;
	let rteName = document.getElementById("rteNme").value;
	let rteLat = document.getElementById("rteLa").value;
	let rteLon = document.getElementById("rteLo").value;
	if(fileName == ""){
		alert("No file has been selected chosen");
	}
	else if((rteLon == "")||(rteLat == "")){
		alert("Invalid Latitude/Longitude");
	}
	else{
		alert("Could not add route");
	}
});

$("#addWpt").click(function(){
	let la = document.getElementById("wptLa").value;
	let lo = document.getElementById("wptLo").value;
	if((la == "")||(lo == "")){
		alert("Invalid Latitude/Longitude");
	}
	else{
		alert("Could not add waypoint");
	}
});

$("#createGPXBtn").click(function(){
	let fileName = document.getElementById("gpxFileName").value;
	let creator = document.getElementById("gpxFileCreator").value;
	if((fileName = "")||(creator == "")){
		alert("Invalid Input");
	}
	else{
		alert("Failed to create file");
	}
});

$("#dbLogin").click(function(){
	let user = document.getElementById("us").value;
	let pass = document.getElementById("ps").value;
	let data = document.getElementById("db").value;
	
	
	if(user != "" && data != ""){
		let xmlHt = new XMLHttpRequest();
		if(pass == ""){
			xmlHt.open('get', '/asyncDB/' + user + '/' + data, true);
		}
		else{
			xmlHt.open('get', '/asyncDB/' + user + '/' + pass + '/' + data, true);

		}
		xmlHt.onload = function(){
			if(xmlHt.response == "true"){
				alert("Database opened");
				document.getElementById("dbLogout").disabled = false;
				document.getElementById("dbLogin").disabled = true;
				if(document.getElementById("docSelect").length > 0){
					document.getElementById("dbFile").disabled = false;
				}
				document.getElementById("dbClear").disabled = false;
				document.getElementById("dbDisplay").disabled = false;
				document.getElementById("dbQ").disabled = false;
			}
			else{
				alert("Failed to open database");
			}
		}
		xmlHt.onerror = function(){
		
		}
		xmlHt.send();
	}
	else{
		alert("Failed to open database");
	
	}
	
	
	

});//close dbLogin Button

$("#dbLogout").click(function(){
	let xmlHt = new XMLHttpRequest();
	xmlHt.open("get", '/closeDB', true);
	xmlHt.onload = function(){
		alert("Database Closed");
		document.getElementById("dbLogin").disabled = false;
		document.getElementById("dbLogout").disabled = true;
		document.getElementById("dbFile").disabled = true;
		document.getElementById("dbClear").disabled = true;
		document.getElementById("dbDisplay").disabled = true;
		document.getElementById("dbQ").disabled = true;
		
	}
	xmlHt.onerror = function(){
		console.log("Some sort of error");
	}
	xmlHt.send();
});


//Store all files
$("#dbFile").click(function(){
	let t = 'true';
	//Retrieve all valid files
	let arr = new Array();
		$("#docSelect option").each(function(){
			arr.push($(this).val());
	
		})
		
	
	for(let i = 0; i < arr.length; i++){
		//Getting File Information
		let xhrO = new XMLHttpRequest();
		xhrO.open('GET', '/upload/' + arr[i], true);
		xhrO.onload = function(){
			let chars = JSON.parse(xhrO.response);	
			let newVar = "INSERT INTO FILE VALUES (null, '" + arr[i] + "', " + chars.version + ", '" + chars.creator + "')";
			
			//Saving File info to server
			let xhrS = new XMLHttpRequest();
			xhrS.open('GET', '/saveDBFiles/' + newVar + '/' + arr[i], true);
			//Sending the file information to the server
			xhrS.onload = function(){
				if(xhrS.response == 'true'){
					console.log("Written to File Table");
					
				}
				else{
					console.log("Could not write to database");
					t = 'false';
				}
			}
			xhrS.onerror = function(){
				console.log("Error encountered");
			}
			xhrS.send();
		}
		xhrO.onerror = function(){
			console.log("Something went wrong");
			alert("Could not store files");
		}
		xhrO.send();
	
		//Getting Route Info
		let xhrRI = new XMLHttpRequest();
		xhrRI.open('GET', '/routes/' + arr[i], true);
		xhrRI.onload = function(){
			if(xhrRI.response != '[]'){
				//Send all of the route info to the server
				let rteArray = xhrRI.response;
				var newVar2 = "INSERT INTO ROUTE (route_id, route_name, route_len, gpx_id) VALUES";
					
				let xhrRO = new XMLHttpRequest();
				xhrRO.open('GET', '/saveDBRoutes/' + newVar2 + '/' + arr[i] + '/' + rteArray, true);
				xhrRO.onload = function(){
			
			
				}
				xhrRO.send();
			}
			
		}
		xhrRI.send();
		
		//Getting Route Point info
// 		let xhrPI = new XMLHttpRequest();
// 		xhrPI.open();
// 		xhrPI.onload = function(){
// 			//Send all of the point info to the server
// 		
// 		}
// 		xhrPI.send();
	
	}//End of for loop
	
	if(t == 'true'){
		alert("Files saved");

	}
});


//Clear all Data
$("#dbClear").click(function(){
	let xmlHt = new XMLHttpRequest();
	xmlHt.open('GET', '/clearDB', true);
	xmlHt.onload = function(){
		alert("Database has been cleared");
	}
	xmlHt.onerror = function(){
		alert("Database could not be cleared");
	}
	xmlHt.send();

});


// Display DB Status
$("#dbDisplay").click(function(){
	let xmlHt = new XMLHttpRequest();
	xmlHt.open('GET', '/displayDB', true);
	xmlHt.onload = function(){
		let y = xmlHt.response;
		alert(y)
		console.log(y);
	}
	xmlHt.onerror = function(){
		console.log("Something went wrong");
	}
	xmlHt.send();
});//Done DB Display Status


//Query Button
$("#dbQ").click(function(){
	let choice = document.getElementById("queryList").value;
	if(choice == 1){
		console.log("Display stuff 1");
	}
	else if(choice == 2){
		console.log("Display stuff 2");
	}
	else if(choice == 3){
		console.log("Display stuff 3");
	}
	else if(choice == 4){
		console.log("Display stuff 4");
	}

});//Close Query button

//End of Page Ready    
});


	      

