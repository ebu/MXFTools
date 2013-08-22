#ifndef CPP_PLAYERENGINE_CLASS_H
#define CPP_PLAYERENGINE_CLASS_H

/*!
 * \project_name EBU Player
 * \file playerEngine.hpp
 * \brief player engine specifications
 * \details This class is used to load the EBU Player engine and his functionnalities
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 20 august 2012
 * \copyright GNU GPLv3
 *
 */



#include <gtkmm/drawingarea.h>
#include <gtkmm/togglebutton.h>
#if defined (GDK_WINDOWING_X11)
#include <gdk/gdkx.h>
#elif defined (GDK_WINDOWING_WIN32)
#include <gdk/win32/gdkwin32.h>
#elif defined (GDK_WINDOWING_QUARTZ)
#include <gdk/gdkquartz.h>
#endif

#include <glibmm/main.h>
#include <glibmm/miscutils.h>
#include <glibmm/convert.h>

#include <gstreamermm/init.h>
#include <gstreamermm/element.h>
#include <gstreamermm/pipeline.h>
#include <gstreamermm/pad.h>
#include <gstreamermm/bus.h>
#include <gstreamermm/playbin.h>
#include <gstreamermm/caps.h>
#include <gstreamermm/clock.h>
#include <gstreamermm/buffer.h>
#include <gstreamermm/event.h>
#include <gstreamermm/message.h>
#include <gstreamermm/query.h>
#include <gstreamermm/videooverlay.h>

#include <iostream>
#include <string.h>
#include <iterator>
#include <sstream>
#include <iomanip>
#include <time.h>


#include "genericFeatures.hpp"
//#include "metadataWindow.hpp"
//#include "playlistWindow.hpp"

/*! \class playerEngine
 * \brief this class represent the player engine
 *
 * This class controls all the player features
 */

class playerEngine {
	public:
		/*!*
		* @brief Class constructor
		* @brief The playerEngine class constructor instantiates a new player engine and all its features.
		*/
		playerEngine(void);
		/*!*
		* @brief Class destructor
		* @brief The playerEngine class destructor destroys all variables and instanciate objects. This is the garbage collector class. It is always called at the end of class life.
		*
		*/
		virtual ~playerEngine();
			
		/**
		* @fn void on_videoDrawingArea_realize(Gtk::DrawingArea* screen)
		* @brief To create and connect the resources associated with the Gtk.Gdk.Window.
		* @brief This method will create and connect the resources associated with the Gtk.Gdk.Window. Normally the realization signal should be implicit because automatically mapped at widget creation. Unfortunately, first we need to instantiate the GStreamer Object to map it into the Gdk.Window and only after we can realiaze the association.
		* @param[in] screen : Gtk::DrawingArea* is required
		* @return nothing if all is right or an error at compilation time.
		*/
		void on_videoDrawingArea_realize
		(
			Gtk::DrawingArea * screen
		);
		/**
		* @fn void setFile(std::string path, std::string filetitle)
		* @brief To set the path to a media file and the title of the file
		* @brief
		* @note needs more documentation
		* @param[in] std::string path : This standard string is the path to a media file
		* @param[in] std::string filetitle : This standard string is the path to a media file
		* @return a bool if all is right or an error at compilation time.
		*/
		void setFile
		(
			std::string path,
			std::string filetitle
		);
		
		bool setState
		(
			Gst::State state
		);
		
		bool isState
		(
			Gst::State state
		);
		
		Gst::State getState
		(
			void
		);
		
		void rewind
		(
			void
		);
		
		void forward
		(
			void
		);
		void on_move_timecodePosition
		(
			void
		);
				
		void setTimecodePosition
		(
			double value
		);

		guint64 getDuration
		(
			void
		);
		
		double getCurrentTimecode
		(
			void
		);
		
		std::string getTitle
		(
			void
		);
 
		void setVolume
		(
			double volume
		);
	
		/**
		* @fn void setLoadState(Gtk::ToggleButton * state)
		* @brief This function will indicates if a playlist is loaded or not
		* @brief 
		* @note needs more documentation
		* @param[in] Gtk::ToggleButton * state : A toggle button supports signal, a simple bool doesn't support it
		* @return nothing if all is right or an error at compilation time.
		*/
		void setLoadState
		(
			Gtk::ToggleButton * state
		);
	
