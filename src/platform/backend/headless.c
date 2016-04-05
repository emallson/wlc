#include <assert.h>
#include <stdbool.h>
#include <chck/math/math.h>
#include "compositor/output.h"
#include "backend.h"
#include "headless.h"
#include "internal.h"

static void
surface_release(struct wlc_backend_surface *bsurface)
{
}

static void
fake_information(struct wlc_output_information *info, uint32_t id)
{
  assert(info);
  wlc_output_information(info);
  chck_string_set_cstr(&info->make, "WLC", false);
  chck_string_set_cstr(&info->model, "Headless", false);
  info->scale = 1;
  info->connector = WLC_CONNECTOR_WLC;
  info->connector_id = id;


  const char *env;
  uint32_t width = 1900, height = 1200;
  if ((env = getenv("WLC_HEADLESS_WIDTH"))) {
    chck_cstr_to_u32(env, &width);
    width = chck_maxu32(width, 1);
  }
  if ((env = getenv("WLC_HEADLESS_HEIGHT"))) {
    chck_cstr_to_u32(env, &height);
    height = chck_maxu32(height, 1);
  }

  struct wlc_output_mode mode = {0};
  mode.refresh = 60 * 1000;
  mode.width = width;
  mode.height = height;
  mode.flags = WL_OUTPUT_MODE_CURRENT | WL_OUTPUT_MODE_PREFERRED;
  wlc_output_information_add_mode(info, &mode);
}

static bool
dummy_flip(struct wlc_backend_surface *bsurface) {
   struct timespec ts;
   wlc_get_time(&ts);
   struct wlc_output *o;
   wlc_output_finish_frame(wl_container_of(bsurface, o, bsurface), &ts);
  return true;
}

static bool
add_output(struct wlc_output_information *info)
{
  struct wlc_backend_surface bsurface;
  if(!wlc_backend_surface(&bsurface, surface_release, 0))
    return false;

  bsurface.window = 0;
  bsurface.display = NULL;
  bsurface.api.page_flip = dummy_flip;

  struct wlc_output_event ev = { .add = {&bsurface, info },
                                 .type = WLC_OUTPUT_EVENT_ADD };
  wl_signal_emit(&wlc_system_signals()->output, &ev);
  return true;
}

static void
terminate(void)
{
  wlc_log(WLC_LOG_INFO, "Closed headless");
}

static uint32_t
update_outputs (struct chck_pool *outputs) {
  /* create some fake outputs */
  const char *env;
  uint32_t fakes = 1;
  if ((env = getenv("WLC_OUTPUTS"))) {
    chck_cstr_to_u32(env, &fakes);
    fakes = chck_maxu32(fakes, 1);
  }

  uint32_t count = 0;
  for(uint32_t i = 0; i < fakes; i++) {
    struct wlc_output_information info;
    fake_information(&info, 1 + i);
    count += (add_output(&info) ? 1 : 0);
  }

  wlc_log(WLC_LOG_INFO, "Updated %d headless outputs", count);
  return count;
}

bool
wlc_headless(struct wlc_backend *backend)
{
  backend->api.update_outputs = update_outputs;
  backend->api.terminate = terminate;
  return true;
}
