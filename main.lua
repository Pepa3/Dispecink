local socket = require("socket")
local ok, signal = pcall(require, "posix.signal")
if not ok then signal = nil end

local port = arg[1]
if not port then assert(false,"Port unspecified") end
print("Binding on port "..port)
local server = assert(socket.bind("*", port))

local name = os.date("dispecink_%y_%m_%d_%H-%M-%S.log")
local f,err = io.open(name,"a")
if err then assert(false, tostring(err)) end

local quit = false

if signal then
	signal.signal(signal.SIGINT, function(signum)
		quit=true
		io.write("Exiting\n")
		f:close()
		os.exit(128+signum)
	end)
end

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