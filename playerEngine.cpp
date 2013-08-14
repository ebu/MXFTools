/*!
 * \project_name EBU Player
 * \file playerEngine.cpp
 * \brief player engine
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 11 february 2013
 *
 * This software is published in LGPLv3.0
 *
 */


#include "playerEngine.hpp"


playerEngine::playerEngine
	(
		void
	) 
{
	// Initialize Gstreamer engine
	Gst::init();
	// initalize the others state variables
	initStates();
	/* playbin2 plays any media type, choosing an appropriate 
	set of elements and linking them together. */
	playout = Gst::PlayBin2::create("playbin");
	std::cerr << "The playbin " 
	<< (
		(!playout) ? 
		"could not be created" : 
		"has been successfully created" 
		) 
	<<"."<< std::endl;
	// Create a video sink where video (if any) will be drawn
	video_sink = Gst::XImageSink::create("ximagesink");
	std::cerr << "The ximagesink " 
	<< (
		(!video_sink) ? 
		"could not be created" : 
		"has been sucessfully created"
		) 
	<<"." <<std::endl;
	/* Set the playbin's video-sink property so that our video
	sink is used for video display */
	playout->property_video_sink() = video_sink;
	// Get the bus from the pipeline
	bus = playout->get_bus();
	/* Enable synchronous message emission to set up video 
	(if any) at the exact appropriate time */
	bus->enable_sync_message_emission();
	/* Connect to bus's synchronous message signal 
	(this is done so that video_area can be set up for 
	drawing at the exact appropriate time) */
	bus->signal_sync_message().connect(
	sigc::mem_fun(*this, &playerEngine::on_bus_message_sync));
	// Add a bus watch to receive messages from the pipeline's bus
	watch_id = bus->add_watch(
	sigc::mem_fun(*this, &playerEngine::on_bus_message) );
}

playerEngine::~playerEngine()
{	
	// remove bus watch from pipeline's bus
	playout->get_bus()->remove_watch(watch_id);
	// initialize the playbin state
	playout->set_state(Gst::STATE_NULL);
	
}

void playerEngine::initStates
(
	void
)
{
	isHeld = false;
	previousState = Gst::STATE_NULL;
	//pad_probe_id = 0;
}

void playerEngine::on_bus_message_sync
(
	const Glib::RefPtr<Gst::Message>& message
)
{
	
	//std::cout << "debug: on_bus_message: unhandled message=" << G_OBJECT_TYPE_NAME(message->gobj()) << std::endl<<"message type is : "<<message->get_message_type()<<std::endl;
	// ignore anything but 'prepare-xwindow-id' element messages
	if(message->get_message_type() != Gst::MESSAGE_ELEMENT)
		return;

	if(!message->get_structure().has_name("prepare-xwindow-id"))
		return;

	Glib::RefPtr<Gst::Element> element =
	Glib::RefPtr<Gst::Element>::cast_dynamic(message->get_source());

	Glib::RefPtr< Gst::ElementInterfaced<Gst::XOverlay> > xoverlay =
	Gst::Interface::cast <Gst::XOverlay>(element);

	if(xoverlay)
		xoverlay->set_xwindow_id(x_window_id);
}

