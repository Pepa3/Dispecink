local f = string.format
it = {}
--[[
       14, 15, 16
 1,2, 3, 4, 5, 6,7,8
 8,9,10,11,12,13

XXXxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx side
8-4-4-4-12
5a1f94dc-1bd3-498e-a732-a701ebc5d94f 1-6
]]--
input = "8 9 10 11 12 13 7 8 ; 1 2 10 11 12 13 7 8; 8 9 3 4 5 6 7 8; 1 2 3 14 15 16 7 8"
print(f("Input: %s",input))
a="0"

for c in string.gmatch(input,"[%d;]%d*") do
  if c == ";" then
	a="0"
  else
	if not it[a] then it[a] = {} end

	table.insert(it[a],c)
	a=c
  end
end
for k,v in pairs(it) do
	print(f("%s -> %s",k,table.concat(it[k]," ")))
end
for k1,v1 in pairs(it) do
  for k2,v2 in pairs(v1) do
    for k3,v3 in pairs(v1) do
      if k3>k2 and v2==v3 then table.remove(v1,k3) end
    end
  end
end
for k1,v1 in pairs(it) do
  for k2,v2 in pairs(v1) do
    for k3,v3 in pairs(v1) do
      if k3>k2 and v2==v3 then table.remove(v1,k3) end
    end
  end
end
print("-------------------------")
for k,v in pairs(it) do
	print(f("%s -> %s",k,table.concat(it[k]," ")))
end
