-- Begin Main --
uart.setup(0,9600,8,0,1,0) -- Do not echo UART commands!
print("Starting RAM: "..node.heap().." Bytes")

-- Init global variables --
SSID="CSUFUAV"
PASS="csufuavisthebest"

WIFI_MODE_FILE="wifi_mode.bin"
current_Wifi_mode=wifi.STATION -- 1 = station, 2 = AP

function save_mode()
	file.remove(WIFI_MODE_FILE)
	file.open(WIFI_MODE_FILE,"w+")
	file.writeline(current_Wifi_mode)
	file.close()
end

refresh_time=10 --seconds for Web Server

blink_Wifi_disconnected=125 -- ms, 8 Hz
blink_Wifi_connecting=20 -- ms, 50 Hz
blink_Wifi_connected=500	-- ms, 2 Hz
blink_wifi_AP=1000				--ms
wifiPrevStatus=-1 			-- force debug to initially update

blinkTimerId=0 	-- Used for the heartbeat
blinkDelay=blink_Wifi_disconnected
wifiTimerId=1 	-- Used to check if wifi is connected
wifiDelay=1000 	-- ms

sendTimerId=2   -- Used to close UDP connection
sendDelay=1000  -- ms delay between packets
--sendDestination="192.168.1.100" -- Motherboard
sendDestination="255.255.255.255" -- Broadcast
sendPort=1450   -- MavLink

blinkLedPin = 3 -- 3=GPIO00, 4=GPIO2
switchWifiModeButton = 4 -- 3=GPIO00, 4=GPIO2

print("Entering main.lua")
dofile("initBlink.lua")
dofile("initWifi.lua")
-- Server started by initWifi.lua on success
dofile("initButtonSwitcher.lua")

-- Crashes so try 
--dofile("sendGPS.lua")

-- End Main --
print("Ending RAM: "..node.heap().." Bytes")