// This function is used to receive asynchronous messages from playbin's bus
bool playerEngine::on_bus_message
(
	const Glib::RefPtr<Gst::Bus>& /* bus */, 
	const Glib::RefPtr<Gst::Message>& message
)
{
	switch(message->get_message_type())
	{	
		/* an error occured. When the application receives an 
		error message it should stop playback of the pipeline 
		and not assume that more data will be played. */ 
    	case Gst::MESSAGE_ERROR:
    	{
      		Glib::RefPtr<Gst::MessageError> msgError =
      		Glib::RefPtr<Gst::MessageError>::cast_dynamic(message);
			if(msgError)
			{
				Glib::Error err;
				err = msgError->parse();
				std::cerr << "Error: " << err.what() << std::endl;
			} else {
				std::cerr << "Error." << std::endl;
				playout->set_state(Gst::STATE_NULL);
			}
			
		} break;
		/* end-of-stream reached in a pipeline. The application 
		will only receive this message in the PLAYING state and 
		every time it sets a pipeline to PLAYING that is in the 
		EOS state. The application can perform a flushing seek 
		in the pipeline, which will undo the EOS state again. */ 
    	case Gst::MESSAGE_EOS: 
    	{
    		// reset playout state
    		playout->set_state(Gst::STATE_NULL);
    		// disconnect the callback which update the position of the stream
			//progress_timeout.disconnect();
			// init timecode
    		//timecode_position = 0;
    		//setTimecodePosition(0.0);
    		// reset the pad 
    		//reset_pad_prob();
    		// inform that we are at the end of stream
    		(*EOSState).set_active(!(*EOSState).get_active());
    	} break;
    	// undefined message 	
		case Gst::MESSAGE_UNKNOWN: {} break;
		// a warning occured
    	case Gst::MESSAGE_WARNING: {} break;
		// an info message occured 
    	case Gst::MESSAGE_INFO: {} break;
		// a tag was found
    	case Gst::MESSAGE_TAG: {} break;
		/* the pipeline is buffering. When the application receives 
		a buffering message in the PLAYING state for a non-live 
		pipeline it must PAUSE the pipeline until the buffering 
		completes, when the percentage field in the message is 100%. 
		For live pipelines, no action must be performed and the 
		buffering percentage can be used to inform the user about 
		the progress. */
    	case Gst::MESSAGE_BUFFERING: {} break;
		// a state change happened 
    	case Gst::MESSAGE_STATE_CHANGED: {} break;
		/* an element changed state in a streaming thread. 
		This message is deprecated. */ 
    	case Gst::MESSAGE_STATE_DIRTY: {} break;
		// a stepping operation finished
    	case Gst::MESSAGE_STEP_DONE: {} break;
		/* an element notifies its capability of providing a clock. 
		This message is used internally and never forwarded to the 
		application. */ 
    	case Gst::MESSAGE_CLOCK_PROVIDE: {} break;
		/* The current clock as selected by the pipeline became 
		unusable. The pipeline will select a new clock on the next 
		PLAYING state change. The application should set the 
		pipeline to PAUSED and back to PLAYING when this message 
		is received. */ 
    	case Gst::MESSAGE_CLOCK_LOST: {} break;
		// a new clock was selected in the pipeline.
    	case Gst::MESSAGE_NEW_CLOCK: {} break;
		/* the structure of the pipeline changed. This message is 
		used internally and never forwarded to the application. */ 
    	case Gst::MESSAGE_STRUCTURE_CHANGE: {} break;
		/* status about a stream, emitted when it starts, stops, 
		errors, etc */ 
    	case Gst::MESSAGE_STREAM_STATUS: {} break;
		/* message posted by the application, possibly via an 
		application-specific element. */ 
    	case Gst::MESSAGE_APPLICATION: {} break;
		// element-specific message
    	case Gst::MESSAGE_ELEMENT: {} break;
		/* pipeline started playback of a segment. This message is 
		used internally and never forwarded to the application. */ 
    	case Gst::MESSAGE_SEGMENT_START: {} break;
		/* pipeline completed playback of a segment. This message is 
		forwarded to the application after all elements that posted 
		GST_MESSAGE_SEGMENT_START posted a GST_MESSAGE_SEGMENT_DONE 
		message. */
    	case Gst::MESSAGE_SEGMENT_DONE: {} break;
		/* The duration of a pipeline changed. The application can 
		get the new duration with a duration query. */ 
    	case Gst::MESSAGE_DURATION: 
    	{
    		Glib::RefPtr<Gst::MessageDuration> durationMsg = Glib::RefPtr<Gst::MessageDuration>::cast_dynamic(message);
    		if (durationMsg)
			{
				std::cout<<"Duration : "<< durationMsg->parse() / Gst::SECOND<<std::endl;
			}
    	} break;
		/* Posted by elements when their latency changes. The 
		application should recalculate & distribute a new latency.*/
    	case Gst::MESSAGE_LATENCY: {} break;
		/* Posted by elements when they start an ASYNC GstStateChange. 
		This message is not forwarded to the application but is used 
		internally. */ 
    	case Gst::MESSAGE_ASYNC_START: {} break;
		/* Posted by elements when they complete an ASYNC 
		GstStateChange. The application will only receive this 
		message from the toplevel pipeline. */ 
    	case Gst::MESSAGE_ASYNC_DONE: {} break;
		/* Posted by elements when they want the pipeline to 
		change state. This message is a suggestion to the 
		application which can decide to perform the state change 
		on (part of) the pipeline. */ 
    	case Gst::MESSAGE_REQUEST_STATE: {} break;
		// A stepping operation was started
    	case Gst::MESSAGE_STEP_START: {} break;
		/* A buffer was dropped or an element changed its 
		processing strategy for Quality of Service reasons. */ 
    	case Gst::MESSAGE_QOS: {} break;
		// A progress message
    	case Gst::MESSAGE_PROGRESS: {} break;
  	}
	return true;
}

