/*!
 * \project_name EBU Player
 * \file numericEntry.cpp
 * \brief Custom entry : numeric
 * \details This class specification defines a custom entry dedicated to numeric inputs
 * \authors Marco Dos Santos Oliveira
 * \version 0.1
 * \date 3 may 2013
 * \copyright This software is published under MPL v2.0
 *
 */


#include "customEntry.hpp"


customEntry::customEntry
(
	 std::string expression,
	 std::string placeholder
)
{

	regex = Glib::Regex::create(getRegEx(expression));
	//set_visibility(true);
	set_max_length(256);
	set_width_chars(32);
	set_value("");
	set_placeholder_text(placeholder);
	set_name("MetadataEditorAttributeEntryEmpty");
	
	
	css = Gtk::CssProvider::create();
	if(not css->load_from_path("css.css")) {
		std::cout << "Failed to load css"<<std::endl;
	}
	
   style_context = get_style_context();
	style_context->add_provider(css, GTK_STYLE_PROVIDER_PRIORITY_USER);
	
	signal_changed().connect
	(
		sigc::mem_fun
		(
			*this, 
			&customEntry::insert_text_handler
		)
	);
}

customEntry::~customEntry
(
	void
)
{
}

Glib::ustring customEntry::get_value
(
	void
)
{
	return get_text();
}

void customEntry::set_value
(
	Glib::ustring value
)
{
	set_text(value);
}

