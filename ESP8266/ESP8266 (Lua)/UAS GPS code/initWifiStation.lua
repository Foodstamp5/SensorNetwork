-- Check defaults --
if(SSID==nil) then -- Check for default SSID
  SSID="Nick"
  print("Using default SSID ("..SSID..")")
end
if(PASS==nil) then -- Check for default PASS
  PASS="nicksiot"
  print("Using default PASS ("..PASS..")")
end
if(wifiTimerId==nil) then
	wifiTimerId=1
end
if(wifiDelay==nil) then
	wifiDelay=1000
end


-- Setup WiFI --
print("Connecting to "..SSID.."\n")
wifi.setmode(wifi.STATION) --Set station mode
wifi.sta.config(SSID, PASS) -- Set SSID/PASSWORD
wifi.sta.autoconnect(1) -- Enable autoconnect

--Debug Info --
status={
 [0]="Idle",
 [1]="Connecting",
 [2]="Wrong Password",
 [3]="No AP Found",
 [4]="Connection Failed",
 [5]="Have IP"
}

tmr.alarm(wifiTimerId, wifiDelay, 1, function() 
	if wifi.sta.status()==5 then
		if(wifiPrevStatus~=5) then
			ip,subnet,gateway=wifi.sta.getip()
		  print("\n\nWiFi Connected")
		  print("IP: "..ip)
		  print("Subnet Mask: "..subnet)
		  print("Gateway: "..gateway)
		  blinkDelay=blink_Wifi_connected
		  dofile("initBlink.lua")
		  print("RAM: "..node.heap())
  		dofile("initServer.lua")
		end
	elseif wifi.sta.status()==1 then
		if(wifiPrevStatus~=1) then
			print("Connecting to "..SSID.."\n")
		  blinkDelay=blink_Wifi_connecting
		  dofile("initBlink.lua")
		else
	 		uart.write(0,'.')			
	 	end
	elseif(wifi.sta.status()~=wifiPrevStatus) then
	  print("\n\nWiFi Status: ",status[wifi.sta.status()])
	  blinkDelay=blink_Wifi_disconnected
	  dofile("initBlink.lua")
	end

	-- Save last Wifi status
	wifiPrevStatus=wifi.sta.status()
end)


