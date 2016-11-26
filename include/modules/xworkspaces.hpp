#pragma once

#include <bitset>

#include "components/config.hpp"
#include "components/types.hpp"
#include "modules/meta/static_module.hpp"
#include "x11/events.hpp"
#include "x11/ewmh.hpp"
#include "x11/icccm.hpp"
#include "x11/window.hpp"

POLYBAR_NS

class connection;

namespace modules {
  enum class desktop_state {
    NONE,
    ACTIVE,
    URGENT,
    EMPTY,
    OCCUPIED,
  };

  enum class viewport_state {
    NONE,
    FOCUSED,
    UNFOCUSED,
  };

  struct viewport {
    position pos;
    string name;
    vector<pair<desktop_state, label_t>> desktops;
    viewport_state state;
    label_t label;
  };

  /**
   * Module used to display EWMH desktops
   */
  class xworkspaces_module : public static_module<xworkspaces_module>, public xpp::event::sink<evt::property_notify> {
   public:
    xworkspaces_module(const bar_settings& bar, const logger& logger, const config& config, string name);

    void setup();
    void start();
    void teardown();
    void handle(const evt::property_notify& evt);
    void update();
    string get_output();
    bool build(builder* builder, const string& tag) const;

   protected:
    void rebuild_desktops();
    void set_current_desktop();

   private:
    static constexpr const char* DEFAULT_ICON{"icon-default"};
    static constexpr const char* DEFAULT_LABEL_STATE{"%icon% %name%"};
    static constexpr const char* DEFAULT_LABEL_MONITOR{"%name%"};

    static constexpr const char* TAG_LABEL_MONITOR{"<label-monitor>"};
    static constexpr const char* TAG_LABEL_STATE{"<label-state>"};

    connection& m_connection;
    ewmh_connection_t m_ewmh;
    vector<monitor_t> m_monitors;
    bool m_monitorsupport{true};

    vector<unique_ptr<viewport>> m_viewports;
    map<desktop_state, label_t> m_labels;
    label_t m_monitorlabel;
    iconset_t m_icons;
    bool m_pinworkspaces{false};
    size_t m_index{0};
  };
}

POLYBAR_NS_END
