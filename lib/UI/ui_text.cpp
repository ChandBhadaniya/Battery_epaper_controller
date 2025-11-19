#include "ui_text.h"
#include "settings.h"
//#include "nvs_header.h"

#define DEFAULT_LANGUAGE LCD_LANG_ET

uint8_t lang ;

void set_lang(language_e language)
{
	if(lang == language)
		return;

	lang = language;
//	Write_Byte(NAMESPACE_LOCAL_DATA, KEY_LANG, language);
}

void read_language()
{
	uint32_t t_lang;
//	if(Read_Byte(NAMESPACE_LOCAL_DATA, KEY_LANG, &t_lang))
	{
		t_lang = DEFAULT_LANGUAGE;
	}
	lang = (language_e)t_lang;
}

language_e get_lang()
{
	return (language_e)lang;
}