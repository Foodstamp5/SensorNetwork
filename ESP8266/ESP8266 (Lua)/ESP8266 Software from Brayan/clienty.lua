print("Boot complete.")
print("Connecting to PIC16F_Server")
wifi.setmode(wifi.STATION)
wifi.sta.config ( "PIC16F_Server" , "" )  
print(wifi.sta.getip())
print("Starting UDP client connection.")
cu=net.createConnection(net.UDP)
cu:on("receive",function(cu,c) print(c) end)
cu:connect(5683,"192.168.4.1")
print("Setting up input modes.")
gpio.mode(3,gpio.INPUT)
gpio.mode(4,gpio.OUTPUT)
gpio.write(4,gpio.LOW)
n=1
print("Starting timer.")
print("Ready to transmit.")
print("Accepting inputs...")
tmr.alarm(1,50,1,function()
if (wifi.sta.getip()==nil) then
  gpio.write(4,0)
else
  gpio.write(4,1)
end
if (gpio.read(3)==1) then
else
print("y")
cu:send("y")
end
  end)