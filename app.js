'use strict'

// C library API
const ffi = require('ffi-napi');
const cLib = ffi.Library('./libgpxparse', {
	'parseGPX': ['string', ['string'] ],
	'getRouteList': ['string', ['string'] ],
	'getTrackList': ['string', ['string'] ],
	'rGetAttList': ['string', ['string', 'int'] ],
	'tGetAttList': ['string', ['string', 'int'] ],
	'rRename': ['bool', ['string', 'int', 'string'] ],
	'tRename': ['bool', ['string', 'int', 'string'] ],
	'rPathList': ['string', ['string', 'float', 'float', 'float', 'float', 'float'] ],
	'newRoute': ['bool', ['string', 'string', 'float', 'float'] ]
});

// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');
const mysql = require('mysql2/promise');
var connection;

app.use(fileUpload());

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/',function(req,res){
  res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function(req, res) {
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }
 
  let uploadFile = req.files.uploadFile;
 
  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err) {
      return res.status(500).send(err);
    }

    res.redirect('/');
  });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/'+req.params.name));
    } else {
      console.log('Error in file downloading route: '+err);
      res.send('');
    }
  });
});

//******************** Your code goes here ******************** 

//Respond to GET requests for files in the uploads/ directory
app.get('/upload/:name', function(req , res){
  let x = cLib.parseGPX(path.join(__dirname+'/uploads/' + req.params.name));
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    if(err == null) {
      if(x == '{}'){
	res.send('{}');
      }
      else{
        res.send(x);
      }

    }
    else{
      console.log('Error in retrieving file: '+err);
      res.send('');
    }
  });
});


//Respond to GET requests for Routes
app.get('/routes/:name', function(req , res){
  let x = cLib.getRouteList(path.join(__dirname+'/uploads/' + req.params.name));
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    if(err == null) {
      if(x == null){
	res.send('');
      }
      else{
        res.send(x);
      }

    }
    else{
      console.log('Error in retrieving file: '+err);
      res.send('');
    }
  });
});


//Respond to GET requests for files in the uploads/ directory
app.get('/tracks/:name', function(req , res){
  let x = cLib.getTrackList(path.join(__dirname+'/uploads/' + req.params.name));
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    if(err == null) {
      if(x == null){
	res.send('');
      }
      else{
        res.send(x);
      }

    }
    else{
      console.log('Error in retrieving file: '+err);
      res.send('');
    }
  });
});

//Get all files that are already in server
app.get('/upload', function(req, res){
	let x = new Array();
	fs.readdir(path.join(__dirname+'/uploads'), function(err, file){
		if(err==null){
			for(var i = 0; i < file.length; i++){
				if(file[i].split('.').pop() == "gpx"){
					x.push(file[i]);
				}
			}
			res.send(x);
		}
		else{
			console.log('Error in getting file names');
			res.send('');
		}
	});
});

app.get('/tAtt/:name/:num', function(req, res){
	let x = cLib.tGetAttList(path.join(__dirname+'/uploads/'+req.params.name), req.params.num);
	res.send(x);
});

app.get('/rAtt/:name/:num', function(req, res){
	let x = cLib.rGetAttList(path.join(__dirname+'/uploads/'+req.params.name), Number(req.params.num));
	res.send(x);
});

app.get('/rName/:num/:fileName/:newName', function(req, res){
	if(req.params.newName == ""){
		console.log("Its technically a string");
	}
	let x = cLib.rRename(path.join(__dirname+'/uploads/'+req.params.fileName), Number(req.params.num), req.params.newName);
	res.send(x);
});

app.get('/tName/:num/:fileName/:newName', function(req, res){
	let x = cLib.tRename(path.join(__dirname+'/uploads/'+req.params.fileName), Number(req.params.num), req.params.newName);
	res.send(x);
});

app.get('/check/:fileName', function(req, res){
	let x = cLib.check(path.join(__dirname+'/uploads/'+req.params.fileName));
	res.send(x);

});

app.get('/rRoutes/:fileName/:l1/:lo1/:l2/:lo2/:d', function(req, res){
	let x = cLib.rPathList(path.join(__dirname+'/uploads/'+req.params.fileName), parseFloat(req.params.l1), parseFloat(req.params.lo1), parseFloat(req.params.l2), parseFloat(req.params.lo2), parseFloat(req.params.d));
	console.log(x);
	if(x == '[]'){
		x = '[{}]';
	}
	res.send(x);
});