std::string customEntry::getRegEx
(
	std::string type
)
{
	if (type == "boolean") 
	{
		sample = "i.e. false\n\
							true\n\
							0\n\
							1";
		return "(^((false)|(true)|(False)|(True)|(FALSE)|(TRUE)|(0)|(1))$){1}";
	}
	else if (type == "integer") 
	{
		sample = "i.e. from minus infinite to plus infinite";
		return "^([-+]?([0-9]{1}|[1-9]{1}[0-9]*))$";
	}
	else if (type == "positiveInteger") 
	{
		sample = "i.e. from 1 to infinite";
		return "^([1-9]{1}|[1-9]{1}[0-9]*)$";
				"^([:digit:^0]{1}|[:digit:^0]{1}[:digit:]*)$"
				"^([1-9][0-9]*)$"
	}
	else if (type == "nonNegativeInteger") 
	{
		sample = "i.e. from 0 to infinite";
		return "^([0-9]{1}|[1-9]{1}[0-9]*)$";
	}
	else if (type == "long") 
	{
		sample = "i.e. from -9223372036854775808 to 9223372036854775807";
		return "^(0|[-+]?([1-9][0-9]{0,17}|[1-8][0-9]{18}|9([01][0-9]{17}|2([01][0-9]{16}|2([0-2][0-9]{15}|3([0-2][0-9]{14}|3([0-6][0-9]{13}|7([01][0-9]{12}|20([0-2][0-9]{10}|3([0-5][0-9]{9}|6([0-7][0-9]{8}|8([0-4][0-9]{7}|5([0-3][0-9]{6}|4([0-6][0-9]{5}|7([0-6][0-9]{4}|7([0-4][0-9]{3}|5([0-7][0-9]{2}|80[0-7]))))))))))))))))|-9223372036854775808)$";
	}
	else if (type == "float") 
	{
		return "^[+-]?([0-9]{1}|[1-9]{1}[0-9]*)[.,]{1}([0-9]*)$";
	}
	else if (type == "string") 
	{
		return ".*";
	}
	else if (type == "anyURI") 
	{
	return "^((ftp(s)?|sftp|ssh)[://](.+(:.*)?@)?([a-zA-Z0-9][.]?([a-zA-Z0-9]|(-|\%))*)?([a-zA-Z0-9][.]?([a-zA-Z0-9]|(-|\%))*){3,63}([.]{1}(com|net|org|info|arpa|biz|name|pro|xxx|edu|gov|mil|int|coop|museum|aero|jobs|travel|cat|mobi|tel|asia|post|swiss|([a-z]{2,6})))?(:([1-9][0-9]{0,3}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5]))?(([/][^/]*)*)?|http(s)?(://)(.+(:.*)?@)?([a-zA-Z0-9][.]?([a-zA-Z0-9]|(-|\%))*)?([a-zA-Z0-9][.]?([a-zA-Z0-9]|(-|\%))*){3,63}([.]{1}(com|net|org|info|arpa|biz|name|pro|xxx|edu|gov|mil|int|coop|museum|aero|jobs|travel|cat|mobi|tel|asia|post|swiss|([a-z]{2,6})))?(:([1-9][0-9]{0,3}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5]))?(([/][^/?#]*)+([#][^#?/]+)?([?][^#?=&/]+[=][^#?=&/]+([&][^#?/]+)?([?][^#?=&/]+[=][^#?=&/]+)*)?)?|(gopher|mms|rtsp|rtspu|nntp|telnet)(://)([a-zA-Z0-9][.]?([a-zA-Z0-9]|(-|\%))*)?([a-zA-Z0-9][.]?([a-zA-Z0-9]|(-|\%))*){3,63}([.]{1}(com|net|org|info|arpa|biz|name|pro|xxx|edu|gov|mil|int|coop|museum|aero|jobs|travel|cat|mobi|tel|asia|post|swiss|([a-z]{2,6})))?(:([1-9][0-9]{0,3}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5]))?(([/][^/]*)*)?|((mailto:)[a-zA-Z0-9!#$\%&\'*+-/=?^_`\{|}~\" <>\()\[]çéèàüãô]{1}([.]?[a-zA-Z0-9!#$\%&\'*+-/=?^_`\{|}~\" <>\()\[]çéèàüãô]+)*[@]{1}[a-zA-Z0-9]{1}([.]?[a-zA-Z0-9]+)*[.]{1}(com|net|org|info|arpa|biz|name|pro|xxx|edu|gov|mil|int|coop|museum|aero|jobs|travel|cat|mobi|tel|asia|post|swiss|([a-z]{2,6})){1})|([uU][rR][nN][:][a-zA-Z0-9-]{1,32}[:]([a-zA-Z0-9\()+,-.:=@;$_!*\'/?#]|[\%][a-fA-F0-9]{2})+)|(((/|../|./)*)?([/]?[a-zA-Z0-9])*([#][a-zA-Z0-9]+)?))$";
	}
	else if (type == "language") 
	{
		return "[a-zA-Z]{1,8}(-[a-zA-Z0-9]{1,8})*";
	}
	else if (type == "gYear") 
	{
		return "^([+-]?(0|[1-9][0-9]{0,9}|[1-3][0-9]{10}|40000000000)){1}([Z]|([+-]{1}(00|0[1-9]|1[0-2])[:](00|0[1-9]|[1-5][0-9])))?$";
	}
	else if (type == "date") 
	{
		return "^([+-]?(0|[1-9][0-9]{0,9}|[1-3][0-9]{10}|40000000000)){1}[-]{1}(0[1-9]|1[0-2]){1}[-]{1}(0[1-9]|1[0-9]|2[0-9]|3[0-1]){1}([Z]|([+-]{1}(00|0[1-9]|1[0-2])[:](00|0[1-9]|[1-5][0-9])))?$";
	}
	else if (type == "time") 
	{
		return "^(0[0-9]|1[0-9]|2[0-3]){1}[:]{1}(0[0-9]|[1-5][0-9]){1}[:]{1}(0[0-9]|[1-5][0-9]){1}([.][0-9]{1,5})?([Z]|([+-]{1}(00|0[1-9]|1[0-2])[:](00|0[1-9]|[1-5][0-9])))?$";
	}
	else if (type == "duration") 
	{
		return "^([-+]?P([0-9]{1,64}Y)?([0-9]{1,64}M)?([0-9]{1,64}D)?(T([0-9]{1,64}H)?([0-9]{1,64}M)?(((0|[1-9][0-9]{0,64})|(0|[1-9][0-9]{0,64})[.][0-9]{1,64})S)?)?)$";
	}
	else if (type == "NMTOKEN") 
	{
		return "^[a-zA-Z0-9._:-]+$";
	} 
	else if (type == "ID" || type == "IDREF") 
	{
		return "^([a-zA-Z0-9]|[_]){1}[a-zA-Z0-9_.-]*$";
	}
	else if (type == "IDREFS") 
	{
		return "^(([a-zA-Z0-9]|[_]){1}[a-zA-Z0-9_.-]*)([ ]([a-zA-Z0-9]|[_]){1}[a-zA-Z0-9_.-]*)*$";
	}
	else if (type == "hexBinary") 
	{
		return "^((0[xX])?[0-9a-fA-F]+){1}$";
	}
	return "";
}

void customEntry::insert_text_handler
(
	void
)
{
	if (regex->match(get_value()))
	{
		set_name("MetadataEditorAttributeEntryRight");
	}
	else
	{
		set_name("MetadataEditorAttributeEntryWrong");
	}
}
