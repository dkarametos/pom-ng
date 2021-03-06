
require "pom"

output_http = pom.output.new("http", {
	{ "prefix", "string", "/tmp/", "Prefix where to save the files" },
	{ "log_file", "string", "http.log", "Log filename" },
	{ "log_format", "string", "$server_name $client_addr $username $url [$query_time] \"$first_line\" $status $response_size", "Log format" },
	{ "dump_img", "bool", "yes", "Enable dumping images" },
	{ "img_min_surface", "uint32", 300 * 300, "Minimum image surface (height * width)" },
	{ "dump_vid", "bool", "yes", "Enable dumping videos" }
})

function output_http:pload_open(priv, pload)

	local event = pload.event

	-- Make sure the payload is coming from the right event
	if event.name ~= "http_request" then return false end

	local pload_type = pload.type

	-- Payload type is not identified
	if pload_type == nil then return false end

	local class = pload_type['class']

	local process = false

	-- Check for images and the minimum surface
	if class == "image" and self:param_get("dump_img") then
		local surface = pload.data["height"] * pload.data["width"]
		if surface >= self:param_get("img_min_surface") then process = true end
	end

	-- Check for video
	if class == "video" and self:param_get("dump_vid") then process = true end

	-- Process it (or not)
	if not process then return false end

	local data = event.data

	-- If we don't have a complete event and url is missing, then don't process it
	if not data["url"] then return false end

	local filename = self.prefix .. data["server_name"] .. data["url"]
	pom.log(POMLOG_DEBUG, "Saving file into " .. filename)
	self.files:pload_process(pload, { filename = filename } )
	self.log:event_process(event)

	return true

end

function output_http:open()

	-- Open the file plugin that saves payloads to the disk
	self.files = pom.plugin.new("file")
	self.files:open()

	-- Open the log_xml plugin to log requests on the disk
	self.log = pom.plugin.new("log_txt")
	self.log:param_set("filename", self:param_get("log_file"))
	self.log:param_set("event", "http_request")
	self.log:param_set("format", self:param_get("log_format"))
	self.log:open()

	-- Listen to payloads
	self:pload_listen_start(self.pload_open, nil, nil)

	-- We need to listen to the http_request event so it will generate payloads
	self:event_listen_start("http_request")

	-- Copy the prefix parameter for faster execution
	self.prefix = self:param_get("prefix")

end

function output_http:close()

	-- Stop listening to the event http_request
	self:event_listen_stop("http_request")

	-- Stop listening to payloads
	self:pload_listen_stop()

	-- Close the plugins
	self.files:close()
	self.log:close()


end

function output_http_register()
	
	-- Register our new output
	pom.output.register(output_http)	
end


