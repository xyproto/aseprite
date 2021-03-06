/* Aseprite
 * Copyright (C) 2001-2014  David Capello
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "app/ui/notifications.h"

#include "app/ui/skin/skin_theme.h"
#include "app/ui/skin/style.h"
#include "base/launcher.h"
#include "ui/menu.h"
#include "ui/paint_event.h"
#include "ui/preferred_size_event.h"

namespace app {

using namespace ui;

static const char* kFlag = "flag";

class NotificationItem : public MenuItem {
public:
  NotificationItem(const char* text, const char* url)
    : MenuItem(text),
      m_url(url) {
  }

protected:
  void onClick() OVERRIDE {
    MenuItem::onClick();

    if (!m_url.empty())
      base::launcher::open_url(m_url);
  }

private:
  std::string m_url;
};

Notifications::Notifications()
  : Button("")
  , m_flagStyle(skin::get_style(kFlag))
  , m_withNotifications(false)
{
}

void Notifications::addLink(const char* text, const char* url)
{
  m_popup.addChild(new NotificationItem(text, url));
  m_withNotifications = true;
}

void Notifications::onPreferredSize(PreferredSizeEvent& ev)
{
  ev.setPreferredSize(gfx::Size(16, 10)*jguiscale()); // TODO hard-coded flag size
}

void Notifications::onPaint(PaintEvent& ev)
{
  Graphics* g = ev.getGraphics();

  skin::Style::State state;
  if (hasMouseOver()) state += skin::Style::hover();
  if (m_withNotifications) state += skin::Style::active();
  if (isSelected()) state += skin::Style::clicked();
  m_flagStyle->paint(g, getClientBounds(), NULL, state);
}

void Notifications::onClick(ui::Event& ev)
{
  m_withNotifications = false;
  invalidate();

  gfx::Rect bounds = getBounds();
  m_popup.showPopup(
    bounds.x - m_popup.getPreferredSize().w,
    bounds.y + bounds.h);
}

} // namespace app