bool playerEngine::on_video_pad_got_buffer
(
	const Glib::RefPtr<Gst::Pad>& pad,
	const Glib::RefPtr<Gst::MiniObject>& data
)
{
	Glib::RefPtr<Gst::Buffer> buffer = Glib::RefPtr<Gst::Buffer>::cast_dynamic(data);

  if(buffer)
  {
    int width_value;
    int height_value;

    Glib::RefPtr<Gst::Caps> caps = buffer->get_caps();

    const Gst::Structure structure = caps->get_structure(0);
    if(structure)
    {
      structure.get_field("width", width_value);
      structure.get_field("height", height_value);
    }

//    videoDrawingArea->set_size_request(-1, 159);

    // Resize to minimum when first playing by making size 
    // smallest then resizing according to video new size:
//    resize(1, 1);
//    check_resize();
  }

  pad->remove_buffer_probe(pad_probe_id);
  pad_probe_id = 0; // Clear probe id to indicate that it has been removed

  return true; // Keep buffer in pipeline (do not throw away)
}

Gst::State playerEngine::getState
(
	void
) 
{
	Gst::State state, pending;
	Gst::StateChangeReturn ret = 
	playout->get_state(state, pending, 1 * Gst::SECOND);

	switch(ret)
	{
		case Gst::STATE_CHANGE_FAILURE :
		{
			std::cout
			<<"Query state failed, hard failure"
			<<std::endl;
		} break;
		case Gst::STATE_CHANGE_SUCCESS :
		{
			std::cout<<"State is "<<getStateString(state)<<std::endl;
			return state;
		}
		case Gst::STATE_CHANGE_ASYNC :
		{
			std::cout
			<<"Query state failed, still performing change"
			<<std::endl;
		} break;
		case Gst::STATE_CHANGE_NO_PREROLL :
		{
			std::cout
			<<"State successfully changed but yet not able to provide data"
			<<std::endl;
		} break;
		
	}	
	return Gst::STATE_NULL;
}

bool playerEngine::setState
(
	Gst::State state
) 
{
	
	Gst::StateChangeReturn ret = playout->set_state(state);

	switch (ret)
	{
		case Gst::STATE_CHANGE_FAILURE : 
		{
			std::cout<<"Failed to set state to "
			<<getStateString(state)
			<<". The Gstreamer core knows an hard failure "
			<<std::endl;
		} break;
		case Gst::STATE_CHANGE_SUCCESS : 
		{
			std::cout<<"state is "<<getStateString(state)<<std::endl;
			if (state == Gst::STATE_NULL) {
				reset_pad_prob();
			}
			return true;
		}
		case Gst::STATE_CHANGE_ASYNC : 
		{
			Gst::State actualState = getState();
			if (state != actualState) 
			{
				std::cout<<"Failed to set state to "
				<<getStateString(state)
				<<". The current state is always "
				<<getStateString(actualState)<<std::endl;
			}
		} break;
		case Gst::STATE_CHANGE_NO_PREROLL : 
		{
			std::cout<<"Failed to set state to "
			<<getStateString(state)
			<<". The Gstreamer core knows an hard failure "
			<<std::endl;
			
		} break;
	}
	// If any failure is detect, reset the pad prob
	reset_pad_prob();
	return false;
}

