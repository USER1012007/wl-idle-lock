#include "ext-idle-notify-v1-client-protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <wayland-client.h>

static void global_handler(void *data, struct wl_registry *registry,
                           uint32_t id, const char *interface,
                           uint32_t version);

static void seat_handle_capabilities(void *data, struct wl_seat *seat,
                                     uint32_t capabilities);

static void keyboard_handle_key(void *data, struct wl_keyboard *keyboard,
                                uint32_t serial, uint32_t time, uint32_t key,
                                uint32_t state);

static void keyboard_handle_keymap(void *data, struct wl_keyboard *keyboard,
                                   uint32_t format, int32_t fd, uint32_t size);

int main(void) {
  printf("wl-idle-lock: ejecutándose correctamente.\n");

  struct wl_display *display = wl_display_connect(NULL);

  if (!display) {
    fprintf(stderr, "No se pudo conectar al display Wayland\n");
    return 1;
  }

  wl_display_disconnect(display);

  printf("wl-idle-lock: cerrando correctamente.\n");
  return 0;
}

// global objects from the compositor
static void global_handler(void *data, struct wl_registry *registry,
                           uint32_t id, const char *interface,
                           uint32_t version) {
  if (strcmp(interface, "wl_seat") == 0) {
    seat = wl_registry_bind(registry, id, &wl_seat_interface, 1);
  }
}

// receive keymaps events from the keyboard
static void seat_handle_capabilities(void *data, struct wl_seat *seat,
                                     uint32_t capabilities) {
  if (capabilities & WL_SEAT_CAPABILITY_KEYBOARD) {
    keyboard = wl_seat_get_keyboard(seat);
    wl_keyboard_add_listener(keyboard, &keyboard_listener, NULL);
  }
}

// keymap processing with xkbcommon
static void keyboard_handle_keymap(void *data, struct wl_keyboard *keyboard,
                                   uint32_t format, int32_t fd, uint32_t size) {
  struct xkb_context *context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
  struct xkb_keymap *keymap =
      xkb_keymap_new_from_fd(context, fd, size, XKB_KEYMAP_COMPILE_NO_FLAGS);
  struct xkb_state *xkb_state = xkb_state_new(keymap);
  close(fd);
}

// Process the keycode to get the key symbol
static void keyboard_handle_key(void *data, struct wl_keyboard *keyboard,
                                uint32_t serial, uint32_t time, uint32_t key,
                                uint32_t state) {
  xkb_keysym_t keysym = xkb_state_key_get_one_sym(xkb_state, key + 8);
  if (state == WL_KEYBOARD_KEY_STATE_PRESSED) {
    if (keysym == XKB_KEY_Escape) {
      // Handle Escape key press
    }
  }
}
