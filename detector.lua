local net = component.proxy(component.list("internet")())

while true do
  local name, addr, side, old, new = computer.pullSignal()
  if name == "redstone_changed" then
	  local h = net.connect("localhost",25564)
	  local fin, err
	  while not fin and not err do fin,err = h.finishConnect() end
	  if err then h.close() error(tostring(err)) end
	  h.write(addr.." ")
	  h.write(tostring(side).." ")
	  h.write(tostring(new))
	  h.write("\n")
	  local tmp
	  while not tmp or tmp=="" do tmp=h.read() end
	  assert(tmp=="ack\n","ack failed: "..tostring(tmp))
	  h.close()
  end
end