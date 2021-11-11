SRC =	main.c \
	client_thread.c \
	client_thread_manager.c \
	log_thread.c \
	CRC16.c \
	network_protocol.c \
	network_protocol_callback_ack.c \
	network_protocol_callback_ping.c \
	network_protocol_callback_simp.c \
	network_protocol_callback_spec.c \
	network_protocol_callback_dcfg.c \
	network_protocol_callback_dcmd.c \
	network_protocol_callback_read.c \
	network_protocol_xg9900.c \
	time.c

xg9901: $(SRC)
	gcc $^ -lpthread -o $@

