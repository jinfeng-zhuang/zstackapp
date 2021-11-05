#include <gst/gst.h>

int gst_basic_concept(int argc, char* argv[])
{
	GstElement* pipeline, * source, * sink;
	GstBus* bus;
	GstMessage* msg;
	GstStateChangeReturn ret;

	gst_init(&argc, &argv);

	source = gst_element_factory_make("videotestsrc", "source");
	sink = gst_element_factory_make("autovideosink", "sink");

	pipeline = gst_pipeline_new("test-pipeline");

	gst_bin_add_many(pipeline, source, sink, NULL);

	gst_element_link(source, sink);

	g_object_set(source, "pattern", 1, NULL);

	gst_element_set_state(pipeline, GST_STATE_PLAYING);

	bus = gst_element_get_bus(pipeline);

	msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

	switch (GST_MESSAGE_TYPE(msg)) {
	case GST_MESSAGE_ERROR:
		break;
	case GST_MESSAGE_EOS:
		g_print("EOS\n");
		break;
	default:
		break;
	}

	gst_message_unref(msg);
	gst_object_unref(bus);
	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(pipeline);

	return 0;
}
