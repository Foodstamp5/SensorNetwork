wifi.setmode(wifi.STATIONAP)			-- Boot up as a wifi access point
gpio.mode(4,gpio.OUTPUT)				-- Set pin4 as an output
gpio.mode(3,gpio.OUTPUT)				-- Set pin3 as an output
cfg={}; cfg.ssid="PIC16F_Server"; 		-- Set up the wifi parameters, SSID and password
cfg.pwd=""; wifi.ap.config(cfg)			-- Continuing set up
s=net.createServer(net.UDP)				-- Start the UDP server
s:on("receive",function(s,c) print(c)	-- Set a function for what to do when data is
  if (c == "x" ) then					-- received; if receiving x, set pin4 to high
  gpio.write(4, gpio.HIGH)
  c = 0
  elseif (c == "y") then				-- If receiving y, set pin3 as high
  gpio.write(3, gpio.HIGH)
  c = 0
end
 end)
s:listen(5683)							-- Setting the port on which to listen for traffic
tmr.alarm(1,25,1,function()				-- Set a timer, for every .025 seconds:
gpio.write(3,gpio.LOW)					-- Turn off pin3 and
gpio.write(4,gpio.LOW)					-- pin4, effectively making a pulse to PIC input
  end)