require('dotenv').load()

var express = require('express')
var app = express()
var port = process.env.PORT || 5000
var spark = require('spark')
var SunCalc = require('suncalc')
var CronJob = require('cron').CronJob
var lat = 40.782167
var lon = -73.946797

spark.login({accessToken: process.env.access_token_spark})

spark.getDevice(process.env.device_id_spark, function(err, device) {
  console.log('Device name: ' + device.name)
  var job1 = new CronJob({
    cronTime: '00 48 19 * * 0-6',
    //
    onTick: function() {
      console.log("Creating cron job for morning")
      var times = SunCalc.getTimes(new Date(), lat, lon)
      var nightEnd = times.nightEnd
      var goldenHourEnd = times.goldenHourEnd
      var timeUntilFullLight = goldenHourEnd - nightEnd // in milliseconds
      var job2 = new CronJob({
        cronTime: nightEnd,
        onTick: function() {
          console.log("Sending microcontroller the millaseconds")
          console.log(timeUntilFullLight)
          device.callFunction('startAlarm', timeUntilFullLight, function(error, data) {
            if (err) {
              console.log('An error occurred:', error)
            } else {
              console.log('Function called succesfully:', data)
            }
          })
        },
        onComplete: function () {
          console.log("Alarm sent to microcontroller")
        }
      })
      job2.start()
    },
    //
    onComplete: function() {
      console.log("Alarm is set")
    },
    start: false,
    timeZone: 'America/New_York'
  })
  job1.start()

  var job3 = new CronJob({
    cronTime: '00 00 10 * * 0-6',
    onTick: function() {
      console.log("Turning off lights")
      device.callFunction('turnOff', null, function(error, data) {
        if (err) {
          console.log('An error occurred:', error)
        } else {
          console.log('Function called succesfully:', data)
        }
      })
    },
    onComplete: function () {
      console.log("Turned of the lights")
    }
  })
  job3.start()
})

app.listen(port, function(){
  console.log("Listening on port: " + port)
})
