//load modules
const express = require('express');
const path = require('path');
const bodyParser = require('body-parser');
const mongoose = require('mongoose');
const compression = require('compression');
var app = express();
//load configs
const dbConfig = require('./configs/db');
const serverConfig = require('./configs/server');
var router = require('./configs/router');
//set app body-bodyParser
app.use(bodyParser.json({limit: '50mb'}));
app.use(bodyParser.urlencoded({limit: '50mb', extended: true}));
//start Database
mongoose.connect('mongodb://'+dbConfig.url+':'+dbConfig.port+'/'+dbConfig.name,{ useNewUrlParser: true });
// add api routes
router.add(app);
//serve front-end files
app.use(express.static(serverConfig.publicDirectoryLocation));
app.use("*",express.static(path.join(serverConfig.publicDirectoryLocation,"index.html")));
//enable server compression
app.use(compression());
// start Server
app.listen(serverConfig.portNumber,function()
{
  console.log("Web Server is Listening to Port "+serverConfig.portNumber);
});
