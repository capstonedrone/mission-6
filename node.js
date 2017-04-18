var http = require('http');
var mysql = require('mysql');
var url = require('url');
var express = require('express');
app = express();


// connect to mysql db
var connection = mysql.createConnection({
        host: 'localhost',
        user: 'sensor',
        password: 'anato39',
        database: 'data'
})
connection.connect();


// r is data
r={};
r.id ='20111650';
r.seq=0;
r.type='T';
r.device='102';
r.unit='0';
r.ip='192.168.0.2';
r.value=10.9;

//select database
connection.query('use data', function(er){
        console.log("SQL : use date");
});

// server
hostname = '127.0.0.1';
port = 80;
server = http.createServer(function (req, res){
        var uri = req.url;
        var query = url.parse(uri,true).query;
        //get method
        if(req.method == 'GET'){
                r.seq++;
                r.value = query.value;
        }

        //insert data
        var q = connection.query('insert into sensors set ?', r, function(er, rows, cols) {
                console.log("insert into sensors set  "+ r.value);
        });

        res.writeHead(200,{'content-Type':'text/html'});

        connection.query('select * from sensors', function(err,rows,fields){
                var data = "no data";
                if(!err){
                        data = "<html><head>20111650 Lee KwangYong Capestone Design Mission</head>";
                        data += "<h1>temperature</h1>";
                        data += "<table border =\"1\">";
                        data += "<tr><th>seq</th><th><th>Temperature</th><th>time</th>          </tr>";
                        for(var i in rows){

                                data += "<tr>";
                                data += "<td>" + rows[i].seq +"</td>";
                                data += "<td>" + rows[i].value+"</td>";
                                data += "<td>" + rows[i].time+"</td>";
                                data += "</tr>";
                        }
                        data += "</table></html>";
                }

                res.end(data);
        });



}).listen(port,'');
