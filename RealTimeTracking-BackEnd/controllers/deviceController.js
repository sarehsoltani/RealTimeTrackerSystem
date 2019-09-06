const express = require('express');
const fs = require('fs');
const path = require('path');
const moment = require('moment');
var locationModel = require('../models/Location');
var router = express.Router();
router.get("/add",(req,res)=>{
  if(!req.query.lat||!req.query.lon||!req.query.alt||!req.query.date||!req.query.speedOTG)
    return res.sendStatus(400);
  var date = moment(req.query.date,"YYYYMMDDHHmmss.SSS").add(4.5,'h');
  if(!date.isValid())
    return res.sendStatus(400)
  locationModel.create({
    lat:req.query.lat,
    lon:req.query.lon,
    alt:req.query.alt,
    speed:req.query.speedOTG * 1.852,
    date:date.format()
  },function(err,location){
    if(err)
      return res.sendStatus(500)
    return res.sendStatus(200);
  });
});
module.exports = router;