std::string playerEngine::getStateString
(
	Gst::State state
) 
{
	switch (state) 
	{
		case Gst::STATE_VOID_PENDING:
			return "VOID_PENDING";
		case Gst::STATE_NULL:
			return "NULL";
		case Gst::STATE_READY:
			return "READY";
		case Gst::STATE_PAUSED:
			return "PAUSED";
		case Gst::STATE_PLAYING:
			return "PLAYING";
	}
	return "UNKNOWN";
}

bool playerEngine::isState
(
	Gst::State state
) 
{
	return (state == getState()) ? true : false;
}

void playerEngine::reset_pad_prob
(
	void
)
{
	/* Remove video sink pad buffer probe if after playing, probe 
	id is not zero (means probe was not removed because media had 
	no video and video_pad_got_buffer method never got a chance 
	to remove probe) */
	if(pad_probe_id != 0) 
	{
		video_sink->get_static_pad("sink")->remove_buffer_probe
		(
			pad_probe_id
		);
		pad_probe_id  = 0;		
		// reset timecode duration and position
		timecode_duration = 0;
		timecode_position = 0;
	}
}

void playerEngine::on_videoDrawingArea_realize
	(
		Gtk::DrawingArea * screen
	)
{
	/* When the video area (the drawing area) is realized, Get its 
	X Window ID and save it for when the Gst::XOverlay is ready to 
	accept an ID in which to draw the video. */
	
	#if defined (GDK_WINDOWING_WIN32)
		x_window_id = (guintptr)GDK_WINDOW_HWND 
		(
			screen->get_window()->gobj()
		);
		
	#elif defined (GDK_WINDOWING_QUARTZ)
		x_window_id = gdk_quartz_window_get_nsview 
		(
	  		screen->get_window()->gobj()
	  	);
	  	
	#elif defined (GDK_WINDOWING_X11)
		x_window_id = GDK_WINDOW_XID 
		(
			screen->get_window()->gobj()
		);
	#endif
}

void playerEngine::setFile
(
	std::string path,
	std::string filetitle
) 
{
	// disconnect the callback which update the position of the stream
	if (progress_timeout.connected()) {
		// Set the pipeline to inactive mode
		setState(Gst::STATE_NULL);
		// disconnect
		progress_timeout.disconnect();
		// reset the pad prob
		reset_pad_prob();
	}
	// set the timecode position at the beginning of the file
	timecode_position = 0;
	// set the path to the new file
	playout->property_uri() = path;
	// set the file title
	title = filetitle;
	/* set the callback on_timeout function at a 200ms interval to 
	regularly update the position of the stream. */
	progress_timeout = Glib::signal_timeout().connect
	(
    	sigc::mem_fun
    	(
    		*this, 
    		&playerEngine::on_timeout
    	), 
    	100
    );
    // set a buffer to the pad probe 
    pad_probe_id = video_sink->get_static_pad
    ("sink")->add_buffer_probe
    	(
    		sigc::mem_fun
    		(
    			*this, 
    			&playerEngine::on_video_pad_got_buffer
    		)
    	);
    setState(Gst::STATE_PAUSED);
    on_timeout();
}

void playerEngine::rewind
(
	void
)
{

	isHeld = !isHeld;
	/* Call the rewind timeout function at a 10ms interval to 
	rewind the playback as long as the rewind button is 
	pressed */
	if (isHeld) { 
		// if state not paused
		if (!isState(Gst::STATE_PAUSED)) 
		{
			// backup the previous state
			previousState = getState();
			// Set the pipeline to paused mode
			playout->set_state(Gst::STATE_PAUSED);
		} else {
			previousState = getState();
		}
		// start the timer
		timeout_isHeld =  Glib::signal_timeout().connect
		(
			sigc::mem_fun
			(
				*this, 
				&playerEngine::on_rewind_timeout
			), 
			20);
	} else {
		// stop the timer
		timeout_isHeld.disconnect() ;
		// Set the pipeline to the previous state mode
		playout->set_state(previousState);
	}
}

