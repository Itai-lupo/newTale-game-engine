#pragma once
#include "osAPI.hpp"
#include "log.hpp"
#include "surface.hpp"
#include "entityPool.hpp"
#include "linuxWindowsInfoComponent.hpp"

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <time.h>


#include <wayland-client.h>
#include <xdg-shell-client-protocol.h>
#include <xkbcommon/xkbcommon.h>

#include <unistd.h>
#include <string.h>
#include <thread>
#include <functional>
#include <list>
#include <shared_mutex>
#include <condition_variable>

#include <Tracy.hpp>

#include "osEventsData.hpp"

class linuxWindowAPI
{
	private:
        

        static void xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial);
        static constexpr struct xdg_wm_base_listener xdg_wm_base_listener = {
            .ping = xdg_wm_base_ping,
        };

		static void global_registry_handler(void *data, struct wl_registry *registry, uint32_t id, const char *interface, uint32_t version);
		static void global_registry_remover(void *data, struct wl_registry *registry, uint32_t id);
		static constexpr wl_registry_listener registry_listener{
            global_registry_handler,
            global_registry_remover
        };

    public:
        static inline wl_display *display = NULL;
        static inline wl_registry *registry;
        


        struct idIndexes
        {
            uint8_t gen;
            uint8_t index;
            uint8_t renderIndex;
        };        

        static void windowEventListener();
        static inline std::thread *eventListenr;
        static inline entityPool *windowsPool;
        static inline windowsInfoComponent *windowsInfo;
        
    public:
        static void init();
        static void closeApi();

        static windowId createWindow(const windowSpec& windowToCreate);
        static void closeWindow(windowId winId);
        static bool isWindowOpen(windowId winId);

};