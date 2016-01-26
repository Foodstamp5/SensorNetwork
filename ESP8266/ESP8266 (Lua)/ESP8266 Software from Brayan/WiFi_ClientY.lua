print("Boot complete.")						-- These messages will print to a serial monitor
print("Connecting to PIC16F_Server")		-- Helps with debugging process
wifi.setmode(wifi.STATION)					-- Boot up as a WiFi client
wifi.sta.config ( "PIC16F_Server" , "" )  	-- Set the WiFi parameters to connect
print(wifi.sta.getip())						-- Print an IP address (If any)
print("Starting UDP client connection.")	-- More debugging messages
cu=net.createConnection(net.UDP)			-- Start a UDP server
cu:on("receive",function(cu,c) print(c) end)-- Set the function as to send data
cu:connect(5683,"192.168.4.1")				-- Specify the address to send data to
print("Setting up input modes.")			-- More debugging messages
gpio.mode(3,gpio.INPUT)						-- Set pin3 as an input
gpio.mode(4,gpio.OUTPUT)					-- Set pin4 as an output
gpio.write(4,gpio.LOW)						-- Set pin4 to low
print("Starting timer.")					-- More debugging messages
print("Ready to transmit.")					-- Debugging messages
print("Accepting inputs...")				-- Debugging messages
tmr.alarm(1,50,1,function()					-- Set a timer, setting send frequency .05 seconds
if (wifi.sta.getip()==nil) then				-- This timer will keep track of connection status
  gpio.write(4,0)							-- to user via green LED
else										-- If the LED is off, it is not connected
  gpio.write(4,1)							-- If the LED is on, the client is connected
end
if (gpio.read(3)==1) then					-- This will check the button for a button press
else										-- It will check this every .05 seconds since
print("y")									-- it is under the same timer.  Printing to
cu:send("y")								-- serial monitor to make sure Tx is occurring
end
  end)