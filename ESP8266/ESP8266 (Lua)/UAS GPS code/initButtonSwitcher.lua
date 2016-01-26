-- Check defaults
if(switchWifiModeButton==nil) then
	switchWifiModeButton=4
end

function toggle_wifi_mode(level)
	if(restarting==nil) then
		if(current_Wifi_mode==wifi.STATION) then
			current_Wifi_mode=wifi.SOFTAP
			print("Wifi mode changed to AP")
		else
			current_Wifi_mode=wifi.STATION
			print("Wifi mode changed to Station")
		end
		save_mode()
		restarting=true -- Debounce
		print("Restarting to apply changes")
		node.restart()
	end
end

gpio.mode(switchWifiModeButton,gpio.INT)
gpio.trig(switchWifiModeButton,"down",toggle_wifi_mode)

-- Debug info --
response="Listing for button press on "
if(blinkLedPin==3) then 
  response=response.."GPIO0"
elseif(blinkLedPin==4) then
  response=response.."GPIO2"
else
  response=response.."pin "..blinkLedPin
end

if(current_Wifi_mode==wifi.STATION) then
	response=response.." to change to AP"
else
	response=response.." to change to Station"
end
print(response)