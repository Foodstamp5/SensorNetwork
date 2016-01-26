-- Check defaults --
if(blinkTimerId==nil) then
	blinkTimerId = 0 -- we have seven timers! 0..6
end
if(blinkDelay==nil) then
	blinkDelay=500
end
if(blinkLedPin==nil) then
	blinkLedPin=3 -- GPIO0
end

-- Blink using timer alarm --
gpio.mode(blinkLedPin,gpio.OUTPUT)
ledState = 0
tmr.stop(blinkTimerId)
tmr.alarm(blinkTimerId, blinkDelay, 1, function() 
  ledState = 1 - ledState;
  gpio.write(blinkLedPin, ledState)
end)

-- Debug info --
response="Heartbeat started on "
-- Find pin --
if(blinkLedPin==3) then 
  response=response.."GPIO0"
elseif(blinkLedPin==4) then
  response=response.."GPIO2"
else
  response=response.."pin "..blinkLedPin
end
response=response.." @ "..1000/blinkDelay.." Hz"
-- Find WiFi status --
if(blinkDelay==blink_Wifi_disconnected) then
	response=response..", Wifi Disconnected"
elseif(blinkDelay==blink_Wifi_connecting) then
	response=response..", Wifi Connecting"
elseif(blinkDelay==blink_Wifi_connected) then
	response=response..", Wifi Connceted"
elseif(blinkDelay==blink_wifi_AP) then
	response=response..", AP Enabled"
end

print(response)