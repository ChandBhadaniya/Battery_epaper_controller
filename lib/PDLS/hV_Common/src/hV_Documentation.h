///
/// @file hV_Documentation.h
/// @brief Documentation for the Pervasive Displays Library Suite
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @mainpage Pervasive Displays Library Suite - Evaluation and Commercial editions
///
/// @details The Pervasive Displays Library Suite is specifically designed for the [Pervasive Displays](https://www.pervasivedisplays.com/) e-paper screens and EXT3 extension board.
///
/// @image html Logo-PDI-text-320.jpeg
/// @image latex Logo-PDI-text-320.pdf width=10cm
///
/// It provides a high-level interface to drive the e-paper screens with global and fast update, supports capacitive touch and haptic feed-back.
///
/// The library supports
///
/// + the [EPD Extension Kit Gen 3 (EXT3 or EXT3-1)](https://www.pervasivedisplays.com/product/epd-extension-kit-gen-3-EXT3/) and [Touch Expansion Board for EXT3 (EXT3-Touch)](https://www.pervasivedisplays.com/product/touch-expansion-board-ext3-touch/) boards;
/// + the iTC monochrome [with global update](https://www.pervasivedisplays.com/products/?_sft_product_colour=black-white), [with touch](https://www.pervasivedisplays.com/products/?_sft_etc_itc=tp), [with embedded fast update](https://www.pervasivedisplays.com/products/?_sft_etc_itc=pu&_sft_product_colour=black-white) and [with wide temperature and embedded fast update](https://www.pervasivedisplays.com/products/?_sft_etc_itc=pu&_sft_temperature_range=m15c-to-p60c&_sft_product_colour=black-white) screens;
/// + the iTC colour [black-white-red](https://www.pervasivedisplays.com/products/?_sft_product_colour=black-white-red) and [black-white-red-yellow](https://www.pervasivedisplays.com/products/?_sft_product_colour=black-white-red-yellow) screens;
/// + the [EPD Pico Kit (EPDK)](https://www.pervasivedisplays.com/product/epd-pico-kit-epdk/), which includes a Raspberry Pi Pico RP2040, an EXT3-1 extension board and a 2.66&quot; monochrome panel; and
/// + the [EPD Pico Kit Touch (EPDK-271-Touch)](https://www.pervasivedisplays.com/product/touch-expansion-board-ext3-touch/#tab-3), which includes a Raspberry Pi Pico RP2040, an EXT3-1 and EXT3-Touch boards and a 2.71&quot;-Touch monochrome panel with embedded fast update.
///
/// @b Documentation
///
/// The documentation includes two documents:
///
/// * This **Reference manual** lists all the objects and functions, with details for their parameters and returned results.
///
/// * The [User guide](https://pdls.pervasivedisplays.com/userguide/index.html) focuses on how to use the different libraries, with commented examples.
///
/// Additionally, the **[Wiki](https://docs.pervasivedisplays.com/)** provides a gradual introduction to the e-paper technology and how to use it.
///
/// @author Rei Vilo
/// @date 21 Aug 2023
/// @version 6.1.3
///
/// @copyright &copy; Rei Vilo, 2010-2023
/// @copyright All rights reserved
///
/// * Basic edition: for hobbyists and for basic usage
/// @n Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
///
/// * Evaluation edition: for professionals or organisations, for evaluation only, no commercial usage
/// @n All rights reserved
///
/// * Commercial edition: for professionals or organisations, commercial usage
/// @n All rights reserved
///
/// * Documentation
/// @n All rights reserved
///
/// @b Disclaimer
///
/// The software, including the related documentation, is provided to you "as is," and we make no express or implied warranties whatsoever with respect to its functionality, operability, or use, including, without limitation, any implied warranties of merchantability, fitness for a particular purpose, or infringement. We expressly disclaim any liability whatsoever for any direct, indirect, consequential, incidental or special damages, including, without limitation, lost revenues, lost profits, losses resulting from business interruption or loss of data, regardless of the form of action or legal theory under which the liability may be asserted, even if advised of the possibility or likelihood of such damages.
///
/// @page Examples Examples
///
/// Examples are under the examples folders.
///
/// * Common
///     * Common_Colours.ino
/// @image html T2_PALET.jpg
/// @image latex T2_PALET.jpg width=8cm
///     * Common_Fonts.ino
/// @image html T2_FONTS.jpg
/// @image latex T2_FONTS.jpg width=8cm
///     * Common_Orientation.ino
/// @image html T2_ORIEN.jpg
/// @image latex T2_ORIEN.jpg width=8cm
///     * Common_Forms.ino
/// @image html T2_FORMS.jpg
/// @image latex T2_FORMS.jpg width=8cm
///     * Common_Text.ino
/// @image html T2_CHARA.jpg
/// @image latex T2_CHARA.jpg width=8cm
///     * Common_WhoAmI.ino
/// @image html T2_WHOAMI.jpg
/// @image latex T2_WHOAMI.jpg width=8cm
///
/// * Graphics
///     * Graphics_Clock.ino
/// @image html CLOCK1.jpg
/// @image latex CLOCK1.jpg width=8cm
///     * Graphics_Gauge.ino
/// @image html GAUGE1.jpg
/// @image latex GAUGE1.jpg width=8cm
///     * Graphics_Histogram.ino
/// @image html HISTO200.jpg
/// @image latex HISTO200.jpg width=8cm
///     * Graphics_Multiple.ino
/// @image html MULTI200.jpg
/// @image latex MULTI200.jpg width=8cm
///     * Graphics_Channels.ino
/// @image html CHANN075.jpg
/// @image latex CHANN075.jpg width=8cm
///
/// * GUI
///     * GUI_BarGraph.ino
/// @image html GUI_BarGraph_1.jpg
/// @image latex GUI_BarGraph_1.jpg width=8cm
///     * GUI_Button.ino
/// @image html GUI_Button_0.jpg
/// @image latex GUI_Button_0.jpg width=8cm
///     * GUI_CheckBox.ino
/// @image html GUI_CheckBox_Normal_1.jpg
/// @image latex GUI_CheckBox_Normal_1.jpg width=8cm
///     * GUI_Cursor.ino
/// @image html GUI_Cursor_1.jpg
/// @image latex GUI_Cursor_1.jpg width=8cm
///     * GUI_FlipFlop.ino
/// @image html GUI_FlipFlop_Normal_1.jpg
/// @image latex GUI_FlipFlop_Normal_1.jpg width=8cm
///     * GUI_Label.ino
/// @image html GUI_Label.jpg
/// @image latex GUI_Label.jpg width=8cm
///     * GUI_PlusMinus.ino
/// @image html GUI_PlusMinus_Normal_1.jpg
/// @image latex GUI_PlusMinus_Normal_1.jpg width=8cm
///     * GUI_Radio.ino
/// @image html GUI_RadioButtons_0.jpg
/// @image latex GUI_RadioButtons_0.jpg width=8cm
///     * GUI_Switch.ino
/// @image html GUI_Switch.jpg
/// @image latex GUI_Switch.jpg width=8cm
///     * GUI_Text.ino
/// @image html GUI_Text.jpg
/// @image latex GUI_Text.jpg width=8cm
///     * GUI_TextBox.ino
/// @image html GUI_TextBox_0.jpg
/// @image latex GUI_TextBox_0.jpg width=8cm
///     * GUI_Table.ino
/// @image html GUI_Table_0.jpg
/// @image latex GUI_Table_0.jpg width=8cm
///     * GUI_Keyboard.ino
/// @image html GUI_Keyboard_0.jpg
/// @image latex GUI_Keyboard_0.jpg width=8cm
///
/// * Code
///     * Code_Bar128.ino
/// @image html BAR128N.jpg
/// @image latex BAR128N.jpg width=8cm
///     * Code_Bar39.ino
/// @image html CODE39N.jpg
/// @image latex CODE39N.jpg width=8cm
///     * Code_QR.ino
/// @image html CODEQR2.jpg
/// @image latex CODEQR2.jpg width=8cm
///
/// * File
///     * File_BMP.ino
///
/// * BWRY
///     * BWRY_Colours.ino
/// @image html BWRY_Colours.jpg
/// @image latex BWRY_Colours.jpg width=8cm
///     * BWRY_Palette.ino
/// @image html BWRY_Palette.jpg
/// @image latex BWRY_Palette.jpg width=8cm
///     * BWRY_Contrasts.ino
/// @image html BWRY_Contrasts.jpg
/// @image latex BWRY_Contrasts.jpg width=8cm
///
/// @page References References
/// The [user guide](https://pdls.pervasivedisplays.com/userguide/) provides more information.
///
/// @subsection Colour Colours
/// This [page](https://pdls.pervasivedisplays.com/userguide/Volume9/Section9/#colours) from the user guide provides more information.
///
/// @subsection Coordinate Coordinates
/// This [page](https://pdls.pervasivedisplays.com/userguide/Volume9/Section9/#coordinates) from the user guide provides more information.
///
/// @subsection Area Area
/// Implemented.
///
/// @subsection Fonts Fonts
/// This [page](https://pdls.pervasivedisplays.com/userguide/Volume3/Chapter3/Part2/Fonts/) from the user guide provides more information.
///
/// @subsection Energy Energy
/// Not implemented.
///
/// @subsection Screen Screen
/// This [page](https://pdls.pervasivedisplays.com/userguide/Volume3/Chapter3/Section2/) from the user guide provides more information.
///
/// @subsection Storage Storage
/// This [page](https://pdls.pervasivedisplays.com/userguide/Volume2/Chapter1/Section3/#storage) from the user guide provides more information.
///
/// @subsection Touch Touch
/// This [page](https://pdls.pervasivedisplays.com/userguide/Volume3/Chapter3/Part2/Touch/) from the user guide provides more information.
///
/// @subsection Value Value
/// This [page](https://pdls.pervasivedisplays.com/userguide/Volume4/Chapter4/#values-using-integer-numbers) from the user guide provides more information.
///

