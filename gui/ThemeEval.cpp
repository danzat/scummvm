/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#include "common/util.h"
#include "common/system.h"
#include "common/events.h"
#include "common/hashmap.h"
#include "common/hash-str.h"
#include "common/xmlparser.h"
#include "graphics/scaler.h"

#include "gui/ThemeEngine.h"
#include "gui/ThemeParser.h"
#include "gui/ThemeEval.h"

namespace GUI {

ThemeEval::~ThemeEval() {
	reset();
}

void ThemeEval::buildBuiltinVars() {
	_builtin["kThumbnailWidth"] = kThumbnailWidth;
	_builtin["kThumbnailHeight"] = kThumbnailHeight1;
	_builtin["kThumbnailHeight2"] = kThumbnailHeight2;
	
	_builtin["kButtonWidth"] = GUI::kButtonWidth;
	_builtin["kButtonHeight"] = GUI::kButtonHeight;
	_builtin["kSliderWidth"] = GUI::kSliderWidth;
	_builtin["kSliderHeight"] = GUI::kSliderHeight;
	_builtin["kBigButtonWidth"] = GUI::kBigButtonWidth;
	_builtin["kBigButtonHeight"] = GUI::kBigButtonHeight;
	_builtin["kBigSliderWidth"] = GUI::kBigSliderWidth;
	_builtin["kBigSliderWidth"] = GUI::kBigSliderWidth;
	_builtin["kBigSliderHeight"] = GUI::kBigSliderHeight;
	
	_builtin["kNormalWidgetSize"] = GUI::kNormalWidgetSize;
	_builtin["kBigWidgetSize"] = GUI::kBigWidgetSize;
}

void ThemeEval::reset() {
	_vars.clear();
	_curDialog.clear();
	_curLayout.clear();
	
	for (LayoutsMap::iterator i = _layouts.begin(); i != _layouts.end(); ++i)
		delete i->_value;
		
	_layouts.clear();
}

bool ThemeEval::getWidgetData(const Common::String &widget, int16 &x, int16 &y, uint16 &w, uint16 &h) {
	Common::StringTokenizer tokenizer(widget, ".");
	
	if (widget.hasPrefix("Dialog."))
		tokenizer.nextToken();

	Common::String dialogName = "Dialog." + tokenizer.nextToken();
	Common::String widgetName = tokenizer.nextToken();
	
	if (!_layouts.contains(dialogName)) 
		return false;

	if (widgetName.empty())
		return _layouts[dialogName]->getDialogData(x, y, w, h);
		
	return _layouts[dialogName]->getWidgetData(widgetName, x, y, w, h);
}


void ThemeEval::addWidget(const Common::String &name, int w, int h, const Common::String &type, bool enabled) {	
	int typeW = -1;
	int typeH = -1;
	
	if (!type.empty()) {
		typeW = getVar("Globals." + type + ".Width", -1);
		typeH = getVar("Globals." + type + ".Height", -1);
	}
	
	ThemeLayoutWidget *widget = new ThemeLayoutWidget(_curLayout.top(), name, 
								typeW == -1 ? w : typeW, 
								typeH == -1 ? h : typeH);
	
	_curLayout.top()->addChild(widget);
	setVar(_curDialog + "." + name + ".Enabled", enabled ? 1 : 0);
}

void ThemeEval::addDialog(const Common::String &name, const Common::String &overlays, bool enabled, int inset) {
	int16 x, y;
	uint16 w, h;
	
	ThemeLayout *layout = 0;
	
	if (overlays == "screen") {
		layout = new ThemeLayoutMain(inset, inset, g_system->getOverlayWidth() - 2 * inset, g_system->getOverlayHeight() - 2 * inset);
	} else if (overlays == "screen_center") {
		layout = new ThemeLayoutMain(-1, -1, -1, -1);
	} else if (getWidgetData(overlays, x, y, w, h)) {
		layout = new ThemeLayoutMain(x + inset, y + inset, w - 2 * inset, h - 2 * inset);
	}
	
	if (!layout)
		error("Error when loading dialog position for '%s'", overlays.c_str());
		
	if (_layouts.contains(name))
		delete _layouts[name];
	
	_layouts[name] = layout;

	layout->setPadding(
		getVar("Globals.Padding.Left", 0),
		getVar("Globals.Padding.Right", 0),
		getVar("Globals.Padding.Top", 0),
		getVar("Globals.Padding.Bottom", 0)
		);
	
	_curLayout.push(layout);
	_curDialog = name;
	setVar(name + ".Enabled", enabled ? 1 : 0);
}

void ThemeEval::addLayout(ThemeLayout::LayoutType type, int spacing, bool center) {
	ThemeLayout *layout = 0;
	
	if (spacing == -1)
		spacing = getVar("Globals.Layout.Spacing", 4);
	
	if (type == ThemeLayout::kLayoutVertical)
		layout = new ThemeLayoutVertical(_curLayout.top(), spacing, center);
	else if (type == ThemeLayout::kLayoutHorizontal)
		layout = new ThemeLayoutHorizontal(_curLayout.top(), spacing, center);

	assert(layout);
	
	layout->setPadding(
		getVar("Globals.Padding.Left", 0),
		getVar("Globals.Padding.Right", 0),
		getVar("Globals.Padding.Top", 0),
		getVar("Globals.Padding.Bottom", 0)
		);
	
	_curLayout.top()->addChild(layout);
	_curLayout.push(layout);
}

void ThemeEval::addSpace(int size) {
	ThemeLayout *space = new ThemeLayoutSpacing(_curLayout.top(), size);
	_curLayout.top()->addChild(space);
}

bool ThemeEval::addImportedLayout(const Common::String &name) {
	if (!_layouts.contains(name))
		return false;
		
	_curLayout.top()->importLayout(_layouts[name]);
	return true;
}

} // End of namespace GUI
