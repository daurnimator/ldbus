-- Simple abstraction for variants

local variant_methods = {}
local variant_mt = { __index = variant_methods; }

function variant_methods:get_signature()
	return self._signature
end

function variant_methods:set_signature(new)
	local old = self._signature
	self._signature = new;
	return old
end

function variant_methods:get_value()
	return self._value
end

function variant_methods:set_value(new)
	local old = self._value
	self._value = new
	return old
end

local function new_variant(signature, value)
	local self = setmetatable({
			_signature = nil;
			_value = nil;
		}, variant_mt)
	self:set_signature(signature)
	self:set_value(value)
	return self;
end

local function is_variant(ob)
	return getmetatable(ob) == variant_mt;
end

return {
	new = new_variant;
	is = is_variant;
}
