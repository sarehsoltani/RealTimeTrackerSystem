var router = {
 controllers : "../controllers",
 apiPrefix : "/api",
 devicePrefix:"/device",
 userPrefix:"/user",
 add:function(app){
  var path = require('path');
  app.use(this.apiPrefix+this.devicePrefix,require(path.join(this.controllers,'deviceController')));
  app.use(this.apiPrefix+this.userPrefix,require(path.join(this.controllers,'userController')));
  }
};
module.exports = router;
