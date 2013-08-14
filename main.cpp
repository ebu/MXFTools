/*!
 * project_name EBU Player
 * \file main.cpp
 * \brief EBU Player main file
 * \details In this file, we instanciate all required class 
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 20 august 2012
 * \copyright This software is published in MPLv2.0
 *
 */

/*!
 *
 * \mainpage EBU Player Documentation

This is the documentation of the EBU Player
 *
 */

#include "playerWindow.hpp"
	/**
	 * @fn int main (int argc, char *argv[])
   * @brief This function loads the player design (all windows design).
   * @brief \n \n
	 * @param[in] argc : Optional.
	 * @param[in] argv : Optional.
   * @return nothing if all is right or gtkmm-critical error/segmentation fault
	 * @note This function and his documentation must be completed
   */
int main (int argc, char *argv[]) {
	//Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
	Gtk::Main app(argc, argv);
  //Load the Glade file and instiate its widgets:
  Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
  try
  {
    refBuilder->add_from_file("ebu-mxf-enc-windows.glade");
  }
  catch(const Glib::FileError& err)
  {
    std::cerr << "FileError: " << err.what() << std::endl;
    return 1;
  }
  catch(const Glib::MarkupError& err)
  {
    std::cerr << "MarkupError: " << err.what() << std::endl;
    return 1;
  }
  catch(const Gtk::BuilderError& err)
  {
    std::cerr << "BuilderError: " << err.what() << std::endl;
    return 1;
  }

	playerWindow* mplayer;
	refBuilder->get_widget_derived("PLAYER-EBU-MXF-ENCODER",mplayer);

	Gtk::Main::run( *mplayer );
	delete mplayer;
  return 0;
}
