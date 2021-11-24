var http = require('http');
var fs = require('fs');
var index = fs.readFileSync('index.html'); //html page connection
var SerialPort = require("serialport");

const parsers = SerialPort.parsers; //setting parsers
const parser = new parsers.Readline({
    delimiter: '\r\n'
});

var port = new SerialPort('COM3',{ //establishing sensor communication
    baudRate: 9600,
    dataBits: 8, 
    parity: 'none',
    stopBits: 1,
    flowControl: false
});

var portAlarm = new SerialPort('COM4',{ //establishing alert device communication
    baudRate: 9600,
    dataBits: 8, 
    parity: 'none',
    stopBits: 1,
    flowControl: false
});

port.pipe(parser);     //parsing ports
portAlarm.pipe(parser);

var app = http.createServer(function(req,res){ // creating the server
    res.writeHead(200, {'Content-Type':'text/html'});
    res.end(index);
});

var io = require('socket.io').listen(app); // collecting data

io.on('connection',function(data){

    console.log('Node.js is listening!');

});

parser.on('data',function(data){ // sending out data
    console.log(data);
    io.emit('data', data);
    setTimeout(function(){    
        portAlarm.write(data);
        console.log("Alarm received data!");
    }, 3000);
});

app.listen(3000);