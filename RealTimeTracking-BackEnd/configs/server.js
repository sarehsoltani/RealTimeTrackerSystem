var path = require('path');
var server={
  portNumber:8080,
  publicDirectoryLocation:path.join(__dirname,"../public"),
};
module.exports = server;
