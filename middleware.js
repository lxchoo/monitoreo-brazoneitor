//recibe tcp(cliente)
var net = require("net");
var socket = require('dgram');
var s = socket.createSocket('udp4');
var s2 = socket.createSocket('udp4');
var fs = require('fs');
const express = require('express');
const {createServer} = require('node:http');
const {join} = require('node:path');
const {Server} = require('socket.io');
const app = express();
const server = createServer(app);
const io = new Server(server);



fs.unlink('datos.txt', (err) => {
    fs.createWriteStream('datos.txt')
});

//          SENSORES
// Sensor 1 udp - Arduino
s.on('message', function(msg, rinfo){
    data = (msg.toString('utf8'));
    console.log('sensor 1(serial):' + data)
    fs.appendFileSync('datos.txt','sensor 1: '+data+'\n');
    io.emit('data2',data);

}).bind(3000, 'localhost');

// Sensor 2 tcp - Random Javascipt
var enchufe_tcp=new net.Socket();
enchufe_tcp.connect(9000,"localhost");
enchufe_tcp.on("data", function(data2){
    data2 = data2.toString();
    console.log('sensor 2(random.js):'+ data2)
    io.emit('lectura',data2)
    fs.appendFileSync('datos.txt','sensor 3: '+data2+'\n')
});
// Sensor 3 udp - Random python
s2.on('message', function(msg, rinfo){
    data1 = (msg.toString('utf8'));
    console.log('sensor 2(random.py): '+data1)
    fs.appendFileSync('datos.txt','sensor 2: '+data1+'\n');
    io.emit('data1',data1);
}).bind(2001, 'localhost');

// comunicacion xmlrpc - Dato procesado (dato*100)
var xmlrpc = require("xmlrpc")
const delay=1000;
var lim=0;
while (lim<10){
    lim=lim+1;
    setTimeout(function(){
        var data3 = parseInt(data1)
        var client = xmlrpc.createClient({host:"localhost",port:5000,path:"/"})
            client.methodCall("Multi", [data3], function(error,value){
                console.log("respuesta:"+value)
                fs.appendFileSync('datos.txt','dato procesado: '+value+'\n');
                io.emit('data2',value);
            });
        }   
    ,delay)
}
//  ENVIA DATOS A HTML 
app.get('/', (rep, res) => {
    res.sendFile(join(__dirname,'index.html'));
});

server.listen(1100, "localhost");

