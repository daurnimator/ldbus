require "ldbus"

function sendsignal ( sigvalue )
	print ( "Sending signal with value " .. sigvalue )
	
	local conn = assert ( ldbus.bus.get ( "session" ) )
	assert ( ldbus.bus.request_name ( conn , "test.signal.source" , { replace_existing = true } ) )
	local msg = assert ( ldbus.message.new_signal ( "/test/signal/Object" , "test.signal.Type" , "Test" ) , "Message Null" )
	
	local iter = ldbus.message.iter.new ( )
	msg:iter_init_append ( iter )
	
	assert ( iter:append_basic ( sigvalue ) , "Out of Memory" )
	
	local ok , serial = assert ( conn:send ( msg ) )
	conn:flush ( )
	
	print ( "Signal Sent" )
end

function query ( param )
	print ( "Calling remote method with " .. param )
	
	local conn = assert ( ldbus.bus.get ( "session" ) )
	assert ( ldbus.bus.request_name ( conn , "test.signal.source" , { replace_existing = true } ) )
	
	local msg = assert ( ldbus.message.new_method_call ( "test.method.server" , "/test/method/Object" , "test.method.Type" , "Method" ) , "Message Null" )
	local iter = ldbus.message.iter.new ( )
	msg:iter_init_append ( iter )
	
	assert ( iter:append_basic ( param ) , "Out of Memory" )
	
	local reply = assert ( conn:send_with_reply_and_block ( msg ) )
	assert ( reply:iter_init ( iter ) , "Message has no arguments" )
	assert ( iter:get_arg_type ( ) == ldbus.types.boolean , "Argument not boolean!" )
	local stat = iter:get_basic ( )
	assert ( iter:next ( ) )
	assert ( iter:get_arg_type ( ) == ldbus.types.uint32 , "Argument not int!" )
	local level = iter:get_basic ( )
	print( "Got reply: " .. tostring ( stat ) .. ", " .. level )
end

local function reply_to_method_call ( msg , conn )
	local stat , level = true , 21614
	
	local iter = ldbus.message.iter.new ( )
	assert ( msg:iter_init ( iter ) , "Message has no arguments" )
	
	assert ( iter:get_arg_type ( ) == ldbus.types.string , "Argument is not a string" )
	local param = iter:get_basic ( )
	
	print ( "Method called with " .. param )
	
	local reply = assert ( msg:new_method_return ( ) )
	reply:iter_init_append ( iter )
	assert ( iter:append_basic ( stat , ldbus.types.uint32 ) , "Out of Memory" )
	assert ( iter:append_basic ( level ) , "Out of Memory" )
	
	local ok , serial = assert ( conn:send ( reply ) , "Out of Memory" )
	conn:flush ( )
end

function listen ( )
	print ( "Listening for method calls" )
	
	local conn = assert ( ldbus.bus.get ( "session" ) )
	assert ( assert ( ldbus.bus.request_name ( conn , "test.method.server" , { replace_existing = true } ) ) == "primary_owner" , "Not Primary Owner" )
	
	while true do
		conn:read_write ( 0 )
		local msg = conn:pop_message ( )
		if not msg then
			os.execute ( "sleep 1" )
		elseif msg:get_type ( ) == "method_call" then
			reply_to_method_call ( msg , conn )
		end
	end
end

function receive ( )
	print ( "Listening for signals" )
	
	local conn = assert ( ldbus.bus.get ( "session" ) )
	assert ( assert ( ldbus.bus.request_name ( conn , "test.signal.sink" , { replace_existing = true } ) ) == "primary_owner" , "Not Primary Owner" )
	
	assert ( ldbus.bus.add_match ( conn , "type='signal',interface='test.signal.Type'" ) )
	conn:flush ( )
	
	print ( "Match rule sent" )
	
	while true do
		conn:read_write ( 0 )
		local msg = conn:pop_message ( )
		if not msg then
			os.execute ( "sleep 1" )
		elseif msg:get_type ( ) == "signal" then
			local iter = ldbus.message.iter.new ( )
			assert ( msg:iter_init ( iter ) , "Message has no parameters" )
			
			assert ( iter:get_arg_type ( ) == ldbus.types.string , "Argument is not a string" )
			local sigvalue = iter:get_basic ( )
			
			print ( "Got signal with value " .. sigvalue )
		end
	end
end

local op = arg [ 1 ]
if op == "send" then
	sendsignal ( arg [ 2 ] or "no param" )
elseif op == "receive" then
	receive ( )
elseif op == "listen" then
	listen ( )
elseif op == "query" then
	query ( arg [ 2 ] or "no param" )
else
	print ( "Syntax: dbus-example [send|receive|listen|query] [<param>]" )
	os.exit ( 1 )
end
