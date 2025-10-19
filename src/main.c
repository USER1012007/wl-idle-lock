#include "ext-idle-notify-v1-client-protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <wayland-client.h>

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