		/**
		* @fn void setEOSState(Gtk::ToggleButton * state)
		* @brief This function will indicates when the track played from the playlist is ended
		* @brief 
		* @note needs more documentation
		* @param[in] Gtk::ToggleButton * state : A toggle button supports signal, a simple bool doesn't support it
		* @return nothing if all is right or an error at compilation time.
		*/
		void setEOSState
		(
			Gtk::ToggleButton * state
		);
 
	protected:
		Glib::RefPtr<Gst::PlayBin> playout;/*!< playout needs more documentation*/
		//Glib::RefPtr<Gst::XImageSink> video_sink;/*!< video_sink needs more documentation*/
		Glib::RefPtr<Gst::Bus> bus; /*!< bus needs more documentation*/
		guint watch_id;/*!< watch_id needs more documentation*/
		gulong x_window_id;/*!< x_window_id needs more documentation*/
		gulong pad_probe_id;/*!< pad_probe_id needs more documentation*/
		sigc::connection progress_timeout;/*!< progress_timeout needs more documentation*/
		
		gint64 timecode_duration;/*!< timecode_duration the total duration of the played file*/
		gint64 timecode_position; /*!< timecode_position the current timecode position of the played file*/
		std::string title; /*!< title string to store the title of the played file */
		bool isHeld;/*!< isHeld to detect if the rewind or the forward button is held pressed */
		sigc::connection timeout_isHeld;/*!< timeout_fullscreen timer to hide player controls */
		Gst::State previousState; /*!< timeout_fullscreen timer to hide player controls */
		Gtk::ToggleButton * loaded; /*!< loaded This switch indicates if a playlist is loaded or not into the playlist engine */
		Gtk::ToggleButton * EOSState; /*!< EOSState This switch indicates if a playlist is loaded or not into the playlist engine */
		
		/**
		* @fn void initStates(void)
		* @brief To initialize the state variables of the mediaplayer engine.
		* @brief
		* @note needs more documentation
		* @param[in] void : no params
		* @return nothing if all is right or an error at compilation time.
		*/
		void initStates
		(
			void
		);
		
		/*!*
		* @brief This function is used to receive asynchronous messages from mainPipeline's bus
		* @brief This function is used to receive asynchronous messages from mainPipeline's bus, specifically to prepare the Gst::XOverlay to draw inside the window in which we want it to draw to.
		*
		*/
		void on_bus_message_sync
		(
			const Glib::RefPtr<Gst::Message>& message
		);
		/*!*
		* @brief This function is used to receive asynchronous messages from playbin's bus
		* @brief This function is used to receive asynchronous messages from playbin's bus.
		*
		*/
		bool on_bus_message
		(
			const Glib::RefPtr<Gst::Bus>& /* bus */, 
			const Glib::RefPtr<Gst::Message>& message
		);
		
		/*!*
		* @fn on_video_pad_got_buffer(const Glib::RefPtr<Gst::Pad>& pad, const Glib::RefPtr<Gst::MiniObject>& buffer)
		* @brief
		* @brief
		* @note needs more documentation
		* @param[in] pad : const Glib::RefPtr<Gst::Pad>&
		* @param[in] buffer : const Glib::RefPtr<Gst::MiniObject>&
		* @return a bool if all is right or an error at compilation time.
		*/
		Gst::PadProbeReturn on_video_pad_got_buffer
		(
			const Glib::RefPtr<Gst::Pad>& pad,
        	const Gst::PadProbeInfo& data
		);
		
		std::string getStateString
		(
			Gst::State state
		);
		
		void reset_pad_prob
		(
			void
		);
		
		/**
		* @fn bool on_timeout(void)
		* @brief This callback updates punctually the timecode position
		* @brief
		* @note needs more documentation
		* @param[in] void : no params
		* @return a bool if all is right or an error at compilation time.
		*/
		bool on_timeout
		(
			void
		);
		
		bool on_rewind_timeout
		(
			void
		);
		
		bool on_forward_timeout
		(
			void
		);
		
		void on_video_changed
		(
			void
		);
};

#endif
