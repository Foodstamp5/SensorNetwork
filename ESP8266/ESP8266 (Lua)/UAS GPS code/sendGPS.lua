-- Check Defaults
if(Lat_dec==nil) then Lat_dec=0; end
if(Long_dec==nil) then Long_dec=0; end
if(Alt==nil) then Alt=0; end
if(sendPort==nil) then sendPort=1450; end
if(sendDestination==nil) then sendDestination="255.255.255.255"; end
if(sendTimerId==nil) then sendTimerId=2; end
if(sendDelay==nil) then sendDelay=1000; end  -- ms delay between packets

debugVar=1

-- Send a packet
function sendPacket()
  conn=net.createConnection(net.UDP,0)
  conn:connect(sendPort,sendDestination)
  conn:send("$"..Lat_dec..","..Long_dec..","..Alt)
  conn:close()

  -- Debug stuff
  print("&"..debugVar)
  debugVar=debugVar+1
end

-- Send first packet
tmr.alarm(sendTimerId,sendDelay,1,sendPacket)

print("Sending GPS data to "..sendDestination..":"..sendPort.." at "..1000/sendDelay.." Hz");