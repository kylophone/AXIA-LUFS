var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var spawn = require('child_process').spawn;
var AXIA_LUFS_BIN = "/usr/local/bin/axialufs";

app.get('/', function(req, res){
  res.sendfile('index.html');
});

axiaLufsJSON = spawn(AXIA_LUFS_BIN, ['1']); //Axia channel 1

io.on('connection', function(socket){
  	axiaLufsJSON.stderr.on('data', function (data) { 					
  		socket.emit('LUFS', {'shortTerm' : String(data)});
	});
});

http.listen(3000, function(){
});