app.get('/newRoute/:fileName/:rtenme/:lat/:lon', function(req, res){
	let x = cLib.newRoute(path.join(__dirname+'/uploads/'+req.params.fileName), req.params.rtenme, parseFloat(req.params.lat), parseFloat(req.params.lon));
	res.send(x);

});
//Sample endpoint
app.get('/uploads', function(req , res){

	let h = cLib.parseGPX(req.query.name1);
	res.send(h);
});



app.get("/asyncDB/:user/:pass/:data", async function(req, res, next){
	try{
		connection = await mysql.createConnection({
			host:'dursley.socs.uoguelph.ca',
			user: req.params.user,
			password:req.params.pass,
			database:req.params.data
		})
		
		
		//Check for the FILE table
		let var1 = "select TABLE_NAME from INFORMATION_SCHEMA.TABLES where TABLE_NAME = 'FILE'";
		connection.execute(var1, async function(err, result){
			if(err){
				console.log(err);
			}
			if(result.length == 0){
				let newVar1 = "CREATE TABLE FILE (gpx_id INT AUTO_INCREMENT PRIMARY KEY, file_name VARCHAR(60) NOT NULL, ver DECIMAL(2,1) NOT NULL, creator VARCHAR(256) NOT NULL)";
				connection.execute(newVar1, async function(err, result){
				if(err){
					console.log(err);
				}
			});
			
			}
		});		
		
		
		//Check for the ROUTE table
		let var2 = "select TABLE_NAME from INFORMATION_SCHEMA.TABLES where TABLE_NAME = 'ROUTE'";
		connection.execute(var2, async function(err, result){
			if(err){
				console.log(err);
			}
			if(result.length == 0){
				let newVar2 = "CREATE TABLE ROUTE (route_id INT AUTO_INCREMENT PRIMARY KEY, route_name VARCHAR(256), route_len FLOAT(15,7) NOT NULL, gpx_id INT NOT NULL, FOREIGN KEY(gpx_id) REFERENCES FILE(gpx_id) ON DELETE CASCADE)";
				connection.execute(newVar2, async function(err, result){
				if(err){
					console.log(err);
				}
			});
			}
		});
		
		
		//Check for POINT table
		let var3 = "select TABLE_NAME from INFORMATION_SCHEMA.TABLES where TABLE_NAME = 'POINT'";
		connection.execute(var3, async function(err, result){
			if(err){
				console.log(err);
			}
			if(result.length == 0){
				let newVar3 = "CREATE TABLE POINT (point_id INT AUTO_INCREMENT PRIMARY KEY, point_index INT NOT NULL, latitude DECIMAL(11,7) NOT NULL, longitude DECIMAL(11,7) NOT NULL, point_name VARCHAR(256), route_id INT NOT NULL, FOREIGN KEY(route_id) REFERENCES ROUTE(route_id) ON DELETE CASCADE)";
				connection.execute(newVar3, async function(err, result){
				if(err){
					console.log(err);
				}
			});
			}
		});
		
		res.send(true);
		
	}
	catch(e){
		console.log("Query Error: " + e);
		res.send(false);
	}

});

app.get("/asyncDB/:user/:data", async function(req, res, next){
	try{
		connection = await mysql.createConnection({
			host:'dursley.socs.uoguelph.ca',
			user: req.params.user,
			database:req.params.data
		})
		
		//Check for the FILE table
		let var1 = "select TABLE_NAME from INFORMATION_SCHEMA.TABLES where TABLE_NAME = 'FILE'";
		connection.execute(var1, async function(err, result){
			if(err){
				console.log(err);
			}
			if(result.length == 0){
				let newVar1 = "CREATE TABLE FILE (gpx_id INT AUTO_INCREMENT PRIMARY KEY, file_name VARCHAR(60) NOT NULL, ver DECIMAL(2,1) NOT NULL, creator VARCHAR(256) NOT NULL)";
				connection.execute(newVar1, async function(err, result){
				if(err){
					console.log(err);
				}
			});
			
			}
		});		
		
		
		//Check for the ROUTE table
		let var2 = "select TABLE_NAME from INFORMATION_SCHEMA.TABLES where TABLE_NAME = 'ROUTE'";
		connection.execute(var2, async function(err, result){
			if(err){
				console.log(err);
			}
			if(result.length == 0){
				let newVar2 = "CREATE TABLE ROUTE (route_id INT AUTO_INCREMENT PRIMARY KEY, route_name VARCHAR(256), route_len FLOAT(15,7) NOT NULL, gpx_id INT NOT NULL, FOREIGN KEY(gpx_id) REFERENCES FILE(gpx_id) ON DELETE CASCADE)";
				connection.execute(newVar2, async function(err, result){
				if(err){
					console.log(err);
				}
			});
			}
		});
		
		
		//Check for POINT table
		let var3 = "select TABLE_NAME from INFORMATION_SCHEMA.TABLES where TABLE_NAME = 'POINT'";
		connection.execute(var3, async function(err, result){
			if(err){
				console.log(err);
			}
			if(result.length == 0){
				let newVar3 = "CREATE TABLE POINT (point_id INT AUTO_INCREMENT PRIMARY KEY, point_index INT NOT NULL, latitude DECIMAL(11,7) NOT NULL, longitude DECIMAL(11,7) NOT NULL, point_name VARCHAR(256), route_id INT NOT NULL, FOREIGN KEY(route_id) REFERENCES ROUTE(route_id) ON DELETE CASCADE)";
				connection.execute(newVar3, async function(err, result){
				if(err){
					console.log(err);
				}
			});
			}
		});
		res.send(true);
		
	}
	catch(e){
		console.log("Query Error: " + e);
		res.send(false);
	}

});

