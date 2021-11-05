#include <gst/gst.h>

typedef struct {
	GstElement* playbin;
	gboolean playing;
	gboolean terminate;
	gboolean seek_enable;
	gboolean seek_done;
	gint64 duration;
} CustomData;

static void handle_message(CustomData* data, GstMessage* msg)
{
	GstState old_state, new_state;
	GstQuery *query;
	gint64 start, end;

	switch (GST_MESSAGE_TYPE(msg)) {
	case GST_MESSAGE_ERROR:
		break;
	case GST_MESSAGE_EOS:
		break;
	case GST_MESSAGE_DURATION:
		data->duration = GST_CLOCK_TIME_NONE; // TODO
		break;
	case GST_MESSAGE_STATE_CHANGED:
		// NULL >> READY >> PAUSED >> PLAYING
		gst_message_parse_state_changed(msg, &old_state, &new_state, NULL);

		if (GST_MESSAGE_SRC(msg) == GST_OBJECT(data->playbin)) {
			g_print("Pipeline state change from %s to %s\n",
				gst_element_state_get_name(old_state),
				gst_element_state_get_name(new_state)
			);

			if (new_state == GST_STATE_PLAYING) {
				data->playing = TRUE;
			}
			else {
				data->playing = FALSE;
			}

			// check if seek enable for this stream after state change to play
			// TODO if query valid in this state
			// new/set/parse
			query = gst_query_new_seeking(GST_FORMAT_TIME);
			if (gst_element_query(data->playbin, query)) {
				// TODO why call parse
				gst_query_parse_seeking(query, NULL, &data->seek_enable, &start, &end);
			}
			gst_query_unref(query);
		}
		break;
	default:
		break;
	}
}

static void handle_timeout(CustomData* data)
{

}

int gst_basic_time(void)
{
	CustomData data;
	GstBus* bus;
	GstMessage* msg;
	GstStateChangeReturn ret;

	memset(&data, 0, sizeof(CustomData));

	data.playbin = gst_element_factory_make("playbin", "playbin");

	g_object_set(data.playbin,
		//"uri", "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm",
		"uri", "file://c/test.mp4",
		NULL);

	gst_element_set_state(data.playbin, GST_STATE_PLAYING);

	bus = gst_element_get_bus(data.playbin);

	while (1) {
		msg = gst_bus_timed_pop_filtered(bus, 100 * GST_MSECOND,
			GST_MESSAGE_ERROR | GST_MESSAGE_STATE_CHANGED | GST_MESSAGE_EOS | GST_MESSAGE_DURATION);

		if (msg)
			handle_message(&data, msg);
		else
			handle_timeout(&data);
	}

	return 0;
}