

----------------------------------------------------------------------------------
rgtd_log("debug","loading lua cs handler")
OnCreatingMsg = function()
	rgtd_log("debug","creating msg")
	return 0
end
OnPlayingMsg = function()
	rgtd_log("debug","playing msg")
	return 0
end
----------------------------------------------------------------------------------
cs_handler_init = function(cs_handler)
	--rgtd_log("debug","cs_handler"..cs_handler)
	rgtd_register_cs_cmd(cs_handler,12590)
end