//Close the mySql connection
app.get("/closeDB", async function(req, res, next){
	if(connection && connection.end){
		connection.end();
		res.send(true);
	}
	else{
		res.send(true);
	}
});

//Clear the tables
app.get('/clearDB', async function(req, res, next){
	let var1 = "DELETE FROM POINT";
	let var2 = "DELETE FROM ROUTE";
	let var3 = "DELETE FROM FILE";
	try{
		connection.execute(var1);
		connection.execute(var2);
		connection.execute(var3);
		res.send(true);
	}
	catch(e){
		console.log(e);
		res.send(false);
	}
});

app.get('/displayDB', async function(req, res, next){
	var x = "";
	try{
		let var1 = "SELECT COUNT(*) AS fileNum FROM FILE";
		let var2 = "SELECT COUNT(*) AS routeNum FROM ROUTE";
		let var3 = "SELECT COUNT(*) AS pointNum FROM POINT";
	
	
		var [row1, field1] = await connection.execute(var1)
		x = "Database has " + row1[0].fileNum + " files, ";
		
		var [row2, field2] = await connection.execute(var2);
		x = x + row2[0].routeNum + " routes, and ";
		
		var [row3, field3] = await connection.execute(var3);
		x = x + row3[0].pointNum + " points.";
	
		res.send(x);
	
	}
	catch(e){
		console.log(e);
		res.send(false);
	}

});

app.get("/saveDBFiles/:str/:fileName", async function(req, res, next){
	try{
		var [row1, other1] = await connection.execute("SELECT file_name FROM FILE WHERE file_name = '" + req.params.fileName + "'");
		
		if(row1.length == 0){
			try{
				await connection.execute(req.params.str);
				res.send(true);
			}
			catch(e){
				console.log(e);
				res.send(false);
			}
		}
		else{
			res.send(true);
		}
	}
	catch(e){
	console.log(e);
	res.send(false);
	}
});


app.get("/saveDBRoutes/:str/:fileName/:array", async function(req, res, next){
	try{
		var [row1, other1] = await connection.execute("SELECT gpx_id AS gpx_id FROM FILE WHERE file_name = '" + req.params.fileName + "'");
		console.log(row1);
		var txt = req.params.str;
		var arr = JSON.parse(req.params.array);
		
		for(let i = 0; i < arr.length; i++){
			txt = txt + " (null, '" + arr[i].name + "', " + arr[i].len + ", " + row1[0].gpx_id + ")";
			if(Number(i + 1) != arr.length){
				txt = txt + ","
			}
		}
		try{
			await connection.execute(txt);
			res.send(true);
			
		}
		catch(e){
			console.log(e);
			res.send(false);
		}

	}
	catch(e){
		console.log(e);
		res.send(false);
	}
});


app.get("/saveDBPoints/:str/:fileName/:array", async function(req, res, next){
	try{
		var [row1, other1] = await connection.execute("SELECT file_name FROM FILE WHERE file_name = '" + req.params.fileName + "'");
		
		if(row1.length == 0){
			try{
				await connection.execute(req.params.str);
				res.send(true);
			}
			catch(e){
				console.log(e);
				res.send(false);
			}
		}
		else{
			res.send(true);
		}
	}
	catch(e){
	console.log(e);
	res.send(false);
	}
});


app.listen(portNum);
console.log('Running app at localhost: ' + portNum);
