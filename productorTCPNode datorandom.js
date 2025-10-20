//ProductorTCPNode

var net = require('net');

var server = net.createServer(function(socket){
	function generate(){
		lim = 0;
		var r = Math.random()*100;
		r=r.toFixed(1);
		lim = lim + 1;
		console.log('sensor_2: %s', r);
		socket.write(r.toString());
		if(limit=200){
			setTimeout(generate,1000);
		}

	return r;
	}
	generate();
});
server.listen(9000,"localhost");