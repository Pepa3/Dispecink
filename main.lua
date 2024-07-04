local socket = require("socket")
local signal = require("posix.signal")
local port = arg[1]
if not port then assert(false,"Port unspecified") end
print("Binding on port "..port)
local server = assert(socket.bind("*", port))

local f,err = io.open("dispecink.log","a")
if err then assert(false, tostring(err)) end

local quit = false

signal.signal(signal.SIGINT, function(signum)
	quit=true
	io.write("Exiting\n")
	f:close()
	os.exit(128+signum)
end)

while not quit do
  local client = server:accept()
  client:settimeout(10)
  local line, err = client:receive()
  print("Received "..tostring(line))
  f:write(line)
  f:write("\n")
  f:flush()
  if not err then client:send("ack\n") end

  client:close()
end