#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wayland-client.h>

#include "ext-idle-notify-v1-client-protocol.h"

static struct wl_display *display;
static struct wl_registry *registry;
static struct ext_idle_notifier_v1 *idle_notifier;
static struct ext_idle_notification_v1 *idle_notification;

static int timeout_seconds = 300; // 5 minutos por defecto
static const char *lock_command = "swaylock -f";

static void handle_idle(void *data, struct ext_idle_notification_v1 *notif) {
  fprintf(stderr, "[wl-idle-lock] Inactivo por %d segundos, bloqueando...\n",
          timeout_seconds);
  int ret = system(lock_command);
  if (ret == -1) {
    perror("Error al ejecutar comando de bloqueo");
  }
  ext_idle_notification_v1_destroy(notif);
  idle_notification = ext_idle_notifier_v1_get_idle_notification(
      idle_notifier, timeout_seconds * 1000, NULL);
  ext_idle_notification_v1_add_listener(
      idle_notification,
      (struct ext_idle_notification_v1_listener[]){.idled = handle_idle,
                                                   .resumed = NULL},
      NULL);
}

static void handle_resumed(void *data, struct ext_idle_notification_v1 *notif) {
  fprintf(stderr, "[wl-idle-lock] Actividad detectada, reiniciando timer\n");
}

static const struct ext_idle_notification_v1_listener idle_listener = {
    .idled = handle_idle,
    .resumed = handle_resumed,
};

static void registry_handler(void *data, struct wl_registry *reg, uint32_t id,
                             const char *interface, uint32_t version) {
  if (strcmp(interface, ext_idle_notifier_v1_interface.name) == 0) {
    idle_notifier =
        wl_registry_bind(reg, id, &ext_idle_notifier_v1_interface, 1);
  }
}

static void registry_remove(void *data, struct wl_registry *reg, uint32_t id) {}

static const struct wl_registry_listener registry_listener = {
    .global = registry_handler,
    .global_remove = registry_remove,
};

static void sigint_handler(int sig) {
  fprintf(stderr, "\n[wl-idle-lock] Saliendo...\n");
  if (idle_notification)
    ext_idle_notification_v1_destroy(idle_notification);
  if (idle_notifier)
    ext_idle_notifier_v1_destroy(idle_notifier);
  if (registry)
    wl_registry_destroy(registry);
  if (display)
    wl_display_disconnect(display);
  exit(0);
}

int main(int argc, char **argv) {
  if (argc > 1)
    timeout_seconds = atoi(argv[1]);
  if (argc > 2)
    lock_command = argv[2];

  signal(SIGINT, sigint_handler);

  display = wl_display_connect(NULL);
  if (!display) {
    fprintf(stderr, "No se pudo conectar a Wayland.\n");
    return 1;
  }

  registry = wl_display_get_registry(display);
  wl_registry_add_listener(registry, &registry_listener, NULL);
  wl_display_roundtrip(display);

  if (!idle_notifier) {
    fprintf(stderr, "El compositor no soporta ext-idle-notify-v1.\n");
    return 1;
  }

  idle_notification = ext_idle_notifier_v1_get_idle_notification(
      idle_notifier, timeout_seconds * 1000, NULL);
  ext_idle_notification_v1_add_listener(idle_notification, &idle_listener,
                                        NULL);

  fprintf(stderr, "[wl-idle-lock] Iniciado (timeout = %d s, comando = %s)\n",
          timeout_seconds, lock_command);

  while (wl_display_dispatch(display) != -1) {
  }

  return 0;
}