bool playerEngine::on_rewind_timeout
(
	void
)
{
	gint64 pos = 0;
	#ifdef _WIN32
		GstFormat fmt = GstFormat::GST_FORMAT_TIME;
	#else
		Gst::Format fmt = Gst::FORMAT_TIME;
	#endif

	if(playout->query_position(fmt, pos))
	{
		gint64 newPos = (pos > (gint64)Gst::SECOND) 
			? (pos - (gint64)Gst::SECOND) : 0;

		timecode_position = 
			(
				playout->seek
				(
					(Gst::Format)fmt, 
					Gst::SEEK_FLAG_FLUSH,
					newPos 
				)
			) 
			? newPos : pos;
	}
	return true;
}

void playerEngine::forward
(
	void
)
{
	isHeld = !isHeld;
	/* Call the forward timeout function at a 10ms interval to 
	forward the playback as long as the forward button is 
	pressed */
	if (isHeld) {
		// if state not paused
		if (!isState(Gst::STATE_PAUSED)) 
		{
			// backup the previous state
			previousState = getState();
			// Set the pipeline to paused mode
			playout->set_state(Gst::STATE_PAUSED);
		} else {
			previousState = getState();
		}
		// start the timer
		timeout_isHeld =  Glib::signal_timeout().connect
		(
			sigc::mem_fun
			(
				*this, 
				&playerEngine::on_forward_timeout
			), 
			20);
	} else {
		// stop the timer
		timeout_isHeld.disconnect() ;
		// Set the pipeline to the previous state mode
		playout->set_state(previousState);
	}
}

bool playerEngine::on_forward_timeout
(
	void
)
{
	Gst::Format fmt = Gst::FORMAT_TIME;
	Glib::RefPtr<Gst::Query> query = 
		Gst::QueryPosition::create(fmt);

	if(playout->query(query))
	{
		Glib::RefPtr<Gst::QueryPosition> posQuery = 
		Glib::RefPtr<Gst::QueryPosition>::cast_dynamic(query);

		gint64 pos = posQuery->parse();
		gint64 newPos = 
			((pos + (gint64)Gst::SECOND) < timecode_duration) 
			? (pos + (gint64)Gst::SECOND) : timecode_duration;

		Glib::RefPtr<Gst::Event> event = Gst::EventSeek::create
		(
			1.0, 
			fmt,
			Gst::SEEK_FLAG_FLUSH, 
			Gst::SEEK_TYPE_SET, 
			newPos,
			Gst::SEEK_TYPE_NONE, 
			-1
		);

		Glib::RefPtr<Gst::EventSeek> seekEvent = 
		Glib::RefPtr<Gst::EventSeek>::cast_dynamic(event);

		timecode_position = (playout->send_event(seekEvent)) 
			? newPos : pos ;
	}
	return true;	
}

bool playerEngine::on_timeout
(
	void
)
{
	#ifdef _WIN32
		GstFormat fmt = GstFormat::GST_FORMAT_TIME;
	#else
		Gst::Format fmt = Gst::FORMAT_TIME;
	#endif
	gint64 pos = 0;

	if (
		playout->query_position(fmt, pos)
		&& playout->query_duration(fmt, timecode_duration)
	)
	{
		timecode_position = pos;
	}
	return true;
}

void playerEngine::on_move_timecodePosition
(
	void
)
{
}

void playerEngine::setTimecodePosition
(
	double value
)
{
	const gint64 newPos = gint64(value * timecode_duration);
	
	timecode_position = 
	(
		playout->seek
		(
			Gst::FORMAT_TIME, 
			Gst::SEEK_FLAG_FLUSH,
			newPos 
		)
	) 
	? newPos : newPos;
}

void playerEngine::setVolume
(
	double volume
)
{
	playout->property_volume() = volume;
}

guint64 playerEngine::getDuration
(
	void
)
{
	return timecode_duration;
}


double playerEngine::getCurrentTimecode
(
	void
)
{
	Gst::State current,pending;
	playout->get_state(current, pending, Gst::SECOND);
	return (current==Gst::STATE_PLAYING || current==Gst::STATE_PAUSED) 
		? timecode_position : 0;
}

std::string playerEngine::getTitle
(
	void
)
{
	return title;
}

void playerEngine::setLoadState
(
	Gtk::ToggleButton * state
)
{
	loaded = state;
}

void playerEngine::setEOSState
(
	Gtk::ToggleButton * state
)
{
	EOSState = state;
}
