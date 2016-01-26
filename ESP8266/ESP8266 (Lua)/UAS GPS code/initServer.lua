print("Loading initServer")

function body()
  --checkNil()
  if(Min<10) then
    Min_str="0"..Min
  else
    Min_str=tostring(Min)
  end
  if(Sec<10) then
    Sec_str="0"..Sec
  else
    Sec_str=tostring(Sec)
  end
  
  result="ESP Free RAM: "..node.heap().." Bytes <br>"
  result=result.."Latitude: "..Lat_dms.." ("..Lat_dec..")<br>"
  result=result.."Longitude: "..Long_dms.." ("..Long_dec..")<br>"
  result=result.."Altitude: "..Alt.." m<br>"
  result=result.."Satellites: "..Sat.."<br>"
  result=result.."Battery Voltage: "..Bat.." V<br>"
  result=result.."Time: "..Hour..":"..Min_str..":"..Sec_str.."<br>"
  result=result.."Fix: "..Fix.."<br>"
  result=result.."HDOP: "..HDOP.."<br>"
  return(result)  
end

--function checkNil()
  if Lat_dms==nil then
    Lat_dms="No Data Yet"
  end
  if Lat_dec==nil then
    Lat_dec=0
  end
  if Long_dms==nil then
    Long_dms="No Data Yet"
  end
  if Long_dec==nil then
    Long_dec=0
  end
  if Alt==nil then
    Alt=0
  end
  if Sat==nil then
    Sat=0
  end
  if Bat==nil then
    Bat=0
  end
  if Hour==nil then
    Hour=0
  end
  if Min==nil then
    Min=0
  end
  if Sec==nil then
    Sec=0
  end
  if Fix==nil then
    Fix="No Data Yet"
  end
  if HDOP==nil then
    HDOP=0
  end
--end

if port==nil then -- Check default port
  port=80
end
if(refresh_time==nil) then
  refresh_time=10
end
if server~=nil then
  print("Stopping Existing Webserver")
  server:close()
  server=nil
end

print("Initializing Webserver")
server=net.createServer(net.TCP, 30)    -- 30s time out for an inactive client
server:listen(port,function(socket)
  socket:on("receive", function(socket, data)
    print(data)
  end)
  socket:send(
    "HTTP/1.1 200 OK\n"..
    "Content-Type: text/html\n"..
    "Connection: close\n"..
    "Refresh: "..refresh_time.."\n"..
    "\n"..
    "<!DOCTYPE HTML>"..
    "<html>"..
    body()..
    "</html>"
  )
  tmr.delay(10000) -- Wait 10 ms for the client to recieve the data
  socket:close()
end)

-- Debug Info --
if(wifi.getmode()==wifi.STATION) then
  ip,subnet,gateway=wifi.sta.getip()
else
  ip,subnet,gateway=wifi.ap.getip()
end
if(ip==nil) then
  print("Webserver started on port "..port)
else
  print("Webserver started on "..ip..":"..port)
end
print("RAM Remaining: "..node.heap())