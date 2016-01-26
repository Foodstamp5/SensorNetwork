-- Check defaults
if(current_Wifi_mode==nil) then
	current_Wifi_mode=wifi.STATION
end
if(WIFI_MODE_FILE==nil) then
	WIFI_MODE_FILE="wifi_mode.bin"
end

-- Close Web Server
if server~=nil then
  print("Stopping Webserver")
  server:close()
  server=nil
end

if(file.open(WIFI_MODE_FILE)) then
	-- Only runs if the file openned
	temp=tonumber(file.read(1))
	if(temp==1 or temp==2) then
		current_Wifi_mode=temp
	end
	-- Else current_Wifi_mode stays what the main defaults it too
	file.close()
end
-- Call appropriate Wifi file
if(current_Wifi_mode==wifi.STATION) then
	print("Setting ESP to station mode")
	dofile("initWifiStation.lua")
else
	print("Setting ESP to AP mode")
	-- Stop checking wifi status
	tmr.stop(wifiTimerId)

	-- Make ESP an AP
	wifi.setmode(wifi.SOFTAP)
	ip=wifi.ap.getip()
	print("AP initalized")

	-- I can't figure out how to rename the SSID and leave the PASS blank so use the default SSID

	-- Set Heartbeat to connected rate
	blinkDelay=blink_wifi_AP
	dofile("initBlink.lua")

	-- Start server
	dofile("initServer.lua")
end