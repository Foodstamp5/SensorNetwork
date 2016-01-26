wifi.setmode(wifi.STATIONAP)
gpio.mode(4,gpio.OUTPUT)
gpio.mode(3,gpio.OUTPUT)
cfg={}; cfg.ssid="PIC16F_Server"; cfg.pwd=""; wifi.ap.config(cfg)
s=net.createServer(net.UDP)
s:on("receive",function(s,c) print(c) 
  if (c == "x" ) then
  gpio.write(4, gpio.HIGH)
  c = 0
  elseif (c == "y") then
  gpio.write(3, gpio.HIGH)
  c = 0
end
 end)
s:listen(5683)
tmr.alarm(1,3000,1,function()
gpio.write(3,gpio.LOW)
gpio.write(4,gpio.LOW)
  end